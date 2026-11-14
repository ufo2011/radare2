/* radare2 - LGPL - Copyright 2026 - pancake */

#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <dlfcn.h>
#include <stdarg.h>
#include "../../libr/io/p/io_r2k_linux.h"

static int device = -1;
static ut8 memory[16];
static struct r2k_data written;

int open(const char *path, int flags, ...) {
	int (*real_open)(const char *, int, ...) = dlsym (RTLD_NEXT, "open");
	mode_t mode = 0;
	if (flags & O_CREAT) {
		va_list ap;
		va_start (ap, flags);
		mode = va_arg (ap, int);
		va_end (ap);
	}
	if (!strcmp (path, "/dev/r2k")) {
		return device = real_open ("/dev/zero", O_RDONLY);
	}
	return real_open (path, flags, mode);
}

int ioctl(int fd, unsigned long request, ...) {
	va_list ap;
	va_start (ap, request);
	void *arg = va_arg (ap, void *);
	va_end (ap);
	if (fd != device || _IOC_TYPE (request) != R2_TYPE) {
		int (*real_ioctl)(int, unsigned long, ...) = dlsym (RTLD_NEXT, "ioctl");
		return real_ioctl (fd, request, arg);
	}
	unsigned int op = _IOC_NR (request);
	if (op <= WRITE_PHYSICAL_ADDR) {
		struct r2k_data *data = arg;
		if (_IOC_SIZE (request) != sizeof (*data) || !data->len || data->len > INT_MAX) {
			errno = EINVAL;
			return -1;
		}
		if (!(op & 1)) {
			if (data->len > sizeof (memory)) {
				errno = EINVAL;
				return -1;
			}
			written = *data;
			memcpy (memory, data->buff, data->len);
		} else if (data->addr == written.addr && data->pid == written.pid && data->len == written.len) {
			memcpy (data->buff, memory, data->len);
		} else {
			memset (data->buff, op + data->pid + (data->addr >> 32), data->len);
		}
	} else if (op == READ_CONTROL_REG) {
		memset (arg, 0, sizeof (struct r2k_control_reg));
		*(size_t *)arg = 1;
	} else if (op == PRINT_PROC_INFO) {
		struct r2k_proc_info *data = arg;
		memcpy (data->comm, "r2k-test", 9);
		data->task = 0xabcdef;
		data->stack = 0x123456;
		data->vmareastruct[0] = 0x1000;
		data->vmareastruct[1] = 0x2000;
		data->vmareastruct[2] = VM_READ | VM_EXEC;
		memcpy (&data->vmareastruct[7], "test", 5);
	} else if (op == GET_KERNEL_MAP) {
		struct r2k_kernel_maps *maps = arg;
		maps->size = getpagesize ();
		maps->n_entries = 1;
	} else {
		errno = EINVAL;
		return -1;
	}
	return 0;
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
	void *(*real_mmap)(void *, size_t, int, int, int, off_t) = dlsym (RTLD_NEXT, "mmap");
	if (fd != device) {
		return real_mmap (addr, len, prot, flags, fd, offset);
	}
	struct r2k_kernel_map_info *map = real_mmap (addr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (map != MAP_FAILED) {
		map->start_addr = 0x1000;
		map->end_addr = 0x2000;
		map->n_pages = map->n_phys_addr = 1;
		map->phys_addr[0] = 0x3000;
	}
	return map;
}

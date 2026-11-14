#include <r_types.h>
#include <r_util/r_log.h>
#include <r_util/r_protobuf.h>

int LLVMFuzzerInitialize(int *lf_argc, char ***lf_argv) {
	r_log_set_quiet (true);
	return 0;
}

int LLVMFuzzerTestOneInput(const ut8 *data, size_t len) {
	char *pb = r_protobuf_decode (data, len, 0);
	free (pb);
	pb = r_protobuf_decode (data, len, 'j');
	free (pb);
	pb = r_protobuf_decode (data, len, 'J');
	free (pb);
	return 0;
}

/* radare - LGPL - Copyright 2026 - pancake */

#include <r_util.h>

typedef struct {
	const char *name;
	const char *utf8;
} REmojiEntry;

static const REmojiEntry emoji_table[] = {
	{ "smile", "\xf0\x9f\x98\x80" },
	{ "grin", "\xf0\x9f\x98\x81" },
	{ "joy", "\xf0\x9f\x98\x82" },
	{ "wink", "\xf0\x9f\x98\x89" },
	{ "heart_eyes", "\xf0\x9f\x98\x8d" },
	{ "thinking", "\xf0\x9f\xa4\x94" },
	{ "cry", "\xf0\x9f\x98\xa2" },
	{ "sob", "\xf0\x9f\x98\xad" },
	{ "angry", "\xf0\x9f\x98\xa0" },
	{ "rage", "\xf0\x9f\x98\xa1" },
	{ "skull", "\xf0\x9f\x92\x80" },
	{ "poop", "\xf0\x9f\x92\xa9" },
	{ "fire", "\xf0\x9f\x94\xa5" },
	{ "star", "\xe2\xad\x90" },
	{ "heart", "\xe2\x9d\xa4" },
	{ "thumbsup", "\xf0\x9f\x91\x8d" },
	{ "thumbsdown", "\xf0\x9f\x91\x8e" },
	{ "ok", "\xf0\x9f\x91\x8c" },
	{ "wave", "\xf0\x9f\x91\x8b" },
	{ "eyes", "\xf0\x9f\x91\x80" },
	{ "warning", "\xe2\x9a\xa0" },
	{ "check", "\xe2\x9c\x85" },
	{ "cross", "\xe2\x9d\x8c" },
	{ "question", "\xe2\x9d\x93" },
	{ "bug", "\xf0\x9f\x90\x9b" },
	{ "rocket", "\xf0\x9f\x9a\x80" },
	{ "bomb", "\xf0\x9f\x92\xa3" },
	{ "lock", "\xf0\x9f\x94\x92" },
	{ "key", "\xf0\x9f\x94\x91" },
	{ "pin", "\xf0\x9f\x93\x8c" },
	{ "snake", "\xf0\x9f\x90\x8d" },
	{ "ghost", "\xf0\x9f\x91\xbb" },
	{ "alien", "\xf0\x9f\x91\xbd" },
	{ "robot", "\xf0\x9f\xa4\x96" },
};

R_API const char *r_emoji_from_name(const char *name) {
	R_RETURN_VAL_IF_FAIL (name, NULL);
	const char *start = name;
	size_t len = strlen (name);
	if (len > 1 && start[0] == ':' && start[len - 1] == ':') {
		start++;
		len -= 2;
	}
	if (!len) {
		return NULL;
	}
	const size_t n = sizeof (emoji_table) / sizeof (emoji_table[0]);
	size_t i;
	for (i = 0; i < n; i++) {
		const REmojiEntry *e = &emoji_table[i];
		if (!strncmp (e->name, start, len) && e->name[len] == '\0') {
			return e->utf8;
		}
	}
	return NULL;
}

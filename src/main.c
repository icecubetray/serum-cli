#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <serum/crypto/hashing/md5.h>




int cmd_md5(int, char **);




typedef int (*subcmd_entry)(int, char **);
struct {
	char cmd[32];
	subcmd_entry ep;
} subcmd_table[] = {
	{ .cmd = "md5", .ep = cmd_md5 }
};




int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("need param");
		return 1;
	}

	unsigned int register i;
	for (i = (sizeof(subcmd_table) / sizeof(*subcmd_table)); i--;) {
		if (strcmp(argv[1], subcmd_table[i].cmd) == 0) {
			return subcmd_table[i].ep(
				(argc - 1),
				&argv[1]
			);
		}
	}

	puts("no exec");
	return 2;
}




int
cmd_md5(int argc, char **argv) {
	if (argc < 2) {
		puts("need param");
		return 1;
	}

	unsigned char digest[16];

	struct serum_md5_context md5;
	serum_md5_init(&md5);
	serum_md5_finish(&md5, digest, argv[1], strlen(argv[1]), strlen(argv[1]));

	fputs("Digest: ", stdout);
	unsigned int i;
	for (i = 0; i < 16; ++i) {
		printf("%02X", digest[i]);
	}
	puts("");

	return 0;
}

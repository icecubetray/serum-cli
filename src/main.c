#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <serum/crypto/hash.h>
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
		serum_interface_hash_init f_init;
		serum_interface_hash_clear f_clear;
		serum_interface_hash_update f_update;
		serum_interface_hash_finish f_finish;

		const unsigned int res = serum_hash_getimpl(SERUM_MD5, &f_init, &f_clear, &f_update, &f_finish);
		if (res != 0) {
			fprintf(stderr, "Error getting hash impl: %u\n", res);
			return (const int)res;
		}

		printf("Ident : %X\n", SERUM_MD5);
		printf("Init  : %p\n", f_init);
		printf("Clear : %p\n", f_clear);
		printf("Update: %p\n", f_update);
		printf("Finish: %p\n", f_finish);
	} else {
		unsigned char digest[16];

		struct serum_md5_context md5;
		serum_md5_init(&md5);
		serum_md5_finish(&md5, digest, argv[1], strlen(argv[1]), strlen(argv[1]));
		serum_md5_clear(&md5);

		fputs("Digest: ", stdout);
		unsigned int i;
		for (i = 0; i < 16; ++i) {
			printf("%02X", digest[i]);
		}
		puts("");
	}

	return 0;
}

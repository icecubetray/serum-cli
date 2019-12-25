#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <serum/crypto/hash.h>
#include <serum/crypto/hashing/debug.h>
#include <serum/crypto/hashing/md5.h>




int cmd_hash_generic(int, char **);




typedef int (*subcmd_entry)(int, char **);
struct {
	char cmd[32];
	subcmd_entry ep;
} subcmd_table[] = {
	{ .cmd = "debug", .ep = cmd_hash_generic },
	{ .cmd = "md5", .ep = cmd_hash_generic }
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
tpl_hash_info(const unsigned int identifier) {
	struct serum_hash_info info;
	if (serum_hash_getinfo(identifier, &info) != SERUM_OK) {
		return 1;
	}

	printf("Name: %s\n", info.name);
	printf("Identifier: 0x%02X\n", info.identifier);
	puts("");
	puts("Sizes:");
	printf(" - %u context\n", info.context_size);
	printf(" - %u block\n", info.block_size);
	printf(" - %u digest\n", info.digest_size);
	puts("");
	puts("Functions:");
	printf(" - %p init\n", (void*)info.f_init);
	printf(" - %p clear\n", (void*)info.f_clear);
	printf(" - %p update\n", (void*)info.f_update);
	printf(" - %p finish\n", (void*)info.f_finish);

	return 0;
}

int
tpl_hash_digest(const unsigned int identifier, const char *const data, const size_t len) {
	struct serum_hash_info info;
	if (serum_hash_getinfo(identifier, &info) != SERUM_OK) {
		return 1;
	}

	unsigned char digest[info.digest_size];
	if (serum_hash(identifier, digest, data, len) != SERUM_OK) {
		return 2;
	}

	unsigned int register i;
	for (i = 0; i < sizeof(digest); ++i) {
		printf("%02x", digest[i]);
	}
	puts("");

	return 0;
}




int
cmd_hash_generic(int argc, char **argv) {
	static struct {
		char name[16];
		unsigned int identifier;
	} table[] = {
		{ "debug", SERUM_HASH_DEBUG },
		{ "md5", SERUM_MD5 }
	};

	unsigned int identifier = 0;
	unsigned int register i;
	for (i = (sizeof(table) / sizeof(*table)); i--;) {
		if (strcmp(table[i].name, argv[0]) == 0) {
			identifier = table[i].identifier;
			break;
		}
	}

	if (identifier == 0) {
		puts("lookup failure");
		return 1;
	}

	if (argc < 2) {
		return tpl_hash_info(identifier);
	} else {
		return tpl_hash_digest(identifier, argv[1], strlen(argv[1]));
	}

	return 0;
}

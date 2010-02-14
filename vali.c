#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/hmac.h>

#include "key.c"

#define G_RECORD_LEN 32

int parse_g_record(const char *s, unsigned char *g_record)
{
	if (*s++ != 'G')
		return -1;
	for (int i = 0; i < G_RECORD_LEN; ++i) {
		if ('0' <= *s && *s <= '9')
			*g_record = (*s - '0') << 4;
		else if ('A' <= *s && *s <= 'F')
			*g_record = (*s - 'A' + 0xA) << 4;
		else
			return -1;
		++s;
		if ('0' <= *s && *s <= '9')
			*g_record++ |= *s - '0';
		else if ('A' <= *s && *s <= 'F')
			*g_record++ |= *s - 'A' + 0xA;
		else
			return -1;
		++s;
	}
#if 0
	if (*s++ != '\r')
		return -1;
#endif
	if (*s++ != '\n')
		return -1;
	if (*s)
		return -1;
	return 0;
}

int main(int argc, char *argv[])
{

	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *file = 0;
	if (strcmp(argv[1], "-")) {
		file = fopen(argv[1], "r");
		if (!file)
			goto error;
	} else {
		file = stdin;
	}

	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);
	HMAC_Init_ex(&ctx, key, key_len, EVP_sha256(), 0);
	int g_records = 0;
	unsigned char g_record[G_RECORD_LEN];
	char line[1024];
	while (fgets(line, sizeof line, file)) {
		if (line[0] == 'G') {
			if (g_records++ || parse_g_record(line, g_record))
				goto error;
		} else {
			HMAC_Update(&ctx, line, strlen(line));
		}
	}
	if (!feof(file))
		goto error;

	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned md_len;
	HMAC_Final(&ctx, md, &md_len);
	if (md_len != G_RECORD_LEN || memcmp(md, g_record, G_RECORD_LEN))
		goto error;

	printf("Valid\n");
	return EXIT_SUCCESS;

error:
	printf("Invalid\n");
	return EXIT_FAILURE;

}

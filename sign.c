#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>

#include "key.c"

int main(int argc, char *argv[])
{
	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);
	HMAC_Init_ex(&ctx, key, key_len, EVP_sha256(), 0);

	char line[1024];
	while (fgets(line, sizeof line, stdin)) {
		fputs(line, stdout);
		if (line[0] == 'H' && (line[1] == 'O' || line[1] == 'P')) {
			0;
		} else if (line[0] == 'L' && strncmp(line, "LXTP", 4)) {
			0;
		} else {
			HMAC_Update(&ctx, (unsigned char *) line, strlen(line));
		}
	}

	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned md_len;
	HMAC_Final(&ctx, md, &md_len);

	printf("G");
	for (unsigned i = 0; i < md_len; ++i)
		printf("%02X", md[i]);
	printf("\n");

	return EXIT_SUCCESS;
}

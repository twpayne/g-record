#include <stdio.h>
#include <stdlib.h>

#include <openssl/hmac.h>

#include "key.c"

int main(int argc, char *argv[])
{
	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);
	HMAC_Init_ex(&ctx, key, key_len, EVP_sha1(), 0);

	size_t n;
	char buffer[1024];
	while ((n = fread(buffer, 1, sizeof buffer, stdin))) {
		fwrite(buffer, n, 1, stdout);
		HMAC_Update(&ctx, buffer, n);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/hmac.h>

#define VERSION "0.1"

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

	time_t t;
	time(&t);
	struct tm tm;
	gmtime_r(&t, &tm);
	char timestamp[32];
	strftime(timestamp, sizeof timestamp, "%Y-%m-%d-%H%M%S", &tm);
	snprintf(line, sizeof line, "LXTPSGN g-record %s %s\n", VERSION, timestamp);
	fputs(line, stdout);
	HMAC_Update(&ctx, (unsigned char *) line, strlen(line));

	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned md_len;
	HMAC_Final(&ctx, md, &md_len);

	printf("G");
	for (unsigned i = 0; i < md_len; ++i)
		printf("%02X", md[i]);
	printf("\n");

	return EXIT_SUCCESS;
}

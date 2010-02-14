#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *file = fopen(argv[1], "w");
	if (!file) {
		fprintf(stderr, "Error: fopen: %s: %s\n", argv[1], strerror(errno));
		return EXIT_FAILURE;
	}
	if (fchmod(fileno(file), S_IRUSR | S_IWUSR)) {
		fprintf(stderr, "Error: fchmod: %s: %s\n", argv[1], strerror(errno));
		return EXIT_FAILURE;
	}

	unsigned char key[EVP_MAX_KEY_LENGTH];
	RAND_bytes(key, sizeof key);

	fprintf(file, "const unsigned char key[] = {");
	for (int i = 0; i < sizeof key; ++i)
		fprintf(file, "%s0x%02x", i ? ", " : "", key[i]);
	fprintf(file, "};\n");
	fprintf(file, "const unsigned key_len = sizeof key;\n");

	if (fclose(file))
		fprintf(stderr, "Warning: fclose: %s: %s\n", argv[1], strerror(errno));

	return EXIT_SUCCESS;
}

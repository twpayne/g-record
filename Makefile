CFLAGS=--std=c99 -O3 -fomit-frame-pointer -D_GNU_SOURCE -Wall
LIBS=/usr/lib/libcrypto.a

.PHONY: all
all: sign-xtp vali-xtp

.PHONY: clean
clean:
	rm -f genkey sign-xtp vali-xtp

sign-xtp: sign-xtp.c key.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

vali-xtp: vali-xtp.c key.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

.PRECIOUS: key.c
key.c: genkey
	[ -f $@ ] || ./genkey $@

genkey: genkey.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

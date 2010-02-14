CFLAGS=--std=c99 -O3 -fomit-frame-pointer -D_GNU_SOURCE -Wall
LIBS=/usr/lib/libcrypto.a

.PHONY: all
all: sign vali

.PHONY: clean
clean:
	rm -f genkey sign vali

sign: sign.c key.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

vali: vali.c key.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

.PRECIOUS: key.c
key.c: genkey
	[ -f $@ ] || ./genkey $@

genkey: genkey.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

G-record
========

IGC G-record signing proof of concept
Copyright &copy; Tom Payne 2010


Quick start
===========

Dependencies
------------

 * OpenSSL development files (package `libssl-dev` on Ubuntu)


Building
--------

Run `make`.


Usage
-----

### `genkey`

`genkey filename` generates a random signing key in `filename`.  It is automatically compiled and invoked during the build process.

### `sign`

`sign` copies its standard input to its standard output and appends a one line G-record.  The G record is one line hexadecimal encoding of the SHA-1 HMAC of the data read.

### `vali`

`vali` reads a single file, calculates its SHA-1 HMAC and compares this to the G-record found at the end.  If the file validates successfully then return code is 0 (success), otherwise it is 1 (failure).


Example
-------

 1. Build the software:
        $ make
 2. Create a file to sign:
        $ $EDITOR example.txt
 3. Sign it with `sign`:
        $ ./sign < example.txt > example.txt.g
 4. Verify the signature with `vali':
        $ ./vali example.txt.g && echo Valid || echo Invalid
        Valid
 5. Modify the signed file:
        $ $EDITOR example.txt.g
 6. Check that the signature is no longer valid:
        $ ./vali example.txt.g && echo Valid || echo Invalid
        Invalid

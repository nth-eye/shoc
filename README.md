# SHA-cpp

## Descryption

This is a header-only templated implementation of SHA-1 and SHA-2 family algorithms in C++11.
Library supports all standard algorithms versions which are mentioned in [FIPS 180-4][1]: 
SHA-1, SHA-224, SHA-256, SHA-384, SHA-512, SHA-512/224 and SHA-512/256. It's intended for simple
and convenient usage within any project, even in embedded devices. There is no standard library 
dependencies except for `cstdint` and `cstddef` for `size_t` and fixed-width integers.

## How to use

As in [openssl][2] library, typical `init() -> update() -> final()` API is used. To instantiate
`SHA<>` object with appropriate `SHA_Type` enum. For example `SHA_1`: 

```
using SHA_t = SHA<SHA_1>;

SHA_t sha;

uint8_t digest[SHA_t::HASH_SIZE];

const char *data = "data to be hashed";

sha.init();
sha.update((const uint8_t*) data, strlen(data));
sha.final(digest);

// Do what you want with digest...
// Then another

const uint8_t data_2[] = { 0xef, 0xbe, 0xad, 0xde };

sha.init();
sha.update(data_2, 4);
sha.final(digest);

// Or do it in many update() cycles

const char *data_3 = "data to be hashed";
const uint8_t data_4[] = { 0xef, 0xbe, 0xad, 0xde };

sha.init();
sha.update((const uint8_t*) data_3, strlen(data_3));
sha.update(data_4, 4);
sha.final(digest);

// ...

```

Some tests are written in main.cpp where you can see typical usage.

## TODO

**_Currently no TODO-s_**

[1]: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
[2]: https://github.com/openssl/openssl
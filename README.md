# hash

This is a header-only implementation of different hashing algorithms in C++11 (templates are used). Currently
only SHA and MD family. Library supports all standard SHA-1 and SHA-2 algorithms versions which are mentioned 
in [FIPS 180-4][1]. It's intended for simple and convenient usage within any project, even in embedded devices. 
There is no standard library dependencies except `cstdint`, `cstddef` and `cstring` for `memcpy`.
**Behavior on big-endian platforms is not tested!**

## How to use

As in [openssl][2] library, typical `init() -> update() -> final()` API is used. 

```cpp
using hash_t = SHA_2<SHA_256>;

hash_t hash;

uint8_t digest[hash_t::HASH_SIZE];

const char *data = "data to be hashed";

hash.init();
hash.update(data, strlen(data));
hash.final(digest);

// Do what you want with digest...
// Then another

const uint8_t data_2[] = { 0xef, 0xbe, 0xad, 0xde };

hash.init();
hash.update(data_2, 4);
hash.final(digest);

// Or do it in many update() cycles

const char *data_3 = "data to be hashed";
const uint8_t data_4[] = { 0xef, 0xbe, 0xad, 0xde };

hash.init();
hash.update(data_3, strlen(data_3));
hash.update(data_4, 4);
hash.final(digest);

// ...

```

Some tests are written in main.cpp where you can also see typical usage.

## TODO

**_Nothing_**

[1]: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
[2]: https://github.com/openssl/openssl

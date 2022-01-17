# creep

This is a header-only C++17 implementation (templates are used) of different cryptographic algorithms: hashing, message 
authentication codes, one-time passwords, etc. It's intended for simple and convenient usage within any project, even in 
embedded devices. There is no standard library dependencies except `cstdint`, `cstddef` and `cstring`.
**Behavior on big-endian platforms is not tested!**

## How to use

### Hash example

As in [openssl][1] library, typical `init() -> update() -> final()` API is used for hashing objects. 

```cpp
using hash_t = SHA2<SHA_256>;

hash_t hash;
uint8_t digest[hash_t::HASH_SIZE];
const char *data_1 = "data to be hashed";
const uint8_t data_2[] = { 0xef, 0xbe, 0xad, 0xde };

hash.init();
hash.update(data_1, strlen(data_1));
hash.update(data_2, sizeof(data_2));
hash.final(digest);
```

### HMAC example

Example with SHA hashing object. To use your own hash-fn, write class with appropriate API and static members as described above.

```cpp
SHA1 sha;
uint8_t digest[SHA1::HASH_SIZE];
const char *key = "key";
const char *msg = "The quick brown fox jumps over the lazy dog";

hmac(sha, msg, strlen(msg), key, strlen(key), digest);
```

### HOTP example

```cpp
SHA1 sha;
const char *key = "12345678901234567890";
uint64_t counter = 0;
uint32_t code = hotp(sha, key, strlen(key), counter++, 6);
```

Some tests are written in main.cpp where you can also see typical usage.

## TODO

**_Nothing_**

[1]: https://github.com/openssl/openssl

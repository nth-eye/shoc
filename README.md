# shoc

Software-Header-Only-Crypto library in C++.

## TODO

- [ ] source
    - [x] unified api for consumers (md2, sha1, hmac, etc)
    - [ ] safer interface (span_i/o)
    - [ ] byteswap
    - [ ] put-, get- little_endian/big_endian
    - [ ] assertions
    - [ ] xoodoo family algorithms
    - [x] cipher
    - [x] ecc
    - [ ] hash
        - [x] gimli
        - [x] md2
        - [x] md4
        - [x] md5
        - [x] sha1
        - [x] sha2
        - [ ] sha3
    - [ ] kdf
        - [ ] hkdf
    - [ ] mac
        - [ ] cbc_mac
        - [ ] gmac
        - [ ] hmac
- [ ] tests
    - [x] cipher
    - [x] ecc
    - [ ] hash
        - [ ] constexpr
        - [x] gimli
        - [x] md2
        - [x] md4
        - [x] md5
        - [x] sha1
        - [x] sha2
        - [ ] sha3
    - [ ] kdf
        - [ ] hkdf
    - [ ]
- [ ] readme
    - [ ] intro
    - [ ] guide
    - [ ] examples
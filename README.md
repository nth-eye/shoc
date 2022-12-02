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
    - [x] hash
    - [x] kdf
    - [ ] mac
        - [x] cbc_mac
        - [ ] gmac
        - [x] hmac
    - [ ] mode
        - [x] ecb
        - [x] cbc
        - [x] cfb
        - [x] ofb
        - [x] ctr
        - [x] ccm
        - [ ] gcm
    - [x] otp
- [ ] tests
    - [x] cipher
    - [x] ecc
    - [x] hash
    - [ ] kdf
        - [ ] hkdf
            - [ ] constexpr
    - [ ] mac
        - [ ] cbc_mac
        - [ ] gmac
        - [ ] hmac
            - [ ] constexpr
    - [ ] mode
        - [x] ecb
        - [x] cbc
        - [x] cfb
        - [x] ofb
        - [x] ctr
        - [x] ccm
        - [ ] gcm
    - [x] otp
- [ ] readme
    - [ ] intro
    - [ ] guide
    - [ ] examples
#include <gtest/gtest.h>
#include "shoc/ecc/crc.h"

using namespace shoc;

static constexpr auto test_data = "123456789";
static constexpr auto test_size = 9;

template<class T, T poly, T init, T xorout, bool refin, bool refout>
static constexpr auto check_fast_and_slow(T exp)
{
    EXPECT_EQ(exp, (crc_slow<T, poly, init, xorout, refin, refout>(test_data, test_size)));
    EXPECT_EQ(exp, (crc_fast<T, poly, init, xorout, refin, refout>(test_data, test_size)));
}

template<uint8_t poly, uint8_t init, uint8_t xorout, bool refin, bool refout>
static constexpr auto check_fast_and_slow_8(uint8_t exp)
{
    return check_fast_and_slow<uint8_t, poly, init, xorout, refin, refout>(exp);
}

template<uint16_t poly, uint16_t init, uint16_t xorout, bool refin, bool refout>
static constexpr auto check_fast_and_slow_16(uint16_t exp)
{
    return check_fast_and_slow<uint16_t, poly, init, xorout, refin, refout>(exp);
}

template<uint32_t poly, uint32_t init, uint32_t xorout, bool refin, bool refout>
static constexpr auto check_fast_and_slow_32(uint32_t exp)
{
    return check_fast_and_slow<uint32_t, poly, init, xorout, refin, refout>(exp);
}

template<uint64_t poly, uint64_t init, uint64_t xorout, bool refin, bool refout>
static constexpr auto check_fast_and_slow_64(uint64_t exp)
{
    return check_fast_and_slow<uint64_t, poly, init, xorout, refin, refout>(exp);
}


TEST(Ecc, Crc8) 
{
    check_fast_and_slow_8<0x2f, 0xff, 0xff, 0, 0>(0xdf); // CRC-8/AUTOSAR
    check_fast_and_slow_8<0xa7, 0x00, 0x00, 1, 1>(0x26); // CRC-8/BLUETOOTH
    check_fast_and_slow_8<0x9b, 0xff, 0x00, 0, 0>(0xda); // CRC-8/CDMA2000
    check_fast_and_slow_8<0x39, 0x00, 0x00, 1, 1>(0x15); // CRC-8/DARC
    check_fast_and_slow_8<0xd5, 0x00, 0x00, 0, 0>(0xbc); // CRC-8/DVB-S2
    check_fast_and_slow_8<0x1d, 0x00, 0x00, 0, 0>(0x37); // CRC-8/GSM-A
    check_fast_and_slow_8<0x49, 0x00, 0xff, 0, 0>(0x94); // CRC-8/GSM-B
    check_fast_and_slow_8<0x1d, 0xff, 0x00, 0, 0>(0xb4); // CRC-8/HITAG
    check_fast_and_slow_8<0x07, 0x00, 0x55, 0, 0>(0xa1); // CRC-8/I-432-1
    check_fast_and_slow_8<0x1d, 0xfd, 0x00, 0, 0>(0x7e); // CRC-8/I-CODE
    check_fast_and_slow_8<0x9b, 0x00, 0x00, 0, 0>(0xea); // CRC-8/LTE
    check_fast_and_slow_8<0x31, 0x00, 0x00, 1, 1>(0xa1); // CRC-8/MAXIM-DOW
    check_fast_and_slow_8<0x1d, 0xc7, 0x00, 0, 0>(0x99); // CRC-8/MIFARE-MAD
    check_fast_and_slow_8<0x31, 0xff, 0x00, 0, 0>(0xf7); // CRC-8/NRSC-5
    check_fast_and_slow_8<0x2f, 0x00, 0x00, 0, 0>(0x3e); // CRC-8/OPENSAFETY
    check_fast_and_slow_8<0x07, 0xff, 0x00, 1, 1>(0xd0); // CRC-8/ROHC
    check_fast_and_slow_8<0x1d, 0xff, 0xff, 0, 0>(0x4b); // CRC-8/SAE-J1850
    check_fast_and_slow_8<0x07, 0x00, 0x00, 0, 0>(0xf4); // CRC-8/SMBUS
    check_fast_and_slow_8<0x1d, 0xff, 0x00, 1, 1>(0x97); // CRC-8/TECH-3250
    check_fast_and_slow_8<0x9b, 0x00, 0x00, 1, 1>(0x25); // CRC-8/WCDMA
}

TEST(Ecc, Crc16) 
{
    check_fast_and_slow_16<0x8005, 0x0000, 0x0000, 1, 1>(0xbb3d); // CRC-16/ARC
    check_fast_and_slow_16<0xc867, 0xffff, 0x0000, 0, 0>(0x4c06); // CRC-16/CDMA2000
    check_fast_and_slow_16<0x8005, 0xffff, 0x0000, 0, 0>(0xaee7); // CRC-16/CMS
    check_fast_and_slow_16<0x8005, 0x800d, 0x0000, 0, 0>(0x9ecf); // CRC-16/DDS-110
    check_fast_and_slow_16<0x0589, 0x0000, 0x0001, 0, 0>(0x007e); // CRC-16/DECT-R
    check_fast_and_slow_16<0x0589, 0x0000, 0x0000, 0, 0>(0x007f); // CRC-16/DECT-X
    check_fast_and_slow_16<0x3d65, 0x0000, 0xffff, 1, 1>(0xea82); // CRC-16/DNP
    check_fast_and_slow_16<0x3d65, 0x0000, 0xffff, 0, 0>(0xc2b7); // CRC-16/EN-13757
    check_fast_and_slow_16<0x1021, 0xffff, 0xffff, 0, 0>(0xd64e); // CRC-16/GENIBUS
    check_fast_and_slow_16<0x1021, 0x0000, 0xffff, 0, 0>(0xce3c); // CRC-16/GSM
    check_fast_and_slow_16<0x1021, 0xffff, 0x0000, 0, 0>(0x29b1); // CRC-16/IBM-3740
    check_fast_and_slow_16<0x1021, 0xffff, 0xffff, 1, 1>(0x906e); // CRC-16/IBM-SDLC
    check_fast_and_slow_16<0x1021, 0xc6c6, 0x0000, 1, 1>(0xbf05); // CRC-16/ISO-IEC-14443-3-A
    check_fast_and_slow_16<0x1021, 0x0000, 0x0000, 1, 1>(0x2189); // CRC-16/KERMIT
    check_fast_and_slow_16<0x6f63, 0x0000, 0x0000, 0, 0>(0xbdf4); // CRC-16/LJ1200
    check_fast_and_slow_16<0x5935, 0xffff, 0x0000, 0, 0>(0x772b); // CRC-16/M17
    check_fast_and_slow_16<0x8005, 0x0000, 0xffff, 1, 1>(0x44c2); // CRC-16/MAXIM-DOW
    check_fast_and_slow_16<0x1021, 0xffff, 0x0000, 1, 1>(0x6f91); // CRC-16/MCRF4XX
    check_fast_and_slow_16<0x8005, 0xffff, 0x0000, 1, 1>(0x4b37); // CRC-16/MODBUS
    check_fast_and_slow_16<0x080b, 0xffff, 0x0000, 1, 1>(0xa066); // CRC-16/NRSC-5
    check_fast_and_slow_16<0x5935, 0x0000, 0x0000, 0, 0>(0x5d38); // CRC-16/OPENSAFETY-A
    check_fast_and_slow_16<0x755b, 0x0000, 0x0000, 0, 0>(0x20fe); // CRC-16/OPENSAFETY-B
    check_fast_and_slow_16<0x1dcf, 0xffff, 0xffff, 0, 0>(0xa819); // CRC-16/PROFIBUS
    check_fast_and_slow_16<0x1021, 0xb2aa, 0x0000, 1, 1>(0x63d0); // CRC-16/RIELLO
    check_fast_and_slow_16<0x1021, 0x1d0f, 0x0000, 0, 0>(0xe5cc); // CRC-16/SPI-FUJITSU
    check_fast_and_slow_16<0x8bb7, 0x0000, 0x0000, 0, 0>(0xd0db); // CRC-16/T10-DIF
    check_fast_and_slow_16<0xa097, 0x0000, 0x0000, 0, 0>(0x0fb3); // CRC-16/TELEDISK
    check_fast_and_slow_16<0x1021, 0x89ec, 0x0000, 1, 1>(0x26b1); // CRC-16/TMS37157
    check_fast_and_slow_16<0x8005, 0x0000, 0x0000, 0, 0>(0xfee8); // CRC-16/UMTS
    check_fast_and_slow_16<0x8005, 0xffff, 0xffff, 1, 1>(0xb4c8); // CRC-16/USB
    check_fast_and_slow_16<0x1021, 0x0000, 0x0000, 0, 0>(0x31c3); // CRC-16/XMODEM
}

TEST(Ecc, Crc32) 
{
    check_fast_and_slow_32<0x814141ab, 0x00000000, 0x00000000, 0, 0>(0x3010bf7f); // CRC-32/AIXM
    check_fast_and_slow_32<0xf4acfb13, 0xffffffff, 0xffffffff, 1, 1>(0x1697d06a); // CRC-32/AUTOSAR
    check_fast_and_slow_32<0xa833982b, 0xffffffff, 0xffffffff, 1, 1>(0x87315576); // CRC-32/BASE91-D
    check_fast_and_slow_32<0x04c11db7, 0xffffffff, 0xffffffff, 0, 0>(0xfc891918); // CRC-32/BZIP2
    check_fast_and_slow_32<0x8001801b, 0x00000000, 0x00000000, 1, 1>(0x6ec2edc4); // CRC-32/CD-ROM-EDC
    check_fast_and_slow_32<0x04c11db7, 0x00000000, 0xffffffff, 0, 0>(0x765e7680); // CRC-32/CKSUM
    check_fast_and_slow_32<0x1edc6f41, 0xffffffff, 0xffffffff, 1, 1>(0xe3069283); // CRC-32/ISCSI
    check_fast_and_slow_32<0x04c11db7, 0xffffffff, 0xffffffff, 1, 1>(0xcbf43926); // CRC-32/ISO-HDLC
    check_fast_and_slow_32<0x04c11db7, 0xffffffff, 0x00000000, 1, 1>(0x340bc6d9); // CRC-32/JAMCRC
    check_fast_and_slow_32<0x741b8cd7, 0xffffffff, 0x00000000, 1, 1>(0xd2c22f51); // CRC-32/MEF
    check_fast_and_slow_32<0x04c11db7, 0xffffffff, 0x00000000, 0, 0>(0x0376e6e7); // CRC-32/MPEG-2
    check_fast_and_slow_32<0x000000af, 0x00000000, 0x00000000, 0, 0>(0xbd0be338); // CRC-32/XFER
}

TEST(Ecc, Crc64) 
{
    check_fast_and_slow_64<0x42f0e1eba9ea3693, 0x0000000000000000, 0x0000000000000000, 0, 0>(0x6c40df5f0b497347); // CRC-64/ECMA-182
    check_fast_and_slow_64<0x000000000000001b, 0xffffffffffffffff, 0xffffffffffffffff, 1, 1>(0xb90956c775a41001); // CRC-64/GO-ISO
    check_fast_and_slow_64<0x259c84cba6426349, 0xffffffffffffffff, 0x0000000000000000, 1, 1>(0x75d4b74f024eceea); // CRC-64/MS
    check_fast_and_slow_64<0x42f0e1eba9ea3693, 0xffffffffffffffff, 0xffffffffffffffff, 0, 0>(0x62ec59e3f1a4f00a); // CRC-64/WE
    check_fast_and_slow_64<0x42f0e1eba9ea3693, 0xffffffffffffffff, 0xffffffffffffffff, 1, 1>(0x995dc9bbdf1939fa); // CRC-64/XZ
}

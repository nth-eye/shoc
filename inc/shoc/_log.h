#ifndef SHOC_LOG_H
#define SHOC_LOG_H

#include "shoc/_util.h"
#include <cstdio>

namespace shoc {

/**
 * @brief Print bytes nicely as hex and relevant ASCII from LSB.
 * 
 * @param data Data to log
 * @param len Number of bytes
 * @param str Null-terminated information string
 */
inline void log_hex(const void *data, size_t len, const char *str)
{
    printf("[%8lx]  \"%s\"\n", len, str);

    if (!data || !len)
        return;
    auto p = static_cast<const byte*>(data);

    for (size_t i = 0; i < len; ++i) {

        if (!(i & 15))
            printf("[%08llx]  ", (long long unsigned) p + i);
        printf("%02x ", p[i]);
        
        if ((i & 7) == 7)
            putchar(' ');

        if ((i & 15) == 15) {
            printf("|");
            for (int j = 15; j >= 0; --j) {
                char c = p[i - j];
                printf("%c", isprint(c) ? c : '.');
            }
            printf("|\n");
        }
    }
    int rem = len - ((len >> 4) << 4);
    if (rem) {
        printf("%*c |", (16 - rem) * 3 + ((~rem & 8) >> 3), ' ');
        for (int j = rem; j; --j) {
            char c = p[len - j];
            printf("%c", isprint(c) ? c : '.');
        }
        for (int j = 0; j < 16 - rem; ++j)
            putchar('.');
        printf("|\n");
    }
}

/**
 * @brief Print bits nicely from offset position with MSB at left and relevant ASCII.
 * 
 * @param data Memory with bits to log
 * @param len Number of bits
 * @param pos Bit offset in initial byte
 */
inline void log_bits(const void *data, size_t len, size_t pos)
{
    printf("[%08llx]: %lu bits with %lu offset \n", (long long unsigned) data, len, pos);

    if (!data || !len)
        return;
    auto p = static_cast<const byte*>(data);
    auto offset = pos & 7;
    auto rem = len & 7;

    byte line[8];

    for (size_t i = 0; i < len; ++i) {

        if (!(i & 63))
            printf("[%08llx : %lu]  ", (long long unsigned) data + (pos >> 3), offset);

        if (get_arr_bit(p, pos++))
            set_arr_bit(line, i & 63);
        else
            clr_arr_bit(line, i & 63);
            
        if (i == len - 1 && rem) {
            byte c = line[(i & 63) >> 3];
            for (int j = rem - 1; j >= 0; --j)
                putchar(get_arr_bit(&c, j) + '0');
        }

        if ((i & 7) == 7) {
            byte c = line[(i & 63) >> 3];
            for (int j = 7; j >= 0; --j)
                putchar(get_arr_bit(&c, j) + '0');
            printf(" ");
        }

        if ((i & 63) == 63) {
            printf(" |");
            for (int j = 0; j < 8; ++j) {
                char c = line[j];
                if (isprint(c))
                    printf("%c", c);
                else
                    printf(".");
            }
            printf("|\n");
        }
    }

    int rem_bits = len - ((len >> 6) << 6);
    int rem_bytes = rem_bits >> 3;
    int fill_bits = 64 - rem_bits;
    int fill_bytes = fill_bits >> 3;

    if (rem_bits) {
        printf("%*c |", int(fill_bits + bytes_in_bits(fill_bits)), ' ');
        for (int j = 0; j < rem_bytes; ++j) {
            char c = line[j];
            if (isprint(c))
                printf("%c", c);
            else
                printf(".");
        }
        if (rem)
            printf(".");
        for (int j = 0; j < fill_bytes; ++j)
            putchar('.');
        printf("|\n");
    }
}

}

#endif
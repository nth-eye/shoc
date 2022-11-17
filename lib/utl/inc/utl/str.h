#ifndef UTL_STR_H
#define UTL_STR_H

#include "utl/math.h"
#include "utl/vector.h"
#include <string_view>
#include <algorithm>

namespace utl {
namespace imp {
inline constexpr char digits_upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
inline constexpr char digits_lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
}

/**
 * @brief Convert integer to hexadecimal character.
 * 
 * @tparam L Lowercase
 * @param bin Byte value
 * @return Hexadecimal character 
 */
template<bool L = true>
constexpr char bin_to_char(uint8_t bin)
{
    return L ?
        imp::digits_lower[bin & 0xf]:
        imp::digits_upper[bin & 0xf];
}

/**
 * @brief C style function, but with constexpr support.
 * 
 * @param str Pointer to null-terminated string
 * @return String length
 */
constexpr size_t str_len(const char* str)
{
    size_t len = 0;
    while (*str++) ++len;
    return len;
}

/**
 * @brief Convert string to double, malformed input returns 0.
 * 
 * @param str Input string
 * @param len String length
 * @return Result 
 */
constexpr double str_to_dbl(const char* str, size_t len)
{
    if (!str || !len)
        return 0;

    const auto end = str + len;
    double neg = 1;
    double res = 0;
    int dot = 0;

    switch (*str) {
        case '-': neg = -1; [[fallthrough]];
        case '+': str += 1;
        break;
    }
    for (; str != end; ++str) {
        if (*str >= '0' && *str <= '9') {
            res = res * 10.0 + (*str - '0');
        } else {
            if (*str != '.' || dot)
                return 0;
            dot = end - str - 1;
        }
    }
    return res * neg / ipow(10, dot);
}

/**
 * @brief Convert string view to double, malformed input returns 0.
 * 
 * @param sv Input string view
 * @return Result
 */
constexpr double str_to_dbl(std::string_view sv)
{
    return str_to_dbl(sv.data(), sv.size());
}

/**
 * @brief Convert string to integer, malformed input returns 0.
 * 
 * @param str Input string
 * @param len String length
 * @return Result 
 */
constexpr long str_to_int(const char* str, size_t len)
{
    if (!str || !len)
        return 0;

    const auto end = str + len;
    long neg = 1;
    long res = 0;

    switch (*str) {
        case '-': neg = -1; [[fallthrough]];
        case '+': str += 1;
        break;
    }
    for (; str != end; ++str) {
        if (*str >= '0' && *str <= '9')
            res = res * 10 + (*str - '0');
        else
            return 0;
    }
    return res * neg;
}

/**
 * @brief Convert string view to integer, malformed input returns 0.
 * 
 * @param sv Input string view
 * @return Result
 */
constexpr long str_to_int(std::string_view sv)
{
    return str_to_int(sv.data(), sv.size());
}

/**
 * @brief Convert string with hexadecimal characters ('0'...'F') to array of bytes.
 * All non-hex chars will be mapped as 0. String with odd length will be interpeted
 * as with prepended '0', e.g. "fff" --> "0fff". Works with both upper and lower cases.
 * If output array is too small, as much as possible will be processed.
 * 
 * @param str Input string
 * @param str_len Input string length
 * @param bin Output array
 * @param max_bin_len Output array max size 
 * @return Length of resulting array, 0 if failed
 */
constexpr size_t str_to_bin(const char* str, size_t str_len, uint8_t* bin, size_t max_bin_len)
{
    if (!str || !bin)
        return 0;

    constexpr uint8_t map[256] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'()*+,-./
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0123456789:;<=>?
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // @ABCDEFGHIJKLMNO
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVWXYZ[\]^_
        0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // `abcdefghijklmno
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvwxyz{|}~.
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
    };
    size_t i = 0; 
    size_t j = 0;
    size_t bin_len = (str_len + 1) >> 1; // The output array size is half the str length (rounded up)

    if (bin_len > max_bin_len) {
        bin_len = max_bin_len;
        str_len = max_bin_len << 1; // return 0;
    }
    if (str_len & 1) {
        int idx = str[0];
        bin[0] = map[idx];
        i = j = 1;
    }
    for (; i < str_len; i += 2, j++) {
        int i0 = str[i];
        int i1 = str[i + 1];
        bin[j] = (map[i0] << 4) | map[i1];
    }
    return bin_len;
}

/**
 * @brief onvert byte array to hexadecimal string, NOT null-terminated. If input 
 * is too large for output, as much bytes as possible will be processed.
 * 
 * @tparam L Lowercase
 * @param bin Input array
 * @param bin_len Input array length
 * @param str Output string
 * @param max_str_len Output string maximum length
 * @return Resulting string length, 0 if failed
 */
template<bool L = true>
constexpr size_t bin_to_str(const uint8_t* bin, size_t bin_len, char* str, size_t max_str_len)
{
    if (!str || !bin || !bin_len || max_str_len < 2)
        return 0;

    size_t str_len = bin_len << 1;

    if (str_len >= max_str_len) {
        str_len = max_str_len & ~1;
        bin_len = str_len >> 1;
    }
    for (size_t i = 0; i < bin_len; ++i) {
        *str++ = bin_to_char<L>(bin[i] >> 4);
        *str++ = bin_to_char<L>(bin[i] & 0xf);
    }
    return str_len;
}

/**
 * @brief Convert byte array to hexadecimal null-terminated C string. If input 
 * is too large for output, as much bytes as possible will be processed.
 * 
 * @tparam L Lowercase
 * @param bin Input array
 * @param bin_len Input array length
 * @param str Output string
 * @param max_str_len Output string maximum length, including 0-terminator
 * @return Resulting string length, 0 if failed
 */
template<bool L = true>
constexpr size_t bin_to_cstr(const uint8_t* bin, size_t bin_len, char* str, size_t max_str_len)
{
    if (!str || !bin || !bin_len || max_str_len < 3)
        return 0;

    size_t str_len = bin_len << 1;

    if (str_len >= max_str_len) {
        str_len = (max_str_len - 1) & ~1;
        bin_len = str_len >> 1;
    }
    for (size_t i = 0; i < bin_len; ++i) {
        *str++ = bin_to_char<L>(bin[i] >> 4);
        *str++ = bin_to_char<L>(bin[i] & 0xf);
    }
    *str = 0;

    return str_len;
}

/**
 * @brief Split string into tokens by given delimiter. Returns 
 * utl::vector filled with std::string_view tokens.
 * 
 * @tparam N Maximum number of tokens
 * @param str String to split
 * @param del All delimiter characters
 * @return Static vector with split string
 */
template<size_t N>
constexpr auto split(std::string_view str, std::string_view del)
{
    vector<std::string_view, N> tokens;

    size_t head = 0; 
    size_t tail = 0; 
 
    for (size_t i = 0; i < N && tail != std::string_view::npos; ++i) { 
        tail = str.find_first_of(del, head); 
        tokens.push_back(str.substr(head, tail - head)); 
        head = tail + 1; 
    } 
    return tokens;
}

/**
 * @brief String literal helper to use it as template parameter.
 * 
 * @tparam N Deduced string literal length
 */
template<size_t N>
struct string_literal {
    constexpr string_literal() = default;
    constexpr string_literal(const char (&str)[N])          { std::copy_n(str, N, buf); }
    constexpr static size_t size()                          { return N - 1; }
    constexpr auto view() noexcept                          { return std::string_view{buf, size()}; }
    constexpr auto view() const noexcept                    { return std::string_view{buf, size()}; }
    constexpr auto data() noexcept                          { return buf; }
    constexpr auto data() const noexcept                    { return buf; }
    constexpr auto begin() noexcept                         { return buf; }
    constexpr auto begin() const noexcept                   { return buf; }
    constexpr auto end() noexcept                           { return buf + size(); }
    constexpr auto end() const noexcept                     { return buf + size(); }
    constexpr auto& operator[](size_t i) noexcept           { return buf[i]; }
    constexpr auto& operator[](size_t i) const noexcept     { return buf[i]; }
    constexpr auto& front() noexcept                        { return buf[0]; }
    constexpr auto& front() const noexcept                  { return buf[0]; }
    constexpr auto& back() noexcept                         { return buf[size() - 1]; }
    constexpr auto& back() const noexcept                   { return buf[size() - 1]; }
    char buf[N];
};

/**
 * @brief Wrapper to convert either integral or floating number 
 * to utl::string_literal at compile time and save it to static 
 * constexpr storage. Can be copied or accessed as const char* 
 * and std::string_view. For integral types P should be from 2 
 * to 36 inclusive. If base > 10 digits can be in lower- or 
 * uppercase depending on U parameter.
 * 
 * @tparam N Number to convert
 * @tparam P Additional parameter: base for integral, precision for floating
 * @tparam U Use uppercase for integral with base > 10
 */
template<arithmetic auto N, int P = std::integral<decltype(N)> ? 10 : 5, bool U = false>
struct to_string {
    static constexpr auto& literal()    { return arr; }
    static constexpr auto cstr()        { return arr.data(); }
    static constexpr auto view()        { return arr.view(); }
private:
    static constexpr auto arr = []() 
    {
        if constexpr (std::integral<decltype(N)>) {

            static_assert(P > 1 && P < sizeof(imp::digits_upper));

            string_literal<ilen(N, P) + 1> arr;

            arr.back() = 0;

            if (N) {
                auto n = N;
                auto p = arr.size();
                if (N < 0) {
                    arr.front() = '-';
                    n = -n;
                }
                for (; n; n /= P) {
                    arr[--p] = U ? imp::digits_upper[n % P]:
                                   imp::digits_lower[n % P];
                }
            } else {
                arr.front() = '0'; 
            }
            return arr;

        } else {

            string_literal<flen(N, P) + 1> arr;

            arr.back() = 0;

            auto n = N;
            auto p = arr.size();

            if (N < 0) {
                arr.front() = '-';
                n = -n;
            }
            auto append = [&](auto n, int iters) {
                if (n) {
                    for (int i = 0; i < iters; ++i, n /= 10)
                        arr[--p] = imp::digits_lower[n % 10];
                } else {
                    arr[--p] = '0';
                }
            };
            long long int whole = n;
            long long int frac = (n - whole) * ipow(10, P);

            append(frac, P);
            arr[--p] = '.';
            append(whole, ilen(whole));

            return arr;
        }
    }();
};

/**
 * @brief Helper to access number converted to string literal.
 * 
 * @tparam N Number to convert
 * @tparam P Additional parameter: base for integral, precision for floating
 * @tparam U Use uppercase for integral with base > 10
 */
template<arithmetic auto N, int P = std::integral<decltype(N)> ? 10 : 5, bool U = false>
inline constexpr auto& to_string_v = to_string<N, P, U>::literal();

/**
 * @brief Wrapper to join string literals at compile time and
 * save them in static constexpr storage as utl::string_literal.
 * Can be copied or accessed as std::string_view and const char*.
 * 
 * @tparam String literals
 */
template <string_literal... Strings>
struct join {
    static constexpr auto& literal()    { return arr; }
    static constexpr auto cstr()        { return arr.data(); }
    static constexpr auto view()        { return arr.view(); }
private:
    static constexpr auto arr = []() 
    {
        string_literal<(Strings.size() + ... + 0) + 1> arr{};
        auto append = [i = 0, &arr] (const auto& s) mutable {
            for (auto c : s) 
                arr[i++] = c;
        };
        (append(Strings), ...);
        return arr;
    }();
};

/**
 * @brief Helper to access joined string literal.
 * 
 * @tparam Strings Literals
 */
template <string_literal... Strings>
inline constexpr auto& join_v = join<Strings...>::literal();

}

#endif
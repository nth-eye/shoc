#ifndef SHOC_MAC_GMAC_H
#define SHOC_MAC_GMAC_H

#include "shoc/mode/gcm.h"

namespace shoc {

/**
 * @brief Authentication-only variant of the GCM. All pointers MUST be 
 * valid when relevant lenth is not 0. Tag length MUST be {4, 8, 12, 13, 14, 15, 16}:
 * https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-38d.pdf
 * 
 * @tparam E Block cipher
 * @param key Key
 * @param iv Initial vector
 * @param iv_len Initial vector length
 * @param aad Additional authenticated data
 * @param aad_len Additional authenticated data length
 * @param tag Output tag
 * @param tag_len Output tag length
 */
template<class E>
inline bool gmac(
    const byte *key,
    const byte *iv,  size_t iv_len,
    const byte *aad, size_t aad_len,
          byte *tag, size_t tag_len)
{
    return gcm_encrypt<E>(key, iv, iv_len, aad, aad_len, tag, tag_len, nullptr, nullptr, 0);
}

}

#endif
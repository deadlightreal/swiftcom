#include "crypto.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

std::vector<uint8_t> utils::crypto::base32_decode(const std::string& input) {
    static int8_t lookup[256];
    static bool initialized = false;

    if (!initialized) {
        for (int i = 0; i < 256; ++i)
            lookup[i] = -1;

        for (int i = 'A'; i <= 'Z'; ++i)
            lookup[i] = i - 'A';

        for (int i = 'a'; i <= 'z'; ++i)
            lookup[i] = i - 'a';

        for (int i = '2'; i <= '7'; ++i)
            lookup[i] = 26 + (i - '2');

        initialized = true;
    }

    std::vector<uint8_t> out;
    uint32_t buffer = 0;
    int bitsLeft = 0;

    for (char c : input) {
        int8_t val = lookup[static_cast<uint8_t>(c)];
        if (val < 0) continue;

        buffer = (buffer << 5) | val;
        bitsLeft += 5;

        if (bitsLeft >= 8) {
            bitsLeft -= 8;
            out.push_back((buffer >> bitsLeft) & 0xFF);
        }
    }

    return out;
}

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
    int bits_left = 0;

    for (char c : input) {
        int8_t val = lookup[static_cast<uint8_t>(c)];
        if (val < 0) continue;

        buffer = (buffer << 5) | val;
        bits_left += 5;

        if (bits_left >= 8) {
            bits_left -= 8;
            out.push_back((buffer >> bits_left) & 0xFF);
        }
    }

    return out;
}

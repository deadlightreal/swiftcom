#include "crypto.hpp"
#include <array>
#include <cstdint>
#include <string>

std::string utils::crypto::base32_encode(const std::array<uint8_t, 8> data) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string out;
    out.reserve((8 * 8 + 4) / 5);

    uint32_t buffer = 0;
    int bitsLeft = 0;

    for (size_t i = 0; i < 8; ++i) {
        buffer = (buffer << 8) | data[i];
        bitsLeft += 8;

        while (bitsLeft >= 5) {
            out.push_back(alphabet[(buffer >> (bitsLeft - 5)) & 0x1F]);
            bitsLeft -= 5;
        }
    }

    if (bitsLeft > 0) {
        out.push_back(alphabet[(buffer << (5 - bitsLeft)) & 0x1F]);
    }

    return out;
}

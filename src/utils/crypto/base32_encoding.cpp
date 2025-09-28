#include "crypto.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::string utils::crypto::base32_encode(const std::vector<uint8_t> data) {
    std::cout << "Data size: " << data.size() << std::endl;

    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string out;
    out.reserve((data.size() * 8 + 4) / 5);

    uint32_t buffer = 0;
    int bitsLeft = 0;

    for (size_t i = 0; i < data.size(); ++i) {
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

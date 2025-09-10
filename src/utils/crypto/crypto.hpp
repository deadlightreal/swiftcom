#pragma once

#include <string>
#include <array>

namespace utils::crypto {
    std::string base32_encode(const std::array<uint8_t, 8> data);
    std::array<uint8_t, 8> base32_decode(const std::string& input);
}

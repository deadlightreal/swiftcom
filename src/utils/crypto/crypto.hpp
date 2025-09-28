#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <vector>

namespace utils::crypto {
    std::string base32_encode(const std::vector<uint8_t> data);
    std::vector<uint8_t> base32_decode(const std::string& input);
}

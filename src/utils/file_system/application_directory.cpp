#include "file_system.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>

std::string utils::file_system::get_application_directory() {

#ifdef __APPLE__
    std::string result = std::string("/Library/Application Support/swift_com");

    return result;
#endif

    return "";
}

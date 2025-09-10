#include "file_system.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

std::string utils::file_system::get_application_directory() {

#ifdef __APPLE__
    const char* home_env = std::getenv("HOME");
    if (home_env == nullptr) {
        perror("Failed to get home env");
        exit(EXIT_FAILURE);
    }
    
    std::string result = std::string(home_env);

    result += "/Library/Application Support/swift_com";

    std::cout << "Location: " << result << std::endl;

    return result;
#endif

    return "";
}

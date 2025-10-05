#pragma once

#include <arpa/inet.h>

namespace utils::net {
    in_addr get_public_ip();
    in_addr get_private_ip();
}

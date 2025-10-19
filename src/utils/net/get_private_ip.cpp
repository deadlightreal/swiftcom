#include "net.hpp"
#include <cstdlib>
#include <iostream>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

static bool is_private_ip(const std::string& ip) {
    return ip.find("192.168.") == 0 ||
           ip.find("10.") == 0 ||
           (ip.find("172.") == 0 && std::stoi(ip.substr(4, ip.find('.', 5) - 4)) >= 16 && std::stoi(ip.substr(4, ip.find('.', 5) - 4)) <= 31);
}

in_addr utils::net::get_private_ip() {
    struct ifaddrs *interfaces = nullptr, *ifa = nullptr;

    if (getifaddrs(&interfaces) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = interfaces; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        if (ifa->ifa_addr->sa_family == AF_INET &&
            (ifa->ifa_flags & IFF_LOOPBACK) == 0) {

            char ip[INET_ADDRSTRLEN];
            in_addr* addr_ptr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;

            inet_ntop(AF_INET, addr_ptr, ip, INET_ADDRSTRLEN);

            std::string ip_str(ip);

            if (is_private_ip(ip_str)) {
                std::cout << "Private IP on interface " << ifa->ifa_name << ": " << ip_str << std::endl;
                
                in_addr addr = *addr_ptr;

                freeifaddrs(interfaces);

                return addr;
            }
        }
    }

    freeifaddrs(interfaces);

    exit(EXIT_FAILURE);
}

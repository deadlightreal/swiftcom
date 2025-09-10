#include "objects.hpp"
#include <arpa/inet.h>

objects::LocalServer::LocalServerParsedData objects::LocalServer::parse() {
    return {
        .id = this->id,
        .ip_address = this->ip_address
    };
}

objects::LocalServer::LocalServer(in_addr ip_address, uint32_t id) : id(id), ip_address(ip_address) {};

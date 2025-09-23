#include "objects.hpp"
#include <arpa/inet.h>
#include <cstdint>

objects::HostedServer::HostedServer(uint32_t id) : id(id) {};

uint32_t objects::HostedServer::GetServerId() {
    return this->id;
}

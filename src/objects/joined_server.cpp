#include "objects.hpp"

objects::JoinedServer::JoinedServer(uint16_t server_id, in_addr server_ip_address) : server_id(server_id), server_ip_address(server_ip_address) {};

uint16_t objects::JoinedServer::GetServerId() {
    return this->server_id;
}

in_addr objects::JoinedServer::GetServerIpAddress() {
    return this->server_ip_address;
}

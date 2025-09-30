#include "objects.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <swift_net.h>
#include "../main.hpp"

objects::HostedServer::HostedServer(uint16_t id) : id(id) {};

uint16_t objects::HostedServer::GetServerId() {
    return this->id;
}

objects::HostedServerStatus objects::HostedServer::GetServerStatus() {
    return this->status;
}

void HandleJoinServerRequest(objects::HostedServer* server, uint8_t* data, SwiftNetPacketServerMetadata* metadata) {
}

void PacketCallback(uint8_t* data, SwiftNetPacketServerMetadata* metadata) {
    const uint16_t server_id = metadata->port_info.destination_port;

    objects::HostedServer* server = wxGetApp().GetLocalStorageDataManager()->GetServerById(server_id);
    if (server == nullptr) {
        return;
    }

    RequestInfo* request_info = (RequestInfo *)data;

    switch (request_info->request_type) {
        case JOIN_SERVER: HandleJoinServerRequest(server, data, metadata); break;
    }
}

void objects::HostedServer::StartServer() {
    this->server = swiftnet_create_server(this->GetServerId());

    swiftnet_server_set_message_handler(this->GetServer(), [](uint8_t* data, SwiftNetPacketServerMetadata* metadata){});
}

SwiftNetServer* objects::HostedServer::GetServer() {
    return this->server;
}

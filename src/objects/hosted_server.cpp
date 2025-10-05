#include "objects.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>
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
    const in_addr ip_address = metadata->sender.sender_address.sin_addr;
    const uint16_t server_id = server->GetServerId();

    int result = wxGetApp().GetLocalStorageDataManager()->GetDatabase()->InsertHostedServerUser(server_id, ip_address);
    if (result != 0) {
        const JoinServerResponse response = {
            .status = FAIL
        };

        SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(sizeof(response));

        swiftnet_server_append_to_packet(server->GetServer(), &response, sizeof(response), &buffer);

        swiftnet_server_send_packet(server->GetServer(), &buffer, metadata->sender);

        swiftnet_server_destroy_packet_buffer(&buffer);
    }

    SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(sizeof(JoinServerResponse));

    const JoinServerResponse response = {
        .status = SUCCESS
    };

    swiftnet_server_append_to_packet(server->GetServer(), &response, sizeof(response), &buffer);

    swiftnet_server_send_packet(server->GetServer(), &buffer, metadata->sender);

    swiftnet_server_destroy_packet_buffer(&buffer);
}

void PacketCallback(uint8_t* data, SwiftNetPacketServerMetadata* metadata) {
    const uint16_t server_id = metadata->port_info.destination_port;

    objects::HostedServer* server = wxGetApp().GetLocalStorageDataManager()->GetServerById(server_id);
    if (server == nullptr) {
        printf("null server\n");
        return;
    }

    RequestInfo* request_info = (RequestInfo *)data;

    switch (request_info->request_type) {
        case JOIN_SERVER: HandleJoinServerRequest(server, data, metadata); break;
    }
}

void objects::HostedServer::StartServer() {
    this->server = swiftnet_create_server(this->GetServerId());

    swiftnet_server_set_message_handler(this->GetServer(), PacketCallback);
}

SwiftNetServer* objects::HostedServer::GetServer() {
    return this->server;
}

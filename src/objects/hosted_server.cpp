#include "objects.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <swift_net.h>
#include <vector>
#include "../main.hpp"

using namespace objects;

static void SerializeChannelMessages(SwiftNetPacketBuffer* buffer, std::vector<Database::ChannelMessageRow>* messages) {
    for (auto &message : *messages) {
        swiftnet_server_append_to_packet(&message.id, sizeof(message.id), buffer);
        swiftnet_server_append_to_packet(&message.sender_id, sizeof(message.id), buffer);
        swiftnet_server_append_to_packet(&message.message_length, sizeof(message.id), buffer);
        swiftnet_server_append_to_packet(&message.message, message.message_length + 1, buffer);
    }
}

static void HandleLoadChannelDataRequest(HostedServer* server, SwiftNetServerPacketData* packet_data) {
    LoadChannelDataRequest* request = (LoadChannelDataRequest*)swiftnet_server_read_packet(packet_data, sizeof(LoadChannelDataRequest));

    Database* database = wxGetApp().GetLocalStorageDataManager()->GetDatabase();

    const uint32_t user_id = database->SelectUserId(packet_data->metadata.sender.sender_address.sin_addr.s_addr, server->GetServerId());

    server->AddConnectedUser((ConnectedUser){.addr_data = packet_data->metadata.sender, .user_id = user_id});

    auto channel_messages = database->SelectChannelMessages(request->channel_id);

    uint32_t bytes_to_allocate = (sizeof(LoadChannelDataResponse) + sizeof(RequestInfo));

    for (auto &message : *channel_messages) {
        bytes_to_allocate += (message.message_length + 1);
        bytes_to_allocate += (sizeof(message.id) + sizeof(message.message_length) + sizeof(message.sender_id));
    }

    const RequestInfo request_info = {
        .request_type = LOAD_CHANNEL_DATA
    };

    const LoadChannelDataResponse response = {
        .status = SUCCESS,
        .channel_messages_len = (uint32_t)channel_messages->size()
    };

    SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(bytes_to_allocate);

    swiftnet_server_append_to_packet(&request_info, sizeof(request_info), &buffer);
    swiftnet_server_append_to_packet(&response, sizeof(response), &buffer);

    SerializeChannelMessages(&buffer, channel_messages);

    swiftnet_server_send_packet(server->GetServer(), &buffer, packet_data->metadata.sender);

    swiftnet_server_destroy_packet_buffer(&buffer);
}

static void HandleJoinServerRequest(HostedServer* server, SwiftNetServerPacketData* packet_data) {
    const in_addr ip_address = packet_data->metadata.sender.sender_address.sin_addr;
    const uint16_t server_id = server->GetServerId();

    int result = wxGetApp().GetLocalStorageDataManager()->GetDatabase()->InsertHostedServerUser(server_id, ip_address);
    if (result != 0) {
        const JoinServerResponse response = {
            .status = FAIL
        };

        SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(sizeof(response));

        swiftnet_server_append_to_packet(&response, sizeof(response), &buffer);

        swiftnet_server_send_packet(server->GetServer(), &buffer, packet_data->metadata.sender);

        swiftnet_server_destroy_packet_buffer(&buffer);
    }

    SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(sizeof(JoinServerResponse));

    const JoinServerResponse response = {
        .status = SUCCESS
    };

    swiftnet_server_append_to_packet(&response, sizeof(response), &buffer);

    swiftnet_server_send_packet(server->GetServer(), &buffer, packet_data->metadata.sender);

    swiftnet_server_destroy_packet_buffer(&buffer);
}

static void HandleLoadServerInformationRequest(HostedServer* server, SwiftNetServerPacketData* packet_data) {
    auto server_chat_channels = wxGetApp().GetLocalStorageDataManager()->GetDatabase()->SelectServerChatChannels(server->GetServerId());

    SwiftNetServer* server_swiftnet = server->GetServer();

    const uint32_t size = server_chat_channels->size();

    const uint32_t bytes_to_alloc = sizeof(LoadServerInformationResponse) + size * sizeof(Database::ServerChatChannelRow);
    
    LoadServerInformationResponse* response = (LoadServerInformationResponse*)malloc(bytes_to_alloc);

    response->server_chat_channels_size = size;
    response->status = SUCCESS;

    for (size_t i = 0; i < size; i++) {
        Database::ServerChatChannelRow* channel = &server_chat_channels->at(i);

        response->server_chat_channels[i] = *channel;
    }

    const RequestInfo request_info = {
        .request_type = LOAD_SERVER_INFORMATION,
    };

    SwiftNetPacketBuffer buffer = swiftnet_server_create_packet_buffer(bytes_to_alloc + sizeof(request_info));

    swiftnet_server_append_to_packet(&request_info, sizeof(request_info), &buffer);
    swiftnet_server_append_to_packet(response, bytes_to_alloc, &buffer);
    swiftnet_server_send_packet(server_swiftnet, &buffer, packet_data->metadata.sender);

    swiftnet_server_destroy_packet_buffer(&buffer);

    delete server_chat_channels;
    
    free(response);
}

static void PacketCallback(SwiftNetServerPacketData*  packet_data) {
    const uint16_t server_id = packet_data->metadata.port_info.destination_port;

    HostedServer* server = wxGetApp().GetLocalStorageDataManager()->GetServerById(server_id);
    if (server == nullptr) {
        printf("null server\n");
        return;
    }

    RequestInfo* request_info = (RequestInfo*)swiftnet_server_read_packet(packet_data, sizeof(RequestInfo));

    switch (request_info->request_type) {
        case JOIN_SERVER: HandleJoinServerRequest(server, packet_data); break;
        case LOAD_SERVER_INFORMATION: HandleLoadServerInformationRequest(server, packet_data);
        case LOAD_CHANNEL_DATA: HandleLoadChannelDataRequest(server, packet_data);
    }
}

HostedServer::HostedServer(uint16_t id) : id(id) {

};

HostedServer::~HostedServer() {

}

void HostedServer::StartServer() {
    this->status = RUNNING;

    this->server = swiftnet_create_server(this->GetServerId());

    swiftnet_server_set_message_handler(this->GetServer(), PacketCallback);

    wxGetApp().GetHomeFrame()->GetHostingPanel()->DrawServers();
}

void HostedServer::StopServer() {
    this->status = STOPPED;

    swiftnet_server_cleanup(this->GetServer());

    this->server = NULL;

    wxGetApp().GetHomeFrame()->GetHostingPanel()->DrawServers();
}

void HostedServer::AddConnectedUser(const ConnectedUser connected_user) {
    this->GetConnectedUsers()->push_back(connected_user);
}

std::vector<ConnectedUser>* HostedServer::GetConnectedUsers() {
    return &this->connected_users;
}

SwiftNetServer* HostedServer::GetServer() {
    return this->server;
}

uint16_t HostedServer::GetServerId() {
    return this->id;
}

HostedServerStatus HostedServer::GetServerStatus() {
    return this->status;
}

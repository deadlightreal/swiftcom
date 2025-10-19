#include "../frames.hpp"
#include "swift_net.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <wx/panel.h>
#include "../../main.hpp"

using namespace frames;

static ChatRoomFrame::ChatPanel* current_chat_panel = nullptr;

static std::vector<objects::Database::ChannelMessageRow>* DeserializeChannelMessages(SwiftNetClientPacketData* packet_data, const uint32_t channel_messages_len) {
    auto result = new std::vector<objects::Database::ChannelMessageRow>();

    for (uint32_t i = 0; i < channel_messages_len; i++) {
        const uint32_t* message_id = (uint32_t*)swiftnet_client_read_packet(packet_data, sizeof(uint32_t));
        const uint32_t* sender_id = (uint32_t*)swiftnet_client_read_packet(packet_data, sizeof(uint32_t));
        const uint32_t* message_length = (uint32_t*)swiftnet_client_read_packet(packet_data, sizeof(uint32_t));
        const char* message = (const char*)swiftnet_client_read_packet(packet_data, *message_length);

        char* message_copy = (char*)malloc(*message_length);
        memcpy(message_copy, message, *message_length);

        result->push_back((objects::Database::ChannelMessageRow){
            .message_length = *message_length,
            .sender_id = *sender_id,
            .id = *message_id,
            .message = message_copy
        });
    }

    return result;
}

static void HandleLoadChannelDataRequest(SwiftNetClientPacketData* packet_data) {
    const LoadChannelDataResponse* response = (LoadChannelDataResponse*)swiftnet_client_read_packet(packet_data, sizeof(LoadChannelDataResponse));

    if (response->status != SUCCESS) {
        // Handle errors
        return;
    }

    auto channel_messages = DeserializeChannelMessages(packet_data, response->channel_messages_len);
}

static void PacketHandler(SwiftNetClientPacketData* packet_data) {
    const RequestInfo* request_info = (RequestInfo*)swiftnet_client_read_packet(packet_data, sizeof(RequestInfo));

    switch(request_info->request_type) {
        case LOAD_CHANNEL_DATA: HandleLoadChannelDataRequest(packet_data); break;
        default: break;
    }
}

ChatRoomFrame::ChatPanel::ChatPanel(const uint32_t channel_id, const uint16_t server_id, wxWindow* parent_window, const in_addr ip_address) : channel_id(channel_id), server_id(server_id), wxPanel(parent_window) {
    current_chat_panel = this;

    this->InitializeConnection(ip_address);
}

ChatRoomFrame::ChatPanel::~ChatPanel() {
    swiftnet_client_cleanup(this->GetClientConnection());

    current_chat_panel = nullptr;
}

void ChatRoomFrame::ChatPanel::InitializeConnection(const in_addr ip_address) {
    const char* ip_address_string = inet_ntoa(ip_address);
    SwiftNetClientConnection* new_connection = swiftnet_create_client(ip_address_string, this->GetServerId());
    if (new_connection == nullptr) {
        fprintf(stderr, "Failed to connect to server\n");
        return;
    }

    swiftnet_client_set_message_handler(new_connection, PacketHandler);
}

void ChatRoomFrame::ChatPanel::RequestChannelData() {
    SwiftNetClientConnection* connection = this->GetClientConnection();

    const RequestInfo request_info = {
        .request_type = LOAD_CHANNEL_DATA
    };

    const LoadChannelDataRequest request = {
        .channel_id = this->GetChannelId()
    };

    SwiftNetPacketBuffer buffer = swiftnet_client_create_packet_buffer(sizeof(request) + sizeof(request_info));

    swiftnet_client_append_to_packet(&request_info, sizeof(request_info), &buffer);
    swiftnet_client_append_to_packet(&request, sizeof(request), &buffer);

    swiftnet_client_send_packet(connection, &buffer);

    swiftnet_client_destroy_packet_buffer(&buffer);
}

SwiftNetClientConnection* ChatRoomFrame::ChatPanel::GetClientConnection() {
    return this->client_connection;
}

uint16_t ChatRoomFrame::ChatPanel::GetServerId() {
    return this->server_id;
}

uint32_t ChatRoomFrame::ChatPanel::GetChannelId() {
    return this->channel_id;
}

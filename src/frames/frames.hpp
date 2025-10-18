#pragma once

#include <cstdint>
#include <wx/panel.h>
#include <wx/wx.h>
#include "../widgets/widgets.hpp"
#include "home_frame/panels/panels.hpp"
#include <vector>
#include "../objects/objects.hpp"
#include <swift_net.h>

namespace frames {
    class HomeFrame : public wxFrame {
    public:
        HomeFrame();
        ~HomeFrame();
        void Frame(wxTimerEvent& event);

        frames::home_frame::panels::HostingPanel* GetHostingPanel();
        frames::home_frame::panels::ServersPanel* GetServersPanel();
    private:
        frames::home_frame::panels::HostingPanel* hosting_panel;
        frames::home_frame::panels::ServersPanel* servers_panel;
        widgets::MenuBar* menu_bar;
        uint8_t current_menu;
        wxTimer timer;
    };

    class ChatRoomFrame : public wxFrame {
    public:
        class ChatChannel {
        public:
            ChatChannel(const uint32_t id, char name[20]);

            uint32_t GetId();
            const char* GetName();
        private:
            uint32_t id;
            char name[20];
        };

        void LoadServerInformation();
        ChatRoomFrame(const in_addr ip_address, const uint16_t server_id);
        ~ChatRoomFrame();

        void DrawChannels();

        uint16_t GetServerId();
        SwiftNetClientConnection* GetConnection();
        std::vector<ChatChannel*>* GetChatChannels();
        void HandleLoadServerInfoResponse(SwiftNetClientPacketData* packet_data);
    private:
        wxPanel* channel_list_panel = nullptr;

        uint16_t server_id;
        SwiftNetClientConnection* client_connection;
        std::vector<ChatChannel*> chat_channels;
    };
}

#include "panels.hpp"
#include <arpa/inet.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <wx/utils.h>
#include <wx/wx.h>
#include "../../../utils/net/net.hpp"
#include "../../../widgets/widgets.hpp"
#include "../../../utils/crypto/crypto.hpp"
#include <swift_net.h>
#include "../../../main.hpp"

using frames::home_frame::panels::ServersPanel;

typedef enum {
    EXISTS,
    NO_RESPONSE,
    FAILED_STATUS
} ServerExistsResponse;

ServerExistsResponse server_exists = NO_RESPONSE;

void PacketCallback(SwiftNetClientPacketData* packet_data) {
    const JoinServerResponse* response = (JoinServerResponse*)swiftnet_client_read_packet(packet_data, sizeof(JoinServerResponse));

    if(response->status == SUCCESS) {
        server_exists = EXISTS;
    } else {
        server_exists = FAILED_STATUS;
    }
}

void ServersPanel::AddServerPopupMenu::RequestServerExistsConfirmation(const char* ip_address, const uint16_t server_id) {
    std::cout << "Server id: " << server_id << std::endl;

    SwiftNetClientConnection* client = swiftnet_create_client(ip_address, server_id);

    swiftnet_client_set_message_handler(client, PacketCallback);

    const RequestInfo request_info = {.request_type = JOIN_SERVER};

    while (server_exists == NO_RESPONSE) {
        SwiftNetPacketBuffer buffer = swiftnet_client_create_packet_buffer(sizeof(request_info));
        
        swiftnet_client_append_to_packet(client, &request_info, sizeof(RequestInfo), &buffer);

        swiftnet_client_send_packet(client, &buffer);

        swiftnet_client_destroy_packet_buffer(&buffer);

        usleep(250000);
    }

    in_addr parsed_ip_address;
    inet_pton(AF_INET, ip_address, &parsed_ip_address);

    switch (server_exists) {
        case EXISTS:
            wxGetApp().GetLocalStorageDataManager()->GetDatabase()->InsertJoinedServer(server_id, parsed_ip_address);

            break;
        case FAILED_STATUS:
            break;
        case NO_RESPONSE:
            break;
    }

    server_exists = NO_RESPONSE;

    swiftnet_client_cleanup(client);
}

void ServersPanel::DrawServers() {
    this->joined_servers_panel->DestroyChildren();

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

    this->joined_servers_panel->SetSizer(v_sizer);

    for (auto &server : wxGetApp().GetLocalStorageDataManager()->GetSavedData().joined_servers) {
        uint16_t server_id = server.GetServerId();
        in_addr server_ip_address = server.GetServerIpAddress();

        const char* server_ip_address_parsed = inet_ntoa(server_ip_address);

        widgets::StyledPanel* server_panel = new widgets::StyledPanel(this->joined_servers_panel);
        server_panel->SetMinSize(wxSize(-1, 30));
        server_panel->SetMaxSize(wxSize(-1, 30));

        widgets::Button* start_server_button = new widgets::Button(server_panel, "Enter Server", [this, &server](wxMouseEvent& event){
            frames::ChatRoomFrame* chat_room_frame = new frames::ChatRoomFrame(server.GetServerIpAddress(), server.GetServerId());
            chat_room_frame->Show(true);

            wxGetApp().AddChatRoomFrame(chat_room_frame);
        });
        start_server_button->SetMinSize(wxSize(-1, 30));
        start_server_button->SetMaxSize(wxSize(-1, 30));

        wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

        std::string joined_server_text_string = std::string(server_ip_address_parsed) + "   " + std::to_string(server_id);

        wxStaticText* joined_server_text = new wxStaticText(server_panel, wxID_ANY, wxString(joined_server_text_string));

        button_sizer->Add(joined_server_text, 4, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
        button_sizer->AddStretchSpacer(1);
        button_sizer->Add(start_server_button, 4, wxALIGN_CENTER_VERTICAL);

        server_panel->SetSizer(button_sizer);

        v_sizer->Add(server_panel, 4, wxTOP | wxBOTTOM | wxEXPAND, 5);
    }

    this->joined_servers_panel->GetParent()->Layout();
    this->joined_servers_panel->GetParent()->Refresh();
    this->joined_servers_panel->GetParent()->SendSizeEvent();
}

ServersPanel::AddServerPopupMenu::AddServerReturnCode ServersPanel::AddServerPopupMenu::AddServer(wxString input) {
    in_addr server_ip_address;
    uint16_t server_id;

    std::vector<uint8_t> decoded_code = utils::crypto::base32_decode(input.ToStdString());

    memcpy(&server_ip_address, decoded_code.data(), sizeof(server_ip_address));
    memcpy(&server_id, decoded_code.data() + sizeof(server_ip_address), sizeof(server_id));

    in_addr public_ip_local = utils::net::get_public_ip();

    if (public_ip_local.s_addr == server_ip_address.s_addr) {
        server_ip_address = utils::net::get_private_ip();
    }

    const char* server_ip_string = inet_ntoa(server_ip_address);

    this->RequestServerExistsConfirmation(server_ip_string, server_id);

    return SUCCESS;
}

ServersPanel::AddServerPopupMenu::AddServerPopupMenu(wxWindow* parent, wxPoint pos) : wxDialog(parent, wxID_ANY, wxT("Add Server"), pos, wxSize(400, 200), wxSTAY_ON_TOP | wxDEFAULT_DIALOG_STYLE) {
    wxTextCtrl* server_code_input = new wxTextCtrl(this, 0, "Server Code");

    widgets::Button* add_server_button = new widgets::Button(this, "Add Server", [this, server_code_input](wxMouseEvent& event) { 
        ServersPanel::AddServerPopupMenu::AddServerReturnCode return_code = ServersPanel::AddServerPopupMenu::AddServer(server_code_input->GetValue());

        switch (return_code) {
            case INVALID_LENGTH:
                break;
            case SUCCESS:
                this->EndModal(wxID_CANCEL);
                break;
        }
    });

    add_server_button->SetMinSize(wxSize(-1, 20));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(server_code_input, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(add_server_button, 0, wxALL | wxEXPAND, 10);
    sizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizerAndFit(sizer);
}

ServersPanel::ServersPanel(wxPanel* parent_panel) : wxPanel(parent_panel) {
    widgets::Button* add_server_button = new widgets::Button(this, "Add Server", [this](wxMouseEvent& event) { this->OpenAddServerPopupMenu(event, this); } );
    add_server_button->SetMinSize(wxSize(-1, 40));
    add_server_button->SetMaxSize(wxSize(-1, 40));

    this->joined_servers_panel = new wxPanel(this);

    wxBoxSizer* main_vert_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* main_sizer_margin = new wxBoxSizer(wxHORIZONTAL);

    main_vert_sizer->Add(this->joined_servers_panel, 4, wxTOP | wxBOTTOM | wxEXPAND, 10);
    main_vert_sizer->Add(add_server_button, 4, wxTOP | wxBOTTOM | wxEXPAND, 10);

    main_sizer_margin->AddStretchSpacer(2);
    main_sizer_margin->Add(main_vert_sizer, 10, wxEXPAND);
    main_sizer_margin->AddStretchSpacer(2);

    this->SetSizer(main_sizer_margin);
}

void ServersPanel::OpenAddServerPopupMenu(wxMouseEvent& event, wxWindow* parent) {
    AddServerPopupMenu* popup_menu = new AddServerPopupMenu(parent->MacGetTopLevelWindow(), wxDefaultPosition);

    popup_menu->CentreOnParent();
    popup_menu->ShowModal();
    popup_menu->Destroy();
}

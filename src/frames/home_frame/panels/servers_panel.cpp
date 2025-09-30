#include "panels.hpp"
#include <arpa/inet.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <wx/wx.h>
#include "../../../widgets/widgets.hpp"
#include "../../../utils/crypto/crypto.hpp"
#include <swift_net.h>
#include "../../../main.hpp"

using frames::home_frame::panels::ServersPanel;

void ServersPanel::AddServerPopupMenu::RequestServerExistsConfirmation(const char* ip_address, const uint16_t server_id) {
    std::cout << "Server id: " << server_id << std::endl;

    SwiftNetClientConnection* client = swiftnet_create_client(ip_address, server_id);

    const RequestInfo request_info = {.request_type = JOIN_SERVER};

    SwiftNetPacketBuffer buffer = swiftnet_client_create_packet_buffer(sizeof(request_info));
    
    swiftnet_client_append_to_packet(client, &request_info, sizeof(RequestInfo), &buffer);

    swiftnet_client_send_packet(client, &buffer);
}

ServersPanel::AddServerPopupMenu::AddServerReturnCode ServersPanel::AddServerPopupMenu::AddServer(wxString input) {
    in_addr server_ip_address;
    uint16_t server_id;

    std::vector<uint8_t> decoded_code = utils::crypto::base32_decode(input.ToStdString());

    memcpy(&server_ip_address, decoded_code.data(), sizeof(server_ip_address));
    memcpy(&server_id, decoded_code.data() + sizeof(server_ip_address), sizeof(server_id));

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
    sizer->Add(add_server_button, 0, wxALIGN_CENTER | wxALL | wxEXPAND | wxEXPAND, 10);
    sizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizerAndFit(sizer);
}

ServersPanel::ServersPanel(wxPanel* parent_panel) : wxPanel(parent_panel) {
    widgets::Button* add_server_button = new widgets::Button(this, "Add Server", [this](wxMouseEvent& event) { this->OpenAddServerPopupMenu(event, this); } );
    add_server_button->SetMinSize(wxSize(-1, 40));

    wxBoxSizer* add_server_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    add_server_button_sizer->AddStretchSpacer(1);
    add_server_button_sizer->Add(add_server_button, 4, wxEXPAND);
    add_server_button_sizer->AddStretchSpacer(1);

    wxBoxSizer* main_vert_sizer = new wxBoxSizer(wxVERTICAL);

    main_vert_sizer->AddStretchSpacer(1);

    main_vert_sizer->Add(add_server_button_sizer, 4, wxEXPAND);

    main_vert_sizer->AddStretchSpacer(1);

    this->SetSizer(main_vert_sizer);
}

void ServersPanel::OpenAddServerPopupMenu(wxMouseEvent& event, wxWindow* parent) {
    AddServerPopupMenu* popup_menu = new AddServerPopupMenu(parent->MacGetTopLevelWindow(), wxDefaultPosition);

    popup_menu->CentreOnParent();
    popup_menu->ShowModal();
    popup_menu->Destroy();
}

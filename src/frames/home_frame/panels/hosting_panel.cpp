#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <vector>
#include <wx/wx.h>
#include "../../../main.hpp"
#include "../../../utils/net/net.hpp"
#include "../../../utils/crypto/crypto.hpp"
#include "panels.hpp"
#include "../../../widgets/widgets.hpp"

frames::home_frame::panels::HostingPanel::HostingPanel(wxPanel* parent_panel) : wxPanel(parent_panel) {
    widgets::Button* create_new_server_button = new widgets::Button(this, "Create New Server", [this](wxMouseEvent& event) { this->CreateNewServer(event); });
    create_new_server_button->SetMinSize(wxSize(-1, 40));
    create_new_server_button->SetMaxSize(wxSize(-1, 40));

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* main_sizer_margin = new wxBoxSizer(wxHORIZONTAL);

    this->hosted_servers_panel = new wxPanel(this);

    main_sizer->Add(this->hosted_servers_panel, 4, wxTOP | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 10);
    main_sizer->Add(create_new_server_button, 4, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM | wxEXPAND, 10);

    main_sizer_margin->AddStretchSpacer(2);
    main_sizer_margin->Add(main_sizer, 10, wxEXPAND);
    main_sizer_margin->AddStretchSpacer(2);

    this->SetSizer(main_sizer_margin);
}

void frames::home_frame::panels::HostingPanel::DrawServers() {
    this->hosted_servers_panel->DestroyChildren();

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

    this->hosted_servers_panel->SetSizer(v_sizer);

    in_addr public_ip_address = utils::net::get_public_ip();

    for (auto &server : wxGetApp().GetLocalStorageDataManager()->GetSavedData().hosted_servers) {
        uint16_t server_id = server.GetServerId();

        std::vector<uint8_t> invitation_data;
        invitation_data.resize(sizeof(public_ip_address) + sizeof(server_id));

        memcpy(invitation_data.data(), &public_ip_address, sizeof(public_ip_address));
        memcpy(invitation_data.data() + sizeof(public_ip_address), &server_id, sizeof(server_id));

        std::string invitation_code = utils::crypto::base32_encode(invitation_data);

        std::string server_button_string = std::to_string(server_id) + "   " + invitation_code;

        widgets::StyledPanel* server_panel = new widgets::StyledPanel(this->hosted_servers_panel);
        server_panel->SetMinSize(wxSize(-1, 30));
        server_panel->SetMaxSize(wxSize(-1, 30));

        widgets::Button* start_server_button = new widgets::Button(server_panel, "Start Server", [this, &server](wxMouseEvent& event){ server.StartServer(); });
        start_server_button->SetMinSize(wxSize(-1, 30));
        start_server_button->SetMaxSize(wxSize(-1, 30));

        wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* button_invitation_code_text = new wxStaticText(server_panel, wxID_ANY, invitation_code);

        button_sizer->Add(button_invitation_code_text, 4, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
        button_sizer->AddStretchSpacer(1);
        button_sizer->Add(start_server_button, 4, wxALIGN_CENTER_VERTICAL);

        server_panel->SetSizer(button_sizer);

        v_sizer->Add(server_panel, 4, wxEXPAND | wxTOP | wxBOTTOM, 5);
    }

    this->hosted_servers_panel->GetParent()->Layout();
    this->hosted_servers_panel->GetParent()->Refresh();
    this->hosted_servers_panel->GetParent()->SendSizeEvent();
}

void frames::home_frame::panels::HostingPanel::CreateNewServer(wxMouseEvent&) {
    uint16_t random_generated_id = rand();

    wxGetApp().GetLocalStorageDataManager()->InsertHostedServer(random_generated_id);

    this->DrawServers();
}

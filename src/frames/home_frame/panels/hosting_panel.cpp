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

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

    v_sizer->AddSpacer(10);

    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);

    this->hosted_servers_panel = new wxPanel(this);

    h_sizer->AddStretchSpacer(1);
    h_sizer->Add(create_new_server_button, 4, wxEXPAND);
    h_sizer->AddStretchSpacer(1);
    
    v_sizer->Add(this->hosted_servers_panel, 4, wxEXPAND);

    v_sizer->Add(h_sizer, 0, wxEXPAND | wxALL, 10);

    v_sizer->AddStretchSpacer(1);

    this->SetSizer(v_sizer);
}

void frames::home_frame::panels::HostingPanel::DrawServers() {
    this->hosted_servers_panel->DestroyChildren();

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

    this->hosted_servers_panel->SetSizer(v_sizer);

    in_addr public_ip_address = utils::net::get_public_ip();

    for (auto &server : wxGetApp().GetLocalStorageDataManager()->GetSavedData().hosted_servers) {
        wxBoxSizer* button_alignment_sizer = new wxBoxSizer(wxHORIZONTAL);

        uint16_t server_id = server.GetServerId();

        std::vector<uint8_t> invitation_data;
        invitation_data.resize(sizeof(public_ip_address) + sizeof(server_id));

        memcpy(invitation_data.data(), &public_ip_address, sizeof(public_ip_address));
        memcpy(invitation_data.data() + sizeof(public_ip_address), &server_id, sizeof(server_id));

        std::string invitation_code = utils::crypto::base32_encode(invitation_data);

        std::cout << "Invitation code: " << invitation_code << std::endl;

        std::string server_button_string = std::to_string(server_id);
        server_button_string += "   ";
        server_button_string += invitation_code;

        widgets::StyledPanel* server_panel = new widgets::StyledPanel(this->hosted_servers_panel);
        server_panel->SetMinSize(wxSize(-1, 30));

        widgets::Button* start_server_button = new widgets::Button(server_panel, "Start Server", [this, &server](wxMouseEvent& event){ server.StartServer(); });
        start_server_button->SetMinSize(wxSize(-1, 30));

        wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* button_invitation_code_text = new wxStaticText(server_panel, wxID_ANY, invitation_code);

        button_sizer->Add(button_invitation_code_text, 4, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL);
        button_sizer->AddStretchSpacer(10);
        button_sizer->Add(start_server_button, 4, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL);

        button_alignment_sizer->AddStretchSpacer(1);
        button_alignment_sizer->Add(server_panel, 4, wxEXPAND);
        button_alignment_sizer->AddStretchSpacer(1);

        server_panel->SetSizer(button_sizer);

        v_sizer->Add(button_alignment_sizer, 0, wxEXPAND | wxALL, 10);

        v_sizer->AddStretchSpacer(1);
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

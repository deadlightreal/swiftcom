#include <functional>
#include <string>
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

    for (auto &server : wxGetApp().GetLocalStorageDataManager()->GetSavedData().local_servers) {
        wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

        widgets::Button* server_button = new widgets::Button(this->hosted_servers_panel, std::to_string(server.GetServerId()).c_str(), nullptr);
        server_button->SetMinSize(wxSize(-1, 30));

        button_sizer->AddStretchSpacer(1);
        button_sizer->Add(server_button, 4, wxEXPAND);
        button_sizer->AddStretchSpacer(1);

        v_sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 10);

        v_sizer->AddStretchSpacer(1);
    }

    this->hosted_servers_panel->Layout();
}

void frames::home_frame::panels::HostingPanel::CreateNewServer(wxMouseEvent&) {
    struct in_addr public_ip = utils::net::get_public_ip();

    uint32_t random_generated_id = rand();

    this->GetLocalServers().push_back(
        objects::LocalServer(public_ip, random_generated_id)
    );

    wxGetApp().GetLocalStorageDataManager()->save_data();

    this->DrawServers();
}

std::vector<objects::LocalServer>& frames::home_frame::panels::HostingPanel::GetLocalServers() {
    return this->local_servers;
}

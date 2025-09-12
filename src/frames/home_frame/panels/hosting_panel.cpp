#include <functional>
#include <wx/wx.h>
#include "../../../utils/net/net.hpp"
#include "../../../utils/crypto/crypto.hpp"
#include "panels.hpp"
#include "../../../widgets/widgets.hpp"

objects::LocalStorageDataManager* frames::home_frame::panels::HostingPanel::GetLocalStorageDataManager() {
    return this->local_storage_data_manager;
}

frames::home_frame::panels::HostingPanel::HostingPanel(wxPanel* parent_panel, objects::LocalStorageDataManager* local_storage_data_manager) : wxPanel(parent_panel), local_storage_data_manager(local_storage_data_manager) {
    widgets::Button* create_new_server_button = new widgets::Button(this, "Create New Server", [this](wxMouseEvent& event) { this->CreateNewServer(event); });
    create_new_server_button->SetBackgroundColour(wxColour(255, 0, 0));
    create_new_server_button->SetMinSize(wxSize(-1, 40));

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

    v_sizer->AddSpacer(10);

    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);

    h_sizer->AddStretchSpacer(1);
    h_sizer->Add(create_new_server_button, 4, wxEXPAND);
    h_sizer->AddStretchSpacer(1);

    v_sizer->Add(h_sizer, 0, wxEXPAND | wxALL, 10);

    v_sizer->AddStretchSpacer(1);

    this->SetSizer(v_sizer);
}

void frames::home_frame::panels::HostingPanel::CreateNewServer(wxMouseEvent&) {
    struct in_addr public_ip = utils::net::get_public_ip();

    uint32_t random_generated_id = rand();

    this->GetLocalServers().push_back(
        objects::LocalServer(public_ip, random_generated_id)
    );

    this->GetLocalStorageDataManager()->save_data();
}

std::vector<objects::LocalServer>& frames::home_frame::panels::HostingPanel::GetLocalServers() {
    return this->local_servers;
}

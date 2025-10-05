#include <iostream>
#include <ostream>
#include <wx/wx.h>
#include "../../widgets/widgets.hpp"
#include "panels/panels.hpp"
#include "../frames.hpp"

frames::HomeFrame::HomeFrame() : wxFrame(nullptr, wxID_ANY, "Home Frame", wxDefaultPosition, wxSize(800, 600)) {
    timer.SetOwner(this);
    Bind(wxEVT_TIMER, &HomeFrame::Frame, this);
    timer.Start(16);

    wxPanel* mainPanel = new wxPanel(this);

    this->menu_bar = new widgets::MenuBar(mainPanel, {"Servers", "Hosting", "Settings"});

    this->hosting_panel = new frames::home_frame::panels::HostingPanel(mainPanel);
    this->servers_panel = new frames::home_frame::panels::ServersPanel(mainPanel);

    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    vSizer->Add(menu_bar, 0, wxEXPAND);
    vSizer->Add(servers_panel, 0, wxEXPAND);
    vSizer->Add(hosting_panel, 0, wxEXPAND);

    vSizer->AddStretchSpacer(1);

    mainPanel->SetSizer(vSizer);
}

frames::home_frame::panels::ServersPanel* frames::HomeFrame::GetServersPanel() {
    return this->servers_panel;
}

frames::home_frame::panels::HostingPanel* frames::HomeFrame::GetHostingPanel() {
    return this->hosting_panel;
}

frames::HomeFrame::~HomeFrame() {
    delete this->menu_bar;
    delete this->hosting_panel;
}
    
void frames::HomeFrame::Frame(wxTimerEvent& event) {
    switch(menu_bar->GetCurrentIndex()) {
        case 0:
            hosting_panel->Hide();
            servers_panel->Show();

            break;
        case 1:
            hosting_panel->Show();
            servers_panel->Hide();

            break;
        case 2:
            hosting_panel->Hide();
            servers_panel->Hide();

            break;
    }
}

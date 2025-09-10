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

    menu_bar = new widgets::MenuBar(mainPanel, {"Servers", "Hosting", "Settings"});

    hosting_panel = new frames::home_frame::panels::HostingPanel(mainPanel);

    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    vSizer->Add(menu_bar, 0, wxEXPAND);
    vSizer->Add(hosting_panel, 0, wxEXPAND);

    vSizer->AddStretchSpacer(1);

    mainPanel->SetSizer(vSizer);
}

frames::home_frame::panels::HostingPanel* frames::HomeFrame::GetHostingPanel() {
    return this->hosting_panel;
}

frames::HomeFrame::~HomeFrame() {
    delete this->menu_bar;
    delete this->hosting_panel;
}
    
void frames::HomeFrame::Frame(wxTimerEvent& event) {
    std::cout << menu_bar->GetCurrentIndex() << std::endl;

    switch(menu_bar->GetCurrentIndex()) {
        case 0:
            hosting_panel->Hide();

            break;
        case 1:
            hosting_panel->Show();

            break;
        case 2:
            hosting_panel->Hide();

            break;
    }
}

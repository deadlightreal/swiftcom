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

using frames::home_frame::panels::ServersPanel;

ServersPanel::AddServerPopupMenu::AddServerReturnCode ServersPanel::AddServerPopupMenu::AddServer(wxString input) {
    std::array<uint8_t, 8> decoded_code = utils::crypto::base32_decode(input.ToStdString());

    in_addr server_ip_address;
    uint32_t server_id;

    memcpy(&server_ip_address, &decoded_code[0], sizeof(server_ip_address));
    memcpy(&server_id, &decoded_code[4], sizeof(server_id));

    const char* server_id_string = inet_ntoa(server_ip_address);

    std::cout << "adding server with ip: " << server_id_string << std::endl;

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

#pragma once

#include <cstdint>
#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel);

        void DrawServers();
    private:
        void CreateNewServer(wxMouseEvent&);

        wxPanel* hosted_servers_panel;
    };

    class ServersPanel : public wxPanel {
    public:
        class AddServerPopupMenu : public wxDialog {
        public:
            void RequestServerExistsConfirmation(const char* ip_address, const uint16_t server_id);

            enum AddServerReturnCode {
                SUCCESS,
                INVALID_LENGTH
            };

            AddServerPopupMenu(wxWindow* parent, wxPoint pos);

            AddServerReturnCode AddServer(wxString input);
        };

        ServersPanel(wxPanel* parent_panel);

        void OpenAddServerPopupMenu(wxMouseEvent& event, wxWindow* parent);
    private:
    };
}

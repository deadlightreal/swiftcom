#pragma once

#include <cstdint>
#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel);
        ~HostingPanel();

        void DrawServers();
    private:
        void CreateNewServer(wxMouseEvent&);

        wxPanel* hosted_servers_panel;
    };

    class ServersPanel : public wxPanel {
    public:
        class AddServerPopupMenu : public wxDialog {
        public:
            enum AddServerReturnCode {
                SUCCESS,
                INVALID_LENGTH
            };

            AddServerPopupMenu(wxWindow* parent, wxPoint pos);
            ~AddServerPopupMenu();

            void RequestServerExistsConfirmation(const char* ip_address, const uint16_t server_id);

            AddServerReturnCode AddServer(wxString input);
        };

        ServersPanel(wxPanel* parent_panel);
        ~ServersPanel();

        void DrawServers();

        void OpenAddServerPopupMenu(wxMouseEvent& event, wxWindow* parent);
    private:
        wxPanel* joined_servers_panel;
    };
}

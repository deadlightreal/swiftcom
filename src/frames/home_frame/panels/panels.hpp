#pragma once

#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel);

        std::vector<objects::LocalServer>& GetLocalServers();

        void DrawServers();
    private:
        void CreateNewServer(wxMouseEvent&);

        wxPanel* hosted_servers_panel;

        std::vector<objects::LocalServer> local_servers;
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

            AddServerReturnCode AddServer(wxString input);
        };

        ServersPanel(wxPanel* parent_panel);

        void OpenAddServerPopupMenu(wxMouseEvent& event, wxWindow* parent);
    private:
    };
}

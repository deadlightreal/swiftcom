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
}

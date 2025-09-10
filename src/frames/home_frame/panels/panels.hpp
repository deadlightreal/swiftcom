#pragma once

#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel);

        std::vector<objects::LocalServer>& GetLocalServers();
    private:
        void CreateNewServer(wxMouseEvent&);

        std::vector<objects::LocalServer> local_servers;
    };
}

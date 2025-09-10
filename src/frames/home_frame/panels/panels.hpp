#pragma once

#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel);
    private:
        void CreateNewServer(wxMouseEvent&);

        std::vector<objects::LocalServer>& GetLocalServers() {
            return this->local_servers;
        }

        std::vector<objects::LocalServer> local_servers;
    };
}

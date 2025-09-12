#pragma once

#include <wx/wx.h>
#include "../../../objects/objects.hpp"
#include <vector>

namespace frames::home_frame::panels {
    class HostingPanel : public wxPanel {
    public:
        HostingPanel(wxPanel* parent_panel, objects::LocalStorageDataManager* local_storage_data_manager);

        std::vector<objects::LocalServer>& GetLocalServers();

        objects::LocalStorageDataManager* GetLocalStorageDataManager();
    private:
        void CreateNewServer(wxMouseEvent&);

        std::vector<objects::LocalServer> local_servers;
        objects::LocalStorageDataManager* local_storage_data_manager;
    };
}

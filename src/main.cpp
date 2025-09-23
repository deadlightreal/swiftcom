#include <cstdlib>
#include <ctime>
#include <iostream>
#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include "main.hpp"

Application::Application() {
    this->home_frame = new frames::HomeFrame();
}

Application::~Application() {
    const objects::LocalStorageDataManager* local_storage_data_manager = this->GetLocalStorageDataManager();
    
    if (local_storage_data_manager != nullptr) {
        delete local_storage_data_manager;
    }
}

frames::HomeFrame* Application::GetHomeFrame() {
    return this->home_frame;
}

objects::LocalStorageDataManager* Application::GetLocalStorageDataManager() {
    return this->local_storage_data_manager;
}

bool Application::OnInit() {
    srand(time(0));

    frames::HomeFrame* home_frame = this->GetHomeFrame();

    this->local_storage_data_manager = new objects::LocalStorageDataManager();

    home_frame->GetHostingPanel()->DrawServers();

    home_frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(Application);

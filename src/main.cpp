#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include "main.hpp"

Application::Application() {
    this->home_frame = new frames::HomeFrame();
}

Application::~Application() {
    delete this->home_frame;
    delete this->GetLocalStorageDataManager();
}

frames::HomeFrame* Application::GetHomeFrame() {
    return this->home_frame;
}

objects::LocalStorageDataManager* Application::GetLocalStorageDataManager() {
    return this->local_storage_data_manager;
}

bool Application::OnInit() {
    frames::HomeFrame* home_frame = this->GetHomeFrame();

    this->local_storage_data_manager = new objects::LocalStorageDataManager(home_frame->GetHostingPanel()->GetLocalServers());

    home_frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(Application);

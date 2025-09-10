#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include "main.hpp"

Application::Application() {
    this->home_frame = new frames::HomeFrame();
}

Application::~Application() {
    delete this->home_frame;

    if (this->GetLocalStorageDataManager() != nullptr) {
        delete this->GetLocalStorageDataManager();
    }
}

frames::HomeFrame* Application::GetHomeFrame() {
    return this->home_frame;
}

objects::LocalStorageDataManager* Application::GetLocalStorageDataManager() {
    return this->local_storage_data_manager;
}

bool Application::OnInit() {
    frames::HomeFrame* frame = new frames::HomeFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(Application);

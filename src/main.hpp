#pragma once

#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include "sqlite3.h"

class Application : public wxApp
{
public:
    virtual bool OnInit();

    Application();
    ~Application();

    objects::LocalStorageDataManager* GetLocalStorageDataManager();

    frames::HomeFrame* GetHomeFrame();
private:
    frames::HomeFrame* home_frame;
    objects::LocalStorageDataManager* local_storage_data_manager;
};

wxDECLARE_APP(Application);

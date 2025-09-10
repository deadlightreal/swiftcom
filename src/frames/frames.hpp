#pragma once

#include <wx/wx.h>
#include "../widgets/widgets.hpp"
#include "home_frame/panels/panels.hpp"
#include <vector>
#include "../objects/objects.hpp"

namespace frames {
    class HomeFrame : public wxFrame {
    public:
        HomeFrame();
        ~HomeFrame();
        void Frame(wxTimerEvent& event);
    private:
        frames::home_frame::panels::HostingPanel* hosting_panel;
        widgets::MenuBar* menu_bar;
        uint8_t current_menu;
        wxTimer timer;
    };
}

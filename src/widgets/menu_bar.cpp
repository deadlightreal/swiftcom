#include <initializer_list>
#include <wx/wx.h>
#include "widgets.hpp"

using namespace widgets;

MenuBar::MenuBar(wxPanel* parent_panel, std::initializer_list<const char*> items) : wxPanel(parent_panel) {
    this->SetMinSize(wxSize(0, 30));

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    for(uint32_t i = 0; i < items.size(); i++) {
        MenuBarButton* button = new MenuBarButton(this, *(items.begin() + i), i, &current_index);

        sizer->Add(button, 1, wxEXPAND | wxALL, 0);
    }

    this->SetSizer(sizer);
}

MenuBar::~MenuBar() {

}

uint32_t MenuBar::GetCurrentIndex() {
    return this->current_index;
}

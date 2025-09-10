#include <initializer_list>
#include <wx/wx.h>
#include "widgets.hpp"

widgets::MenuBarButton::MenuBarButton(wxWindow* parent, const wxString& label, const uint32_t index, uint32_t* const current_index) : wxControl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE), label(label), index(index), current_index(current_index) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &MenuBarButton::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &MenuBarButton::OnClick, this);
    Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent&) { hover = true; Refresh(); });
    Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent&) { hover = false; Refresh(); });
}

void widgets::MenuBarButton::OnPaint(wxPaintEvent&) {
    wxPaintDC dc(this);
    wxSize size = GetClientSize();

    wxColour bg = *current_index == index ? wxColour(120, 120, 120) : hover ? wxColour(50, 50, 50) : wxColour(0, 0, 0);
    dc.SetBrush(wxBrush(bg));

    dc.SetPen(wxPen(wxColour(120, 120, 120), 2));
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    dc.SetTextForeground(wxColour(200, 200, 200));
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxCoord w, h;
    dc.GetTextExtent(label, &w, &h);
    dc.DrawText(label, (size.GetWidth() - w) / 2, (size.GetHeight() - h) / 2);
}

void widgets::MenuBarButton::OnClick(wxMouseEvent&) {
    *current_index = index;
}

widgets::MenuBar::MenuBar(wxPanel* parent_panel, std::initializer_list<const char*> items) : wxPanel(parent_panel) {
    this->SetMinSize(wxSize(0, 30));

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    for(uint32_t i = 0; i < items.size(); i++) {
        MenuBarButton* button = new MenuBarButton(this, *(items.begin() + i), i, &current_index);

        sizer->Add(button, 1, wxEXPAND | wxALL, 0);
    }

    this->SetSizer(sizer);
}

uint32_t widgets::MenuBar::GetCurrentIndex() {
    return this->current_index;
}

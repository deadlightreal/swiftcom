#include "widgets.hpp"
#include <wx/wx.h>

widgets::Button::Button(wxWindow* parent, const wxString& label, std::function<void(wxMouseEvent&)> on_click) : wxControl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE), label(label) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &Button::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, on_click);
    Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent&) { hover = true; Refresh(); });
    Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent&) { hover = false; Refresh(); });
}

void widgets::Button::OnPaint(wxPaintEvent&) {
    wxPaintDC dc(this);
    wxSize size = GetClientSize();

    wxColour bg = hover ? wxColour(50, 50, 50) : wxColour(0, 0, 0);
    dc.SetBrush(wxBrush(bg));

    dc.SetPen(wxPen(wxColour(120, 120, 120), 2));
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    dc.SetTextForeground(wxColour(200, 200, 200));
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxCoord w, h;
    dc.GetTextExtent(label, &w, &h);
    dc.DrawText(label, (size.GetWidth() - w) / 2, (size.GetHeight() - h) / 2);
}

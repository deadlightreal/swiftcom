#include "widgets.hpp"
#include <sys/socket.h>

using namespace widgets;

StyledPanel::StyledPanel(wxWindow* parent) : wxPanel(parent) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &StyledPanel::OnPaint, this);
}

StyledPanel::~StyledPanel() {

}

void StyledPanel::OnPaint(wxPaintEvent&) {
    wxPaintDC dc(this);
    wxSize size = GetClientSize();

    wxColour bg = wxColour(0, 0, 0);
    dc.SetBrush(wxBrush(bg));

    dc.SetPen(wxPen(wxColour(120, 120, 120), 2));
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
}

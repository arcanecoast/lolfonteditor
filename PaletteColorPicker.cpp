#include "PaletteColorPicker.h"

BEGIN_EVENT_TABLE(PaletteColorPicker, wxPanel)
    EVT_PAINT(PaletteColorPicker::OnPaint)
    EVT_LEFT_UP(PaletteColorPicker::OnLeftClick)
    EVT_RIGHT_UP(PaletteColorPicker::OnRightClick)
END_EVENT_TABLE()


PaletteColorPicker::PaletteColorPicker(wxWindow* parent, const wxPoint& pos, const wxSize& size) :
wxPanel(parent, wxID_ANY, pos, size), m_selectedColor(-1)
{
}

int16 PaletteColorPicker::GetSelectedColorIndex() const
{
    return m_selectedColor;
}

void PaletteColorPicker::SetSelectedColorIndex(int16 selectedColor)
{
    m_selectedColor = selectedColor;
}

void PaletteColorPicker::OnLeftClick(wxMouseEvent& evt)
{
    int symbol_width = GetSize().GetWidth() / 16;
    int symbol_height = GetSize().GetHeight();

    m_selectedColor = evt.GetX() / symbol_width;

    Refresh();
}

void PaletteColorPicker::OnRightClick(wxMouseEvent& evt)
{
    m_selectedColor = -1;

    Refresh();
}

void PaletteColorPicker::OnPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    wxPen defaultPen(wxColor(64, 64, 64)), selectedPen(wxColor(255, 255, 0), 2);

    int symbol_width  = GetSize().GetWidth() / 16;
    int symbol_height = GetSize().GetHeight() - 16;

    wxColor colors[16];

    for (int i = 0; i < 256; i += 16) {
        colors[i / 16] = wxColor(255 - i, 255 - i, 255 - i);
    }

    for (int i = 0; i < 256; i += 80) {
        colors[i / 80] = wxColor(255 - i, 255 - i, 255 - i);
    }

    for (int i = 0; i < 16; ++i) {
        wxPoint position;
        position.x = symbol_width * i;
        position.y = 0;

        wxBrush tempBrush = wxBrush(colors[i]);
        dc.SetBrush(tempBrush);

        if (m_selectedColor == i) {
            dc.SetPen(selectedPen);
        } else {
            dc.SetPen(defaultPen);
        }

        dc.DrawRectangle(position, wxSize(symbol_width, symbol_height));
        dc.DrawText(wxString::Format("%d", i), wxPoint(position.x, symbol_height + 2));
    }
}

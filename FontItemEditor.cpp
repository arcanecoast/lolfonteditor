#include "FontItemEditor.h"
#include "CurrentFontSingleton.h"
#include "PaletteColorPicker.h"

BEGIN_EVENT_TABLE(FontItemEditor, wxPanel)
    EVT_PAINT(FontItemEditor::OnPaint)
    EVT_LEFT_UP(FontItemEditor::OnLeftClick)
    EVT_RIGHT_UP(FontItemEditor::OnRightClick)
END_EVENT_TABLE()

const wxSize FontItemEditor::PIXEL_SIZE(20, 20);

FontItemEditor::FontItemEditor(wxWindow* parent, FontItem* assignedItem, PaletteColorPicker* colorPicker, const wxPoint& pos, const wxSize& size) :
wxPanel(parent, wxID_ANY, pos, size), m_colorPicker(colorPicker), m_assignedItem(assignedItem)
{
}

void FontItemEditor::OnLeftClick(wxMouseEvent& evt)
{
    int16 colorToSet = m_colorPicker->GetSelectedColorIndex();
    if (colorToSet >= 0 && colorToSet <= 15) {
        int x = evt.GetX() / PIXEL_SIZE.GetWidth();
        int y = evt.GetY() / PIXEL_SIZE.GetHeight();

        m_assignedItem->SetPixel(x, y, colorToSet);
    }

    Refresh();
}

void FontItemEditor::OnRightClick(wxMouseEvent& evt)
{
    int x = evt.GetX() / PIXEL_SIZE.GetWidth();
    int y = evt.GetY() / PIXEL_SIZE.GetHeight();

    m_assignedItem->SetPixel(x, y, 0);

    Refresh();
}

void FontItemEditor::OnPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(this);

    dc.SetPen(wxPen(wxColor(255, 0, 255)));
    dc.SetBrush(wxBrush(wxColor(255, 0, 255)));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());

    if (!m_assignedItem) {
        return;
    }

    dc.SetPen(wxPen(wxColor(0, 0, 0)));

    uint8 symbol_width, symbol_height;
    m_assignedItem->GetSize(symbol_width, symbol_height);

    wxColor colors[16];

    for (int i = 0; i < 256; i += 16) {
        colors[i / 16] = wxColor(255 - i, 255 - i, 255 - i);
    }

    for (int i = 0; i < 256; i += 80) {
        colors[i / 80] = wxColor(255 - i, 255 - i, 255 - i);
    }

    colors[0] = wxColor(255, 0, 255);

    uint16 iCount = CurrentFontSingleton::GetInstance().Size();
    uint8 pixelColor;

    for (int h = 0; h < symbol_height; ++h) {
        for (int w = 0; w < symbol_width; ++w) {
            m_assignedItem->GetPixel(w, h, pixelColor);

            wxBrush tempBrush = wxBrush(colors[pixelColor]);
            dc.SetBrush(tempBrush);

            dc.DrawRectangle(w * PIXEL_SIZE.GetWidth(), h * PIXEL_SIZE.GetHeight(), PIXEL_SIZE.GetWidth(), PIXEL_SIZE.GetHeight());

            dc.SetTextForeground(wxColor(255, 255, 0));
            dc.DrawText(wxString::Format("%i", pixelColor), wxPoint(w * PIXEL_SIZE.GetWidth() + 2, h * PIXEL_SIZE.GetHeight() + 2));
        }
    }
}

void FontItemEditor::AssignItem(FontItem* assignedItem)
{
    m_assignedItem = assignedItem;

    Refresh();
}

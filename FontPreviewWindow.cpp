#include "FontPreviewWindow.h"
#include "CurrentFontSingleton.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

BEGIN_EVENT_TABLE(FontPreviewWindow, wxDialog)
    EVT_PAINT(FontPreviewWindow::OnPaint)
    EVT_ERASE_BACKGROUND(FontPreviewWindow::OnEraseBackground)
END_EVENT_TABLE()

FontPreviewWindow::FontPreviewWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size) :
wxDialog(parent, wxID_ANY, _("Предпросмотр шрифта"), pos, size, wxDEFAULT_FRAME_STYLE)
{
}

void FontPreviewWindow::OnPaint(wxPaintEvent & evt)
{
    wxBufferedPaintDC bufferDC(this);

    DoPaint(bufferDC);
}

void FontPreviewWindow::OnEraseBackground(wxEraseEvent& evt)
{
    wxClientDC dc(this);
    wxBufferedDC bufferDC(&dc);

    DoPaint(bufferDC);
}

void FontPreviewWindow::DoPaint(wxDC& dc)
{
    dc.SetBrush(wxBrush(wxColor(255,0,255)));
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());

    int offset_x = 20;
    int offset_y = 10;

    wxSize pixelSize(3, 3);

    int symbol_width = pixelSize.GetWidth() * 15;
    int symbol_height = pixelSize.GetHeight() * 15;

    int symbols_per_line = (GetSize().GetWidth() - offset_x * 2) / symbol_width;

    /*wxColor colors[16] = {
    wxColor(255, 255, 255),
    wxColor(255, 255, 255),
    wxColor(128, 128, 128),
    wxColor(0, 0, 0),
    wxColor(216, 216, 216),
    wxColor(196, 196, 196),
    wxColor(180, 180, 180),
    wxColor(170, 170, 170),
    wxColor(160, 160, 160),
    wxColor(150, 150, 150),
    wxColor(140, 140, 140),
    wxColor(128, 128, 128),
    wxColor(110, 110, 110),
    wxColor(92, 92, 92),
    wxColor(78, 78, 78),
    wxColor(64, 64, 64)
    };*/

    wxColor colors[16];

    for (int i = 0; i < 256; i += 16) {
        colors[i / 16] = wxColor(255 - i, 255 - i, 255 - i);
    }

    for (int i = 0; i < 256; i += 80) {
        colors[i / 80] = wxColor(255 - i, 255 - i, 255 - i);
    }

    uint16 iCount = CurrentFontSingleton::GetInstance().Size();

    for (int i = 0; i < iCount; ++i) {
        wxPoint position;
        position.x = (i % symbols_per_line) * symbol_width + offset_x;
        position.y = (i / symbols_per_line) * symbol_height + offset_y;

        LandsOfLore::FontItem& item = CurrentFontSingleton::GetInstance().GetItem(i);

        uint8 width, height;
        uint8 pixelColor;
        item.GetSize(width, height);

        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                item.GetPixel(w, h, pixelColor);

                if (pixelColor != 0) {
                    wxPen   tempPen   = wxPen(colors[pixelColor]);
                    wxBrush tempBrush = wxBrush(colors[pixelColor]);

                    dc.SetPen(tempPen);
                    dc.SetBrush(tempBrush);

                    dc.DrawRectangle(w * pixelSize.GetWidth() + position.x, h * pixelSize.GetHeight() + position.y, pixelSize.GetWidth(), pixelSize.GetHeight());
                }
            }
        }
    }
}

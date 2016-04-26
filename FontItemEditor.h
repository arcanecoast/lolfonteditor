#pragma once

#include "LandsOfLore/FontItem.h"

#include <wx/wx.h>

using LandsOfLore::FontItem;

class PaletteColorPicker;

class FontItemEditor : public wxPanel
{
private:
    static const wxSize PIXEL_SIZE;

    FontItem* m_assignedItem;
    PaletteColorPicker* m_colorPicker;

public:
    FontItemEditor(wxWindow* parent, FontItem* assignedItem, PaletteColorPicker* colorPicker, const wxPoint& pos, const wxSize& size);

    void AssignItem(FontItem* assignedItem);

private:
    void OnLeftClick(wxMouseEvent& evt);
    void OnRightClick(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent& evt);

    DECLARE_EVENT_TABLE()
};

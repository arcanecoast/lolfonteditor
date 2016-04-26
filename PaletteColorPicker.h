#pragma once

#include "CommonTypes.h"

#include <wx/wx.h>

class PaletteColorPicker : public wxPanel
{
private:
    int16 m_selectedColor;

public:
    PaletteColorPicker(wxWindow* parent, const wxPoint& pos, const wxSize& size);

    int16 GetSelectedColorIndex() const;
    void SetSelectedColorIndex(int16 selectedColor);

private:
    void OnLeftClick(wxMouseEvent& evt);
    void OnRightClick(wxMouseEvent& evt);

    void OnPaint(wxPaintEvent& evt);

    DECLARE_EVENT_TABLE()
};

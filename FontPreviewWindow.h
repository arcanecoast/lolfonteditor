#pragma once

#include <wx/dialog.h>

class FontPreviewWindow: public wxDialog
{
public:
    FontPreviewWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size);

public:
    void OnPaint(wxPaintEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);

private:
    void DoPaint(wxDC& dc);

private:
    DECLARE_EVENT_TABLE()
};
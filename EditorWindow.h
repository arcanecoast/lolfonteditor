#pragma once

#include <wx/wx.h>

class PaletteColorPicker;
class FontItemEditor;

namespace LandsOfLore {
    class FontItem;
}

using LandsOfLore::FontItem;

class EditorWindow : public wxFrame
{
private:
    wxListBox *m_listCharacters;

    wxSpinCtrl *m_spinWidth;
    wxSpinCtrl *m_spinHeight;
    wxSpinCtrl *m_spinUnknown;

    PaletteColorPicker *m_colorPicker;
    FontItemEditor *m_itemEditor;

public:
    EditorWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnFileOpen(wxCommandEvent& event);
    void OnFileSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void OnViewShowFont(wxCommandEvent& event);

    void OnItemSelected(wxCommandEvent& event);

    void OnItemWidthChanged(wxSpinEvent& event);
    void OnItemHeightChanged(wxSpinEvent& event);
    void OnItemUnknownChanged(wxSpinEvent& event);

    void ChangeFontItemSize(FontItem& item, int newWidth, int newHeight);

    DECLARE_EVENT_TABLE()
};

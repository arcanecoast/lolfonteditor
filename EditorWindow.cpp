#include "EditorWindow.h"
#include "CurrentFontSingleton.h"
#include "FontPreviewWindow.h"
#include "PaletteColorPicker.h"
#include "FontItemEditor.h"

#include <wx/spinctrl.h>

enum {
    ID_FILE_OPEN = wxID_HIGHEST + 1,
    ID_FILE_SAVE_AS,
    ID_FILE_EXIT,
    ID_VIEW_PREVIEWFONT,

    ID_CTRL_LISTBOX_CHARACTERS,
    ID_CTRL_SPIN_WIDTH,
    ID_CTRL_SPIN_HEIGHT,
    ID_CTRL_SPIN_UNKNOWN
};

BEGIN_EVENT_TABLE(EditorWindow, wxFrame)
    EVT_MENU(ID_FILE_OPEN, EditorWindow::OnFileOpen)
    EVT_MENU(ID_FILE_SAVE_AS, EditorWindow::OnFileSaveAs)
    EVT_MENU(ID_FILE_EXIT, EditorWindow::OnExit)

    EVT_MENU(ID_VIEW_PREVIEWFONT, EditorWindow::OnViewShowFont)

    EVT_LISTBOX(ID_CTRL_LISTBOX_CHARACTERS, EditorWindow::OnItemSelected)

    EVT_SPINCTRL(ID_CTRL_SPIN_WIDTH, EditorWindow::OnItemWidthChanged)
    EVT_SPINCTRL(ID_CTRL_SPIN_HEIGHT, EditorWindow::OnItemHeightChanged)
    EVT_SPINCTRL(ID_CTRL_SPIN_UNKNOWN, EditorWindow::OnItemUnknownChanged)
END_EVENT_TABLE()

EditorWindow::EditorWindow(const wxString& title, const wxPoint& pos, const wxSize& size) :
wxFrame(0, wxID_ANY, title, pos, size, wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION)
{
    wxMenuBar* menubarMain = new wxMenuBar();

    wxMenu *menuFile = new wxMenu();
    menuFile->Append(ID_FILE_OPEN, _("Открыть..."));
    menuFile->Append(ID_FILE_SAVE_AS, _("Сохранить как..."));
    menuFile->AppendSeparator();
    menuFile->Append(ID_FILE_EXIT, _("Выход"));

    wxMenu *menuView = new wxMenu();
    menuView->Append(ID_VIEW_PREVIEWFONT, _("Предпросмотр шрифта"));

    menubarMain->Append(menuFile, _("Файл"));
    menubarMain->Append(menuView, _("Вид"));

    SetMenuBar(menubarMain);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(leftSizer, 1, wxEXPAND);

    m_listCharacters = new wxListBox(this, ID_CTRL_LISTBOX_CHARACTERS, wxPoint(5, 5), wxSize(120, 300));
    wxPanel *panelConfig = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER);

    new wxStaticText(panelConfig, wxID_ANY, _("Ширина:"), wxPoint(5, 5), wxDefaultSize);
    m_spinWidth = new wxSpinCtrl(panelConfig, ID_CTRL_SPIN_WIDTH, wxEmptyString, wxPoint(5, 25), wxSize(100, 25), wxSP_ARROW_KEYS | wxSP_VERTICAL, 0, 255);

    new wxStaticText(panelConfig, wxID_ANY, _("Высота:"), wxPoint(120, 5), wxDefaultSize);
    m_spinHeight = new wxSpinCtrl(panelConfig, ID_CTRL_SPIN_HEIGHT, wxEmptyString, wxPoint(120, 25), wxSize(100, 25), wxSP_ARROW_KEYS | wxSP_VERTICAL, 0, 255);

    m_colorPicker = new PaletteColorPicker(panelConfig, wxPoint(5, 60), wxSize(250, 40));

    new wxStaticText(panelConfig, wxID_ANY, _("Unknown (смещение по y?):"), wxPoint(5, 110), wxDefaultSize);
    m_spinUnknown = new wxSpinCtrl(panelConfig, ID_CTRL_SPIN_UNKNOWN, wxEmptyString, wxPoint(5, 130), wxSize(100, 25), wxSP_ARROW_KEYS | wxSP_VERTICAL, 0, 255);

    leftSizer->Add(m_listCharacters, 3, wxEXPAND);
    leftSizer->Add(panelConfig, 2, wxEXPAND);

    m_itemEditor = new FontItemEditor(this, 0, m_colorPicker, wxDefaultPosition, wxDefaultSize);

    mainSizer->Add(m_itemEditor, 2, wxEXPAND);

    SetSizer(mainSizer);
}

void EditorWindow::OnFileOpen(wxCommandEvent& event)
{
    wxFileDialog fdOpen(this, _("Выберите файл шрифта"), wxEmptyString, wxEmptyString, "Lands of Lore .FNT|*.FNT", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    fdOpen.ShowModal();

    if (fdOpen.GetFilename() != wxEmptyString) {
        CurrentFontSingleton::GetInstance().LoadFromFile(fdOpen.GetPath().c_str());

        m_listCharacters->Clear();

        m_spinWidth->SetValue(0);
        m_spinHeight->SetValue(0);

        m_itemEditor->AssignItem(0);

        for (int i = 0; i < CurrentFontSingleton::GetInstance().Size(); ++i) {
            m_listCharacters->Append(wxString::Format("%i - %c", i, i));
        }
    }
}

void EditorWindow::OnFileSaveAs(wxCommandEvent& event)
{
    wxFileDialog fdSave(this, _("Назовите файл шрифта"), wxEmptyString, wxEmptyString, "Lands of Lore .FNT|*.FNT", wxFD_SAVE| wxFD_OVERWRITE_PROMPT);
    fdSave.ShowModal();

    if (fdSave.GetFilename() != wxEmptyString) {
        CurrentFontSingleton::GetInstance().SaveToFile(fdSave.GetPath().c_str());
    }
}

void EditorWindow::OnExit(wxCommandEvent& event)
{
    Close();
}

void EditorWindow::OnViewShowFont(wxCommandEvent& event)
{
    FontPreviewWindow fontPreview(this, wxDefaultPosition, wxSize(800, 600));
    fontPreview.ShowModal();
}

void EditorWindow::OnItemSelected(wxCommandEvent& event)
{
    int selectedItem = m_listCharacters->GetSelection();

    if (selectedItem < 0) {
        return;
    }

    FontItem& item = CurrentFontSingleton::GetInstance().GetItem(selectedItem);

    uint8 width, height, unknown;
    item.GetSize(width, height);
    unknown = item.GetUnknown();

    m_spinWidth->SetValue(width);
    m_spinHeight->SetValue(height);
    m_spinUnknown->SetValue(unknown);
   
    m_itemEditor->AssignItem(&item);
}

void EditorWindow::OnItemWidthChanged(wxSpinEvent& event)
{
    int selectedItem = m_listCharacters->GetSelection();

    if (selectedItem < 0) {
        return;
    }

    FontItem& item = CurrentFontSingleton::GetInstance().GetItem(selectedItem);

    uint8 oldWidth, oldHeight;
    item.GetSize(oldWidth, oldHeight);

    uint8 newWidth  = m_spinWidth->GetValue();
    uint8 newHeight = oldHeight;

    ChangeFontItemSize(item, newWidth, newHeight);

    m_itemEditor->AssignItem(&item);
}

void EditorWindow::OnItemHeightChanged(wxSpinEvent& event)
{
    int selectedItem = m_listCharacters->GetSelection();

    if (selectedItem < 0) {
        return;
    }

    FontItem& item = CurrentFontSingleton::GetInstance().GetItem(selectedItem);

    uint8 oldWidth, oldHeight;
    item.GetSize(oldWidth, oldHeight);
    
    uint8 newWidth  = oldWidth;
    uint8 newHeight = m_spinHeight->GetValue();

    ChangeFontItemSize(item, newWidth, newHeight);

    m_itemEditor->AssignItem(&item);
}

void EditorWindow::ChangeFontItemSize(FontItem& item, int newWidth, int newHeight)
{
    FontItem itemCopy(item);

    uint8 oldWidth, oldHeight;
    item.GetSize(oldWidth, oldHeight);
    
    item.SetSize(newWidth, newHeight);

    for (int w = 0, maxwidth = oldWidth < newWidth ? oldWidth : newWidth; w < maxwidth; ++w) {
        for (int h = 0, maxheight = oldHeight < newHeight ? oldHeight : newHeight; h < maxheight; ++h) {
            uint8 color;
            itemCopy.GetPixel(w, h, color);
            item.SetPixel(w, h, color);
        }
    }
}

void EditorWindow::OnItemUnknownChanged(wxSpinEvent& event)
{
    int selectedItem = m_listCharacters->GetSelection();

    if (selectedItem < 0) {
        return;
    }

    FontItem& item = CurrentFontSingleton::GetInstance().GetItem(selectedItem);

    item.SetUnknown(m_spinUnknown->GetValue());
}

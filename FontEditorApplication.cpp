#include "FontEditorApplication.h"
#include "EditorWindow.h"

IMPLEMENT_APP(FontEditorApplication)

bool FontEditorApplication::OnInit()
{
    EditorWindow *editor = new EditorWindow("Lands of Lore Font Editor", wxDefaultPosition, wxSize(800, 600));
    editor->Show();

    return true;
}

/**********************************************************************************************************************/
//Artilab
//File: editorbar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Editorbar constructor function
Editorbar::Editorbar() 
{

}

//Editorbar destructor function
Editorbar::~Editorbar() {

}

//Editorbar clear selection function
void Editorbar::clearSelection() {
    selection = NONE;
}

//Editorbar hide function
void Editorbar::hide(MainViewport* mainViewport) {
    mainViewport->removeSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
}

//Editorbar open function
void Editorbar::open(MainViewport* mainViewport) {
    //Position and size the window
    wndSize = { DEF_WND_WIDTH , mainViewport->workSize().height };
    wndPos = { mainViewport->workPosition().x, mainViewport->workPosition().y };
    mainViewport->addSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
    wndFlags = NoNav | NoDecoration;

    ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
    ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));

    //Create window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
    if (create("##Editorbar", wndFlags)) {
        selection = NONE;

        

        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}
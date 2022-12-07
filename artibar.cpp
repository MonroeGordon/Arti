/**********************************************************************************************************************/
//Artilab
//File: artibar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Artibar constructor function
Artibar::Artibar() :
    viewerBtn(0.0f, 0.0f),
    gamesBtn(0.0f, 64.0f)
{

}

//Artibar destructor function
Artibar::~Artibar() {

}

//Artibar clear selection function
void Artibar::clearSelection() {
    viewerBtn.deselect();
    gamesBtn.deselect();
    selection = NONE;
}

//Artibar hide function
void Artibar::hide(MainViewport* mainViewport) {
    mainViewport->removeSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
}

//Artibar open function
void Artibar::open(MainViewport* mainViewport) {
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
    if (create("##Artibar", wndFlags)) {
        selection = NONE;

        if (viewerBtn.process()) {
            editorBtn.deselect();
            gamesBtn.deselect();
            selection = VIEWER_BUTTON;
        }

        if (editorBtn.process()) {
            viewerBtn.deselect();
            gamesBtn.deselect();
            selection = EDITOR_BUTTON;
        }

        if (gamesBtn.process()) {
            viewerBtn.deselect();
            editorBtn.deselect();
            selection = GAMES_BUTTON;
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}
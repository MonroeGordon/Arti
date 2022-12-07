/**********************************************************************************************************************/
//Artilab
//File: gamesbar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Gamesbar constructor function
Gamesbar::Gamesbar()
{

}

//Gamesbar destructor function
Gamesbar::~Gamesbar() {

}

//Gamesbar clear selection function
void Gamesbar::clearSelection() {
    selection = NONE;
}

//Gamesbar hide function
void Gamesbar::hide(MainViewport* mainViewport) {
    mainViewport->removeSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
}

//Gamesbar open function
void Gamesbar::open(MainViewport* mainViewport) {
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
    if (create("##Gamesbar", wndFlags)) {
        selection = NONE;

        if (tttBtn.process()) {
            chessBtn.deselect();
            selection = TIC_TAC_TOE_BUTTON;
        }

        if (chessBtn.process()) {
            tttBtn.deselect();
            selection = CHESS_BUTTON;
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}
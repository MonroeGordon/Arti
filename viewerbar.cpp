/**********************************************************************************************************************/
//Artilab
//File: viewerbar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Viewerbar constructor function
Viewerbar::Viewerbar() :
    tableBtn(0.0f, 0.0f),
    treeBtn(0.0f, 64.0f),
    matrixBtn(0.0f, 128.0f)
{

}

//Viewerbar destructor function
Viewerbar::~Viewerbar() {

}

//Viewerbar clear selection function
void Viewerbar::clearSelection() {
    tableBtn.deselect();
    treeBtn.deselect();
    matrixBtn.deselect();
    selection = NONE;
}

//Viewerbar hide function
void Viewerbar::hide(MainViewport* mainViewport) {
    mainViewport->removeSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
}

//Viewerbar open function
void Viewerbar::open(MainViewport* mainViewport) {
    //Position and size the window
    wndSize = { DEF_WND_WIDTH , mainViewport->workSize().height };
    wndPos = { mainViewport->workPosition().x, mainViewport->workPosition().y};
    mainViewport->addSidebar(wndSize.width, 0.0f, wndSize.width, 0.0f);
    wndFlags = NoNav | NoDecoration;

    ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
    ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));

    //Create window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
    if (create("##Viewerbar", wndFlags)) {
        selection = NONE;

        if (tableBtn.process()) {
            treeBtn.deselect();
            matrixBtn.deselect();
            selection = TABLE_BUTTON;
        }

        if (treeBtn.process()) {
            tableBtn.deselect();
            matrixBtn.deselect();
            selection = TREE_BUTTON;
        }

        if (matrixBtn.process()) {
            tableBtn.deselect();
            treeBtn.deselect();
            selection = MATRIX_BUTTON;
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}
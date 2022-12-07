/**********************************************************************************************************************/
//Artilab
//File: statusbar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Statusbar constructor function
Statusbar::Statusbar(Arti* a) {
    arti = a;
}

//Statusbar destructor function
Statusbar::~Statusbar() {
    arti = NULL;
}

//Statusbar open function
void Statusbar::open(MainViewport* mainViewport) {
    //Position and size the window
    wndSize = { mainViewport->workSize().width , 2.0f * ImGui::GetFontSize()};
    wndPos = { 0.0f, mainViewport->workSize().height - wndSize.height };
    mainViewport->addSidebar(0.0f, 0.0f, 0.0f, wndSize.height);
    wndFlags = NoNav | NoDecoration | NoInputs;

    ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
    ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));

    //Create window
    if (create("##Statusbar", wndFlags)) {
        if (arti != NULL) {
            if (!ImGui::BeginTable("##", 2, ImGuiTableFlags_BordersInnerV)) {
                ImGui::EndTable();
            }

            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, wndSize.width * 0.89f);
            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, wndSize.width * 0.11f);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::TableNextColumn();

            char buf[30] = {};
            snprintf(buf, 30, "Cycle: %zd ", arti->getCycle());
            ImGui::SetCursorPosX(wndSize.width - ImGui::CalcTextSize(buf).x - 5);
            ImGui::Text("%-30s", buf);

            ImGui::EndTable();
        }
        ImGui::End();
    }
}
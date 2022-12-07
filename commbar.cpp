/**********************************************************************************************************************/
//Artilab
//File: commbar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
ImVec4 Communicatorbar::ARTI_INPUT_COLOR = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
float Communicatorbar::DEF_WND_WIDTH = 320.0f;
ImVec4 Communicatorbar::USER_INPUT_COLOR = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

//Communicatorbar constructor function
Communicatorbar::Communicatorbar() :
    inputBuf{},
    log(std::stack<CommunicatorInput>()),
    communicate(false)
{

}

//Communicatorbar destructor function
Communicatorbar::~Communicatorbar() {
    for (int i = 0; i < log.size(); ++i)
        log.pop();
    communicate = false;
}

//Communicatorbar add log function
void Communicatorbar::addLog(bool user, const char* fmt, ...) {
    char buf[256];
    va_list args;

    va_start(args, fmt);

    vsnprintf(buf, 256, fmt, args);

    buf[255] = 0;

    va_end(args);

    log.emplace(CommunicatorInput(user, buf));
}

//Communicatorbar get communicate function
bool Communicatorbar::communicationPending() const {
    return communicate;
}

//Communicatorbar get log function
std::stack<CommunicatorInput> Communicatorbar::getLog() const {
    return log;
}

//Communicatorbar draw function
void Communicatorbar::open(MainViewport* mainViewport) {
    //Set window flags
    wndFlags = NoResize | NoCollapse;

    //Position and size the window
    wndSize = { mainViewport->workSize().width, 2.5f * ImGui::GetFontSize() };
    mainViewport->addSidebar(0.0f, wndSize.height, 0.0f, wndSize.height);
    wndPos = { 0.0f, 0.0f };
    wndFlags = NoNav | NoDecoration;

    ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
    ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));

    //Create window
    if (create("##Communicatorbar", wndFlags)) {
        //User's input
        bool reclaim_focus = false;
        ImGui::SetNextItemWidth(100.0f);
        ImGui::Text("Communicator: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(wndSize.width - 100.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, USER_INPUT_COLOR);
        if (!communicate && ImGui::InputText("##", inputBuf, 256, ImGuiInputTextFlags_EnterReturnsTrue, NULL, (void*)this))
        {
            addLog(true, inputBuf);

            for (int i = 0; i < 256; ++i)
                inputBuf[i] = 0;

            reclaim_focus = true;

            communicate = true;
        }
        ImGui::PopStyleColor();

        //Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); //Auto-focus previous widget

        ImGui::End();
    }
}

//Communicatorbar Arti responded function
void Communicatorbar::responded() {
    communicate = false;
}
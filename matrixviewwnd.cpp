/**********************************************************************************************************************/
//Artilab
//File: matrixviewwnd.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//MatrixViewWindow constructor function
MatrixViewWindow::MatrixViewWindow(Arti* a) {
	arti = a;
}

//MatrixViewWindow destructor function
MatrixViewWindow::~MatrixViewWindow() {
	arti = NULL;
}

//MatrixViewWindow open function
void MatrixViewWindow::open(MainViewport* mainViewport) {
    //Set window flags
    wndFlags = NoCollapse | Closable | NoResize | NoMove;

    //Position and size the window
    if (anchor || resizeWnd) {
        wndSize = { mainViewport->workSize().width, mainViewport->workSize().height };
        wndPos = { mainViewport->workPosition().x, mainViewport->workPosition().y };
        ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
        ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));
        anchor = false;
        resizeWnd = false;
    }

    //Create window
    if (create("Neural Matrix View", wndFlags)) {
        //Create options section
        ImGui::BeginChild("Option Region", ImVec2(wndSize.width, 30.0f), false, NULL);

        static std::vector<std::string> nmName = std::vector<std::string>(arti->neuralMatrices());
        static int selected = 0;

        for (int i = 0; i < nmName.size(); ++i)
            nmName[i] = arti->getNmName(i);

        //Create neural matrix selection combobox
        ImGui::Text("Neural Matrix: ");

        ImGui::SameLine();
        if (ImGui::BeginCombo("##NeuralMatrixCombo", nmName[selected].c_str(),
            ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
            for (int i = 0; i < nmName.size(); ++i) {
                const bool is_selected = (selected == i);

                if (ImGui::Selectable(nmName[i].c_str(), is_selected))
                    selected = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::EndChild();

        //Create neural matrix section
        ImGui::BeginChild("Neural Matrix Region", ImVec2(wndSize.width, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

        neuralmtxview* nmv = arti->neuralMatrix(nmName[selected]);
        int sensors = (int)*nmv->getSensoryInputs();
        int neurodes = (int)*nmv->getNeurodes();
        static float xHighRange = 0.0f;
        static float xLowRange = 0.0f;
        static float yHighRange = 0.0f;
        static float yLowRange = 0.0f;
        static float sHighRange = 0.0f;
        static float sLowRange = 0.0f;
        static float cHighRange = 0.0f;
        static float cLowRange = 0.0f;
        static float mHighRange = 0.0f;
        static float mLowRange = 0.0f;
        static float nHighRange = 0.0f;
        static float nLowRange = 0.0f;
        static ImU32 textColor = IM_COL32(255, 255, 255, 255);
        static ImU32 gridColor = IM_COL32(128, 128, 128, 32);
        static ImU32 yColor = IM_COL32(255, 0, 0, 0);
        static ImU32 xColor = IM_COL32(0, 255, 0, 0);
        static ImU32 sColor = IM_COL32(0, 0, 255, 0);
        static ImU32 mColor = IM_COL32(255, 0, 255, 0);
        static ImU32 nColor = IM_COL32(255, 255, 0, 0);
        static int alphaShift = 0;
        static float fontHeight = ImGui::GetFont()->FontSize;
        static float zoom = 100.0f;
        static float sqrSize = 10.0f;
        static ImVec2 origin = { 0.0f, 0.0f };
        static ImVec2 zoomed = { 0.0f, 0.0f };
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        //Handle zooming and panning
        if (ImGui::GetIO().MouseWheel != 0.0f) {
            zoom += ImGui::GetIO().MouseWheel * 10.0f;
            zoom = max(50.0f, zoom);
            zoomed.x = ((ImGui::GetMousePos().x / 2.0f) * (zoom / 100.0f)) - (ImGui::GetMousePos().x / 2.0f);
            zoomed.y = ((ImGui::GetMousePos().y / 2.0f) * (zoom / 100.0f)) - (ImGui::GetMousePos().y / 2.0f);
        }

        sqrSize = 10.0f * (zoom / 100.0f);

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            origin.x -= ImGui::GetMouseDragDelta().x;
            origin.y -= ImGui::GetMouseDragDelta().y;
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
        }

        //Draw grid labels
        drawList->AddText(
            ImVec2(pos.x + -origin.x - zoomed.x + ((sensors * sqrSize) / 2.0f) - (ImGui::CalcTextSize("Sensory Inputs").x / 2.0f), pos.y - origin.y - zoomed.y),
            textColor, "Sensory Inputs");

        drawList->AddText(
            ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 2.0f) * sqrSize) + ((neurodes * sqrSize) / 2.0f) - (ImGui::CalcTextSize("Neurode Outputs").x / 2.0f),
                pos.y - origin.y - zoomed.y),
            textColor, "Neurode Outputs");

        drawList->AddTextVertical(
            ImVec2(pos.x + -origin.x - zoomed.x - sqrSize - fontHeight, pos.y - origin.y - zoomed.y + fontHeight + ((neurodes + 2.0f) * sqrSize / 2.0f) +
                (ImGui::CalcTextSize("Neurode Inputs").x / 2.0f)),
            textColor, "Neurode Inputs");

        //Create sensory inputs heat map
        const float* iview = nmv->getSenXView().data();

        for (int i = 0; i < sensors; ++i) {
            if (iview[i * neurodes] > xHighRange)
                xHighRange = iview[i * neurodes];

            if (iview[i * neurodes] < yLowRange)
                xLowRange = iview[i * neurodes];
        }

        for (int i = 0; i < sensors; ++i) {
            drawList->AddRect(
                ImVec2(pos.x + -origin.x - zoomed.x + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + sqrSize),
                ImVec2(pos.x + -origin.x - zoomed.x + ((i + 1) * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 2.0f)),
                gridColor, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);

            alphaShift = (int)(((iview[i * neurodes] - xLowRange) / (xHighRange - xLowRange)) * 255) << IM_COL32_A_SHIFT;

            drawList->AddRectFilled(
                ImVec2(pos.x + -origin.x - zoomed.x + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + sqrSize),
                ImVec2(pos.x + -origin.x - zoomed.x + ((i + 1) * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 2.0f)),
                xColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
        }

        //Create neurode outputs heat map
        const float* yview = nmv->getYView().data();

        for (int i = 0; i < neurodes; ++i) {
            if (yview[i] > yHighRange)
                yHighRange = yview[i];

            if (yview[i] < yLowRange)
                yLowRange = yview[i];
        }

        for (int i = 0; i < neurodes; ++i) {
            alphaShift = (int)(((yview[i] - yLowRange) / (yHighRange - yLowRange)) * 255) << IM_COL32_A_SHIFT;

            drawList->AddRect(
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + sqrSize),
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 2.0f)),
                gridColor, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);

            drawList->AddRectFilled(
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + sqrSize),
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 2.0f)),
                yColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
        }

        //Create neurode sensory inputs heat map
        const float* xview = nmv->getSenSwView().data();

        for (int i = 0; i < sensors; ++i) {
            for (int j = 0; j < neurodes; ++j) {
                if (xview[j + (i * neurodes)] > xHighRange)
                    xHighRange = xview[j + (i * neurodes)];

                if (xview[j + (i * neurodes)] < xLowRange)
                    xLowRange = xview[j + (i * neurodes)];
            }
        }

        for (int i = 0; i < sensors; ++i) {
            for (int j = 0; j < neurodes; ++j) {
                drawList->AddRect(
                    ImVec2(pos.x + -origin.x - zoomed.x + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                    ImVec2(pos.x + -origin.x - zoomed.x + ((i + 1) * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                    gridColor, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);

                alphaShift = (int)(((xview[j + (i * neurodes)] - xLowRange) / (xHighRange - xLowRange)) * 255) << IM_COL32_A_SHIFT;

                drawList->AddRectFilled(
                    ImVec2(pos.x + -origin.x - zoomed.x + (i * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                    ImVec2(pos.x + -origin.x - zoomed.x + ((i + 1) * sqrSize), pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                    xColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
            }
        }

        //Create neurode input sum heat map
        const float* sview = nmv->getSView().data();

        for (int i = 0; i < neurodes; ++i) {
            if (sview[i] > sHighRange)
                sHighRange = sview[i];

            if (sview[i] < sLowRange)
                sLowRange = sview[i];
        }

        for (int i = 0; i < neurodes; ++i) {
            drawList->AddRect(
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 1.0f) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (i * sqrSize)),
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 2.0f) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((i + 1) * sqrSize)),
                gridColor, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);

            alphaShift = (int)(((sview[i] - sLowRange) / (sHighRange - sLowRange)) * 255) << IM_COL32_A_SHIFT;

            drawList->AddRectFilled(
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 1.0f) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (i * sqrSize)),
                ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 2.0f) * sqrSize), 
                    pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((i + 1) * sqrSize)),
                xColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
        }

        //Create connections heat map
        const float* cview = nmv->getSynSwView().data();
        const float* mview = nmv->getModSwView().data();
        const float* nview = nmv->getNrdXwView().data();

        for (int i = 0; i < neurodes; ++i) {
            for (int j = 0; j < neurodes; ++j) {
                if (cview[j + (i * neurodes)] > cHighRange)
                    cHighRange = cview[j + (i * neurodes)];

                if (mview[j + (i * neurodes)] > mHighRange)
                    mHighRange = mview[j + (i * neurodes)];

                if (nview[j + (i * neurodes)] > nHighRange)
                    nHighRange = nview[j + (i * neurodes)];

                if (cview[j + (i * neurodes)] < cLowRange)
                    cLowRange = cview[j + (i * neurodes)];

                if (mview[j + (i * neurodes)] < mLowRange)
                    mLowRange = mview[j + (i * neurodes)];

                if (nview[j + (i * neurodes)] < nLowRange)
                    nLowRange = nview[j + (i * neurodes)];
            }
        }

        for (int i = 0; i < neurodes; ++i) {
            for (int j = 0; j < neurodes; ++j) {
                drawList->AddRect(
                    ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), 
                        pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                    ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize),
                        pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                    gridColor, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);

                if (cview[j + (i * neurodes)] > cLowRange) {
                    alphaShift = (int)(((cview[j + (i * neurodes)] - cLowRange) / (cHighRange - cLowRange)) * 255) << IM_COL32_A_SHIFT;

                    drawList->AddRectFilled(
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), 
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize),
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                        sColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
                }

                if (mview[j + (i * neurodes)] > mLowRange) {
                    alphaShift = (int)(((mview[j + (i * neurodes)] - mLowRange) / (mHighRange - mLowRange)) * 255) << IM_COL32_A_SHIFT;

                    drawList->AddRectFilled(
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), 
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize),
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                        mColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
                }

                if (nview[j + (i * neurodes)] > nLowRange) {
                    alphaShift = (int)(((nview[j + (i * neurodes)] - nLowRange) / (nHighRange - nLowRange)) * 255) << IM_COL32_A_SHIFT;

                    drawList->AddRectFilled(
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + (i * sqrSize), 
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + (j * sqrSize)),
                        ImVec2(pos.x + -origin.x - zoomed.x + ((sensors + 3.0f) * sqrSize) + ((i + 1) * sqrSize),
                            pos.y - origin.y - zoomed.y + fontHeight + (sqrSize * 3.0f) + ((j + 1) * sqrSize)),
                        nColor | alphaShift, 1.0f * (sqrSize / 10.f), ImDrawFlags_RoundCornersAll);
                }
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
}
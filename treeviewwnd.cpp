/**********************************************************************************************************************/
//Artilab
//File: treeviewwnd.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//TreeViewWindow constructor function
TreeViewWindow::TreeViewWindow(Arti* a) :
    arti(a),
    ntree((arti != NULL) ? arti->neuralTree() : NULL),
    treeFlags(ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick),
    tableFlags(ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoSavedSettings)
{

}

//TreeViewWindow destructor function
TreeViewWindow::~TreeViewWindow() {
    arti = NULL;
    ntree = NULL;
    treeFlags = 0;
    tableFlags = 0;
}

//TreeViewWindow open function
void TreeViewWindow::open(MainViewport* mainViewport) {
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
    if (create("Neural Tree View", wndFlags)) {
        //Arti's tree data variables
        std::vector<std::string> nmName;
        const NeuralMatrixNode* nmNode;
        neuralmtxview* nmview;
        ImGuiTreeNodeFlags leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        static int senTable = -1;
        static int nrdTable = -1;
        static int nrdcTable = -1;
        static int modTable = -1;
        static int synTable = -1;
        std::string tNmName = "";
        std::string tGrpName = "";
        std::string tNnName = "";
        std::string tNrnName = "";
        std::string tBrchName = "";
        size_t tSenIdx = 0;
        size_t tNrdIdx = 0;
        size_t tIndex = 0;

        //Create tree view of Arti
        ImGui::BeginChild("Tree Region", ImVec2(wndSize.width / 4.0f, 0.0f), false, NULL);

        if (arti != NULL && ntree != NULL && ImGui::TreeNodeEx((void*)(intptr_t)0, treeFlags, "Arti")) {
            //Create neural matrix trees
            nmName = ntree->neuralMatrixNames();

            for (int i = 0; i < nmName.size(); ++i) {
                nmNode = ntree->neuralMatrixNode(nmName[i]);
                nmview = nmNode->nmv;

                if (ImGui::TreeNodeEx((void*)(intptr_t)(i + 1), treeFlags, nmName[i].c_str())) {
                    std::string nmsxname = (nmName[i] + " Sensory Inputs");

                    //Create neural matrix's sensory inputs tree
                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size()), treeFlags, nmsxname.c_str())) {
                        for (int j = 0; j < nmNode->sen.size(); ++j) {
                            bool senOpen = ImGui::TreeNodeEx((void*)(intptr_t)j, treeFlags,
                                arti->getSenName(nmNode->sen[j].index, nmName[i]).c_str());

                            if (senOpen) {
                                mtxview w = nmview->getSenWView().row(nmNode->sen[j].index);

                                for (int k = 0; k < (int)nmNode->sen[j].nrdIndex.size(); ++k) {
                                    if (w[nmNode->sen[j].nrdIndex[k]] != 0.0f) {
                                        ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), leafFlags, arti->getNrdName(nmNode->sen[j].nrdIndex[k], nmName[i]).c_str());

                                        if (ImGui::IsItemClicked() || senTable == k) {
                                            senTable = k;
                                            tNmName = nmName[i];
                                            tGrpName = "";
                                            tSenIdx = nmNode->sen[j].index;
                                            tIndex = nmNode->sen[j].nrdIndex[k];

                                            nrdTable = -1;
                                            nrdcTable = -1;
                                            modTable = -1;
                                            synTable = -1;
                                        }
                                    }
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's sensory input groups tree
                    std::string nmsgname = (nmName[i] + " Sensory Input Groups");

                    if (ImGui::TreeNodeEx((void*)((i + 1) + nmName.size() * 2), treeFlags, nmsgname.c_str())) {
                        for (int j = 0; j < nmNode->seng.size(); ++j) {
                            std::string sgsxname = (nmNode->seng[j].name + " Sensory Inputs");

                            //Create sensory group's sensory inputs tree
                            if (ImGui::TreeNodeEx((void*)(intptr_t)(j + 1), treeFlags, sgsxname.c_str())) {
                                for (int k = 0; k < nmNode->seng[j].sen.size(); ++k) {
                                    bool senOpen = ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, 
                                        arti->getSenName(nmNode->seng[j].sen[k].index, nmName[i]).c_str());

                                    if (senOpen) {
                                        mtxview w = nmview->getSenWView().row(nmNode->seng[j].sen[k].index);

                                        for (int m = 0; m < (int)nmNode->seng[j].sen[k].nrdIndex.size(); ++m) {
                                            if (w[nmNode->seng[j].sen[k].nrdIndex[m]] != 0.0f) {
                                                ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), leafFlags, 
                                                    arti->getNrdName(nmNode->seng[j].sen[k].nrdIndex[m], nmName[i]).c_str());

                                                if (ImGui::IsItemClicked() || senTable == m) {
                                                    senTable = m;
                                                    tNmName = nmName[i];
                                                    tGrpName = nmNode->seng[j].name;
                                                    tSenIdx = nmNode->seng[j].sen[k].index;
                                                    tIndex = nmNode->seng[j].sen[k].nrdIndex[m];

                                                    nrdTable = -1;
                                                    nrdcTable = -1;
                                                    modTable = -1;
                                                    synTable = -1;
                                                }
                                            }
                                        }

                                        ImGui::TreePop();
                                    }
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's neurode tree
                    std::string nmnrdname = (nmName[i] + " Neurodes");

                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 3), treeFlags, nmnrdname.c_str())) {
                        for (int j = 0; j < nmNode->nrd.size(); ++j) {
                            bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(j + 1), treeFlags, 
                                arti->getNrdName(nmNode->nrd[j].index, nmName[i]).c_str());

                            if (nrdOpen) {
                                //Create neurode data tree
                                ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrd.size()), leafFlags, "Neurode");

                                if (ImGui::IsItemClicked() || nrdTable == j) {
                                    nrdTable = j;
                                    tNmName = nmName[i];
                                    tNnName = "";
                                    tNrnName = "";
                                    tBrchName = "";
                                    tIndex = (int)nmNode->nrd[j].index;

                                    senTable = -1;
                                    nrdcTable = -1;
                                    modTable = -1;
                                    synTable = -1;
                                }

                                //Create neurode connections tree
                                bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrd.size() * 2), treeFlags,
                                    (arti->getNrdName(nmNode->nrd[j].index, nmName[i]) + " Neurode Connections").c_str());

                                if (nrdConnOpen) {
                                    mtxview nw = nmview->getNrdWView().row(nmNode->nrd[j].index);

                                    for (int k = 0; k < (int)nmNode->nrd[j].nrdIndex.size(); ++k) {
                                        if (nw[nmNode->nrd[j].nrdIndex[k]] != 0.0f) {
                                            ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), leafFlags, 
                                                arti->getNrdName(nmNode->nrd[j].nrdIndex[k], nmName[i]).c_str());
                                            
                                            if (ImGui::IsItemClicked() || nrdcTable == k) {
                                                nrdcTable = k;
                                                tNmName = nmName[i];
                                                tNnName = "";
                                                tNrnName = "";
                                                tBrchName = "";
                                                tNrdIdx = nmNode->nrd[j].index;
                                                tIndex = nmNode->nrd[j].nrdIndex[k];

                                                senTable = -1;
                                                nrdTable = -1;
                                                modTable = -1;
                                                synTable = -1;
                                            }
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                //Create modulatory connections tree
                                bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrd.size() * 3), treeFlags,
                                    (arti->getNrdName(nmNode->nrd[j].index, nmName[i]) + " Modulatory Connections").c_str());

                                if (modConnOpen) {
                                    mtxview mw = nmview->getModWView().row(nmNode->nrd[j].index);

                                    for (int k = 0; k < (int)nmNode->nrd[j].modIndex.size(); ++k) {
                                        if (mw[nmNode->nrd[j].modIndex[k]] != 0.0f) {
                                            ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrd[j].nrdIndex.size()), leafFlags,
                                                arti->getNrdName(nmNode->nrd[j].modIndex[k], nmName[i]).c_str());
                                            
                                            if (ImGui::IsItemClicked() || modTable == k) {
                                                modTable = k;
                                                tNmName = nmName[i];
                                                tNnName = "";
                                                tNrnName = "";
                                                tBrchName = "";
                                                tNrdIdx = nmNode->nrd[j].index;
                                                tIndex = nmNode->nrd[j].modIndex[k];

                                                senTable = -1;
                                                nrdTable = -1;
                                                nrdcTable = -1;
                                                synTable = -1;
                                            }
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                //Create synaptic connections tree
                                bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrd.size() * 4), treeFlags,
                                    (arti->getNrdName(nmNode->nrd[j].index, nmName[i]) + " Synaptic Connections").c_str());

                                if (synConnOpen) {
                                    mtxview sw = nmview->getSynWView().row(nmNode->nrd[j].index);

                                    for (int k = 0; k < (int)nmNode->nrd[j].synIndex.size(); ++k) {
                                        if (sw[nmNode->nrd[j].synIndex[k]] != 0.0f) {
                                            ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrd[j].nrdIndex.size() * 2), leafFlags,
                                                arti->getNrdName(nmNode->nrd[j].synIndex[k], nmName[i]).c_str());

                                            if (ImGui::IsItemClicked() || synTable == k) {
                                                synTable = k;
                                                tNmName = nmName[i];
                                                tNnName = "";
                                                tNrnName = "";
                                                tBrchName = "";
                                                tNrdIdx = nmNode->nrd[j].index;
                                                tIndex = nmNode->nrd[j].synIndex[k];

                                                senTable = -1;
                                                nrdTable = -1;
                                                nrdcTable = -1;
                                                modTable = -1;
                                            }
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's dendrite tree
                    std::string nmdendname = (nmName[i] + " Dendrite Branches");

                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 4), treeFlags, nmdendname.c_str())) {
                        for (int j = 0; j < nmNode->dend.size(); ++j) {
                            if (ImGui::TreeNodeEx((void*)(intptr_t)(j + 1), treeFlags, nmNode->dend[j].name.c_str())) {
                                for (int k = 0; k < nmNode->dend[j].nrd.size(); ++k) {
                                    bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, 
                                        arti->getNrdName(nmNode->dend[j].nrd[k].index, nmName[i]).c_str());

                                    if (nrdOpen) {
                                        //Create neurode data tree
                                        ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->dend[j].nrd.size()), leafFlags, "Neurode");

                                        if (ImGui::IsItemClicked() || nrdTable == k) {
                                            nrdTable = k;
                                            tNmName = nmName[i];
                                            tNnName = "";
                                            tNrnName = "";
                                            tBrchName = nmNode->dend[j].name;
                                            tIndex = nmNode->dend[j].nrd[k].index;

                                            senTable = -1;
                                            nrdcTable = -1;
                                            modTable = -1;
                                            synTable = -1;
                                        }

                                        //Create neurode connections tree
                                        bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->dend[j].nrd.size() * 2), treeFlags,
                                            (arti->getNrdName(nmNode->dend[j].nrd[k].index, nmName[i]) + " Neurode Connections").c_str());

                                        if (nrdConnOpen) {
                                            mtxview nw = nmview->getNrdWView().row(nmNode->dend[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->dend[j].nrd[k].nrdIndex.size(); ++m) {
                                                if (nw[nmNode->dend[j].nrd[k].nrdIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), leafFlags, 
                                                        arti->getNrdName(nmNode->dend[j].nrd[k].nrdIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || nrdcTable == m) {
                                                        nrdcTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->dend[j].name;
                                                        tNrdIdx = nmNode->dend[j].nrd[k].index;
                                                        tIndex = nmNode->dend[j].nrd[k].nrdIndex[m];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        modTable = -1;
                                                        synTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create modulatory connections tree
                                        bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->dend[j].nrd.size() * 3), treeFlags,
                                            (arti->getNrdName(nmNode->dend[j].nrd[k].index, nmName[i]) + " Modulatory Connections").c_str());

                                        if (modConnOpen) {
                                            mtxview mw = nmview->getModWView().row(nmNode->dend[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->dend[j].nrd[k].modIndex.size(); ++m) {
                                                if (mw[nmNode->dend[j].nrd[k].modIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->dend[j].nrd[k].nrdIndex.size()), leafFlags,
                                                        arti->getNrdName(nmNode->dend[j].nrd[k].modIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || modTable == m) {
                                                        modTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->dend[j].name;
                                                        tNrdIdx = nmNode->dend[j].nrd[k].index;
                                                        tIndex = nmNode->dend[j].nrd[k].modIndex[m];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        nrdcTable = -1;
                                                        synTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create synaptic connections tree
                                        bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->dend[j].nrd.size() * 4), treeFlags,
                                            (arti->getNrdName(nmNode->dend[j].nrd[k].index, nmName[i]) + " Synaptic Connections").c_str());

                                        if (synConnOpen) {
                                            mtxview sw = nmview->getSynWView().row(nmNode->dend[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->dend[j].nrd[k].synIndex.size(); ++m) {
                                                if (sw[nmNode->dend[j].nrd[k].synIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->dend[j].nrd[k].nrdIndex.size() + 
                                                        nmNode->dend[j].nrd[k].modIndex.size()), leafFlags,
                                                        arti->getNrdName(nmNode->dend[j].nrd[k].synIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || synTable == m) {
                                                        synTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->dend[j].name;
                                                        tNrdIdx = nmNode->dend[j].nrd[k].index;
                                                        tIndex = nmNode->dend[j].nrd[k].synIndex[k];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        nrdcTable = -1;
                                                        modTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        ImGui::TreePop();
                                    }
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's axon tree
                    std::string nmaxonname = (nmName[i] + " Axon Branches");

                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 5), treeFlags, nmaxonname.c_str())) {
                        for (int j = 0; j < nmNode->axon.size(); ++j) {
                            if (ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->dend.size()), treeFlags, nmNode->axon[j].name.c_str())) {
                                for (int k = 0; k < nmNode->axon[j].nrd.size(); ++k) {
                                    bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, 
                                        arti->getNrdName(nmNode->axon[j].nrd[k].index, nmName[i]).c_str());

                                    if (nrdOpen) {
                                        //Create neurode data tree
                                        ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), leafFlags, "Neurode");

                                        if (ImGui::IsItemClicked() || nrdTable == k) {
                                            nrdTable = k;
                                            tNmName = nmName[i];
                                            tNnName = "";
                                            tNrnName = "";
                                            tBrchName = nmNode->axon[j].name;
                                            tIndex = nmNode->axon[j].nrd[k].index;

                                            senTable = -1;
                                            nrdcTable = -1;
                                            modTable = -1;
                                            synTable = -1;
                                        }

                                        //Create neurode connections tree
                                        bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->axon[j].nrd[k].nrdIndex.size() * 2), treeFlags,
                                            (arti->getNrdName(nmNode->axon[j].nrd[k].index, nmName[i]) + " Neurode Connections").c_str());

                                        if (nrdConnOpen) {
                                            mtxview nw = nmview->getNrdWView().row(nmNode->axon[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->axon[j].nrd[k].nrdIndex.size(); ++m) {
                                                if (nw[nmNode->axon[j].nrd[k].nrdIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), leafFlags, 
                                                        arti->getNrdName(nmNode->axon[j].nrd[k].nrdIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || nrdcTable == m) {
                                                        nrdcTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->axon[j].name;
                                                        tNrdIdx = nmNode->axon[j].nrd[k].index;
                                                        tIndex = nmNode->axon[j].nrd[k].nrdIndex[m];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        modTable = -1;
                                                        synTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create modulatory connections tree
                                        bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->axon[j].nrd.size() * 3), treeFlags,
                                            (arti->getNrdName(nmNode->axon[j].nrd[k].index, nmName[i]) + " Modulatory Connections").c_str());

                                        if (modConnOpen) {
                                            mtxview mw = nmview->getModWView().row(nmNode->axon[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->axon[j].nrd[k].modIndex.size(); ++m) {
                                                if (mw[nmNode->axon[j].nrd[k].modIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->axon[j].nrd[k].nrdIndex.size()), leafFlags,
                                                        arti->getNrdName(nmNode->axon[j].nrd[k].modIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || modTable == m) {
                                                        modTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->axon[j].name;
                                                        tNrdIdx = nmNode->axon[j].nrd[k].index;
                                                        tIndex = nmNode->axon[j].nrd[k].modIndex[m];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        nrdcTable = -1;
                                                        synTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create synaptic connections tree
                                        bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->axon[j].nrd.size() * 4), treeFlags,
                                            (arti->getNrdName(nmNode->axon[j].nrd[k].index, nmName[i]) + " Synaptic Connections").c_str());

                                        if (synConnOpen) {
                                            mtxview sw = nmview->getSynWView().row(nmNode->axon[j].nrd[k].index);

                                            for (int m = 0; m < (int)nmNode->axon[j].nrd[k].synIndex.size(); ++m) {
                                                if (sw[nmNode->axon[j].nrd[k].synIndex[m]] != 0.0f) {
                                                    ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->axon[j].nrd[k].nrdIndex.size() + 
                                                        nmNode->axon[j].nrd[k].modIndex.size()), leafFlags,
                                                        arti->getNrdName(nmNode->axon[j].nrd[k].synIndex[m], nmName[i]).c_str());

                                                    if (ImGui::IsItemClicked() || synTable == m) {
                                                        synTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = "";
                                                        tBrchName = nmNode->axon[j].name;
                                                        tNrdIdx = nmNode->axon[j].nrd[k].index;
                                                        tIndex = nmNode->axon[j].nrd[k].synIndex[m];

                                                        senTable = -1;
                                                        nrdTable = -1;
                                                        nrdcTable = -1;
                                                        modTable = -1;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        ImGui::TreePop();
                                    }
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's neuron tree
                    std::string nmnrnname = (nmName[i] + " Neurons");

                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 6), treeFlags, nmnrnname.c_str())) {
                        for (int j = 0; j < nmNode->nrn.size(); ++j) {
                            if (ImGui::TreeNodeEx((void*)(intptr_t)(j + 1), treeFlags, nmNode->nrn[j].name.c_str())) {
                                bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(j + 1), treeFlags, 
                                    arti->getNrdName(nmNode->nrn[j].soma->index, nmName[i]).c_str());

                                if (nrdOpen) {
                                    //Create neurode data tree
                                    ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 1), leafFlags, "Neurode");

                                    if (ImGui::IsItemClicked() || nrdTable == j) {
                                        nrdTable = j;
                                        tNmName = nmName[i];
                                        tNnName = "";
                                        tNrnName = nmNode->nrn[j].name;
                                        tBrchName = "";
                                        tIndex = nmNode->nrn[j].soma->index;

                                        senTable = -1;
                                        nrdcTable = -1;
                                        modTable = -1;
                                        synTable = -1;
                                    }

                                    //Create neurode connections tree
                                    bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 2), treeFlags,
                                        (arti->getNrdName(nmNode->nrn[j].soma->index, nmName[i]) + " Neurode Connections").c_str());

                                    if (nrdConnOpen) {
                                        mtxview nw = nmview->getNrdWView().row(nmNode->nrn[j].soma->index);

                                        for (int k = 0; k < (int)nmNode->nrn[j].soma->nrdIndex.size(); ++k) {
                                            if (nw[nmNode->nrn[j].soma->nrdIndex[k]] != 0.0f) {
                                                ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), leafFlags, 
                                                    arti->getNrdName(nmNode->nrn[j].soma->nrdIndex[k], nmName[i]).c_str());

                                                if (ImGui::IsItemClicked() || nrdcTable == k) {
                                                    nrdcTable = k;
                                                    tNmName = nmName[i];
                                                    tNnName = "";
                                                    tNrnName = nmNode->nrn[j].name;
                                                    tBrchName = "";
                                                    tNrdIdx = nmNode->nrn[j].soma->index;
                                                    tIndex = nmNode->nrn[j].soma->nrdIndex[k];

                                                    senTable = -1;
                                                    nrdTable = -1;
                                                    modTable = -1;
                                                    synTable = -1;
                                                }
                                            }
                                        }

                                        ImGui::TreePop();
                                    }

                                    //Create modulatory connections tree
                                    bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 3), treeFlags,
                                        (arti->getNrdName(nmNode->nrn[j].soma->index, nmName[i]) + " Modulatory Connections").c_str());

                                    if (modConnOpen) {
                                        mtxview mw = nmview->getModWView().row(nmNode->nrn[j].soma->index);

                                        for (int k = 0; k < (int)nmNode->nrn[j].soma->modIndex.size(); ++k) {
                                            if (mw[nmNode->nrn[j].soma->modIndex[k]] != 0.0f) {
                                                ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrn[j].soma->nrdIndex.size()), leafFlags, 
                                                    arti->getNrdName(nmNode->nrn[j].soma->modIndex[k], nmName[i]).c_str());

                                                if (ImGui::IsItemClicked() || modTable == k) {
                                                    modTable = k;
                                                    tNmName = nmName[i];
                                                    tNnName = "";
                                                    tNrnName = nmNode->nrn[j].name;
                                                    tBrchName = "";
                                                    tNrdIdx = nmNode->nrn[j].soma->index;
                                                    tIndex = nmNode->nrn[j].soma->modIndex[k];

                                                    senTable = -1;
                                                    nrdTable = -1;
                                                    nrdcTable = -1;
                                                    synTable = -1;
                                                }
                                            }
                                        }

                                        ImGui::TreePop();
                                    }

                                    //Create synaptic connections tree
                                    bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 4), treeFlags,
                                        (arti->getNrdName(nmNode->nrn[j].soma->index, nmName[i]) + " Synaptic Connections").c_str());

                                    if (synConnOpen) {
                                        mtxview sw = nmview->getSynWView().row(nmNode->nrn[j].soma->index);

                                        for (int k = 0; k < (int)nmNode->nrn[j].soma->synIndex.size(); ++k) {
                                            if (sw[nmNode->nrn[j].soma->synIndex[k]] != 0.0f) {
                                                ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrn[j].soma->nrdIndex.size() + 
                                                    nmNode->nrn[j].soma->modIndex.size()), leafFlags,
                                                    arti->getNrdName(nmNode->nrn[j].soma->synIndex[k], nmName[i]).c_str());

                                                if (ImGui::IsItemClicked() || synTable == k) {
                                                    synTable = k;
                                                    tNmName = nmName[i];
                                                    tNnName = "";
                                                    tNrnName = nmNode->nrn[j].name;
                                                    tBrchName = "";
                                                    tNrdIdx = nmNode->nrn[j].soma->index;
                                                    tIndex = nmNode->nrn[j].soma->synIndex[k];

                                                    senTable = -1;
                                                    nrdTable = -1;
                                                    nrdcTable = -1;
                                                    modTable = -1;
                                                }
                                            }
                                        }

                                        ImGui::TreePop();
                                    }

                                    ImGui::TreePop();
                                }

                                //Create neuron's dendrite tree
                                std::string nrndendname = (nmNode->nrn[j].name + " Dendrite Branches");

                                if (ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrn.size() * 5), treeFlags, nrndendname.c_str())) {
                                    for (int k = 0; k < nmNode->nrn[j].dend.size(); ++k) {
                                        if (ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, nmNode->nrn[j].dend[k].name.c_str())) {
                                            for (int m = 0; m < nmNode->nrn[j].dend[k].nrd.size(); ++m) {
                                                bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), treeFlags, 
                                                    arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].index, nmName[i]).c_str());

                                                if (nrdOpen) {
                                                    //Create neurode data tree
                                                    ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].dend[k].nrd.size() + 1), leafFlags, "Neurode");

                                                    if (ImGui::IsItemClicked() || nrdTable == m) {
                                                        nrdTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = nmNode->nrn[j].name;
                                                        tBrchName = nmNode->nrn[j].dend[k].name;
                                                        tIndex = nmNode->nrn[j].dend[k].nrd[m].index;

                                                        senTable = -1;
                                                        nrdcTable = -1;
                                                        modTable = -1;
                                                        synTable = -1;
                                                    }

                                                    //Create neurode connections tree
                                                    bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].dend[k].nrd.size() + 2), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].index, nmName[i]) + " Neurode Connections").c_str());

                                                    if (nrdConnOpen) {
                                                        mtxview nw = nmview->getNrdWView().row(nmNode->nrn[j].dend[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].dend[k].nrd[m].nrdIndex.size(); ++n) {
                                                            if (nw[nmNode->nrn[j].dend[k].nrd[m].nrdIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)(n + 1), leafFlags, 
                                                                    arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].nrdIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || nrdcTable == n) {
                                                                    nrdcTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].dend[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].dend[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].dend[k].nrd[m].nrdIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    modTable = -1;
                                                                    synTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    //Create modulatory connections tree
                                                    bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].dend[k].nrd.size() + 3), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].index, nmName[i]) + " Modulatory Connections").c_str());

                                                    if (modConnOpen) {
                                                        mtxview mw = nmview->getModWView().row(nmNode->nrn[j].dend[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].dend[k].nrd[m].modIndex.size(); ++n) {
                                                            if (mw[nmNode->nrn[j].dend[k].nrd[m].modIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)((n + 1) + nmNode->nrn[j].dend[k].nrd[m].nrdIndex.size()), 
                                                                    leafFlags,
                                                                    arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].modIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || modTable == n) {
                                                                    modTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].dend[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].dend[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].dend[k].nrd[m].modIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    nrdcTable = -1;
                                                                    synTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    //Create synaptic connections tree
                                                    bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].dend[k].nrd.size() + 4), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].index, nmName[i]) + " Synaptic Connections").c_str());

                                                    if (synConnOpen) {
                                                        mtxview sw = nmview->getSynWView().row(nmNode->nrn[j].dend[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].dend[k].nrd[m].synIndex.size(); ++n) {
                                                            if (sw[nmNode->nrn[j].dend[k].nrd[m].synIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)((n + 1) + nmNode->nrn[j].dend[k].nrd[m].nrdIndex.size() +
                                                                    nmNode->nrn[j].dend[k].nrd[m].modIndex.size()), leafFlags,
                                                                    arti->getNrdName(nmNode->nrn[j].dend[k].nrd[m].synIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || synTable == n) {
                                                                    synTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].dend[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].dend[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].dend[k].nrd[m].synIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    nrdcTable = -1;
                                                                    modTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    ImGui::TreePop();
                                                }
                                            }

                                            ImGui::TreePop();
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                //Create neuron's axon tree
                                std::string nrnaxonname = (nmNode->nrn[j].name + " Axon Branches");

                                if (ImGui::TreeNodeEx((void*)(intptr_t)((j + 1) + nmNode->nrn.size() * 6), treeFlags, nrnaxonname.c_str())) {
                                    for (int k = 0; k < nmNode->nrn[j].axon.size(); ++k) {
                                        if (ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, nmNode->nrn[j].axon[k].name.c_str())) {
                                            for (int m = 0; m < nmNode->nrn[j].axon[k].nrd.size(); ++m) {
                                                bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), treeFlags,
                                                    arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].index, nmName[i]).c_str());

                                                if (nrdOpen) {
                                                    //Create neurode data tree
                                                    ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].axon[k].nrd.size() + 1), leafFlags, "Neurode");

                                                    if (ImGui::IsItemClicked() || nrdTable == m) {
                                                        nrdTable = m;
                                                        tNmName = nmName[i];
                                                        tNnName = "";
                                                        tNrnName = nmNode->nrn[j].name;
                                                        tBrchName = nmNode->nrn[j].axon[k].name;
                                                        tIndex = nmNode->nrn[j].axon[k].nrd[m].index;

                                                        senTable = -1;
                                                        nrdcTable = -1;
                                                        modTable = -1;
                                                        synTable = -1;
                                                    }

                                                    //Create neurode connections tree
                                                    bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].axon[k].nrd.size() + 2), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].index, nmName[i]) + " Neurode Connections").c_str());

                                                    if (nrdConnOpen) {
                                                        mtxview nw = nmview->getNrdWView().row(nmNode->nrn[j].axon[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].axon[k].nrd[m].nrdIndex.size(); ++n) {
                                                            if (nw[nmNode->nrn[j].axon[k].nrd[m].nrdIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)(n + 1), leafFlags,
                                                                    arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].nrdIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || nrdcTable == n) {
                                                                    nrdcTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].axon[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].axon[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].axon[k].nrd[m].nrdIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    modTable = -1;
                                                                    synTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    //Create modulatory connections tree
                                                    bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].axon[k].nrd.size() + 3), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].index, nmName[i]) + " Modulatory Connections").c_str());

                                                    if (modConnOpen) {
                                                        mtxview mw = nmview->getModWView().row(nmNode->nrn[j].axon[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].axon[k].nrd[m].modIndex.size(); ++n) {
                                                            if (mw[nmNode->nrn[j].axon[k].nrd[m].modIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)((n + 1) + nmNode->nrn[j].axon[k].nrd[m].nrdIndex.size()),
                                                                    leafFlags,
                                                                    arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].modIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || modTable == n) {
                                                                    modTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].axon[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].axon[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].axon[k].nrd[m].modIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    nrdcTable = -1;
                                                                    synTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    //Create synaptic connections tree
                                                    bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn[j].axon[k].nrd.size() + 4), treeFlags,
                                                        (arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].index, nmName[i]) + " Synaptic Connections").c_str());

                                                    if (synConnOpen) {
                                                        mtxview sw = nmview->getSynWView().row(nmNode->nrn[j].axon[k].nrd[m].index);

                                                        for (int n = 0; n < (int)nmNode->nrn[j].axon[k].nrd[m].synIndex.size(); ++n) {
                                                            if (sw[nmNode->nrn[j].axon[k].nrd[m].synIndex[n]] != 0.0f) {
                                                                ImGui::TreeNodeEx((void*)(intptr_t)((n + 1) + nmNode->nrn[j].axon[k].nrd[m].nrdIndex.size() +
                                                                    nmNode->nrn[j].axon[k].nrd[m].modIndex.size()), leafFlags,
                                                                    arti->getNrdName(nmNode->nrn[j].axon[k].nrd[m].synIndex[n], nmName[i]).c_str());

                                                                if (ImGui::IsItemClicked() || synTable == n) {
                                                                    synTable = n;
                                                                    tNmName = nmName[i];
                                                                    tNnName = "";
                                                                    tNrnName = nmNode->nrn[j].name;
                                                                    tBrchName = nmNode->nrn[j].axon[k].name;
                                                                    tNrdIdx = nmNode->nrn[j].axon[k].nrd[m].index;
                                                                    tIndex = nmNode->nrn[j].axon[k].nrd[m].synIndex[n];

                                                                    senTable = -1;
                                                                    nrdTable = -1;
                                                                    nrdcTable = -1;
                                                                    modTable = -1;
                                                                }
                                                            }
                                                        }

                                                        ImGui::TreePop();
                                                    }

                                                    ImGui::TreePop();
                                                }
                                            }

                                            ImGui::TreePop();
                                        }
                                    }

                                    ImGui::TreePop();
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    //Create neural matrix's neural net tree
                    std::string nmnetname = (nmName[i] + " Neural Nets");

                    if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 7), treeFlags, nmnetname.c_str())) {
                        for (int j = 0; j < nmNode->net.size(); ++j) {
                            //Create neural net's neuron tree
                            std::string nnnrnname = (nmNode->net[j].name + " Neurons");

                            if (ImGui::TreeNodeEx((void*)(intptr_t)((i + 1) + nmName.size() * 8), treeFlags, nmnrnname.c_str())) {
                                for (int k = 0; k < nmNode->nrn.size(); ++k) {
                                    if (ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags, nmNode->net[j].nrn[k].name.c_str())) {
                                        bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(k + 1), treeFlags,
                                            arti->getNrdName(nmNode->net[j].nrn[k].soma->index, nmName[i]).c_str());

                                        if (nrdOpen) {
                                            //Create neurode data tree
                                            ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 1), leafFlags, "Neurode");

                                            if (ImGui::IsItemClicked() || nrdTable == k) {
                                                nrdTable = k;
                                                tNmName = nmName[i];
                                                tNnName = "";
                                                tNrnName = nmNode->net[j].nrn[k].name;
                                                tBrchName = "";
                                                tIndex = nmNode->net[j].nrn[k].soma->index;

                                                senTable = -1;
                                                nrdcTable = -1;
                                                modTable = -1;
                                                synTable = -1;
                                            }

                                            //Create neurode connections tree
                                            bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 2), treeFlags,
                                                (arti->getNrdName(nmNode->net[j].nrn[k].soma->index, nmName[i]) + " Neurode Connections").c_str());

                                            if (nrdConnOpen) {
                                                mtxview nw = nmview->getNrdWView().row(nmNode->net[j].nrn[k].soma->index);

                                                for (int m = 0; m < (int)nmNode->net[j].nrn[k].soma->nrdIndex.size(); ++m) {
                                                    if (nw[m] != 0.0f) {
                                                        ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), leafFlags,
                                                            arti->getNrdName(nmNode->net[j].nrn[k].soma->nrdIndex[m], nmName[i]).c_str());

                                                        if (ImGui::IsItemClicked() || nrdcTable == m) {
                                                            nrdcTable = m;
                                                            tNmName = nmName[i];
                                                            tNnName = "";
                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                            tBrchName = "";
                                                            tNrdIdx = nmNode->net[j].nrn[k].soma->index;
                                                            tIndex = nmNode->net[j].nrn[k].soma->nrdIndex[m];

                                                            senTable = -1;
                                                            nrdTable = -1;
                                                            modTable = -1;
                                                            synTable = -1;
                                                        }
                                                    }
                                                }

                                                ImGui::TreePop();
                                            }

                                            //Create modulatory connections tree
                                            bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 3), treeFlags,
                                                (arti->getNrdName(nmNode->net[j].nrn[k].soma->index, nmName[i]) + " Modulatory Connections").c_str());

                                            if (modConnOpen) {
                                                mtxview mw = nmview->getModWView().row(nmNode->net[j].nrn[k].soma->index);

                                                for (int m = 0; m < (int)nmNode->net[j].nrn[k].soma->modIndex.size(); ++m) {
                                                    if (mw[m] != 0.0f) {
                                                        ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->net[j].nrn[k].soma->nrdIndex.size()), leafFlags,
                                                            arti->getNrdName(nmNode->net[j].nrn[k].soma->modIndex[m], nmName[i]).c_str());

                                                        if (ImGui::IsItemClicked() || modTable == m) {
                                                            modTable = m;
                                                            tNmName = nmName[i];
                                                            tNnName = "";
                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                            tBrchName = "";
                                                            tNrdIdx = nmNode->net[j].nrn[k].soma->index;
                                                            tIndex = nmNode->net[j].nrn[k].soma->modIndex[m];

                                                            senTable = -1;
                                                            nrdTable = -1;
                                                            nrdcTable = -1;
                                                            synTable = -1;
                                                        }
                                                    }
                                                }

                                                ImGui::TreePop();
                                            }

                                            //Create synaptic connections tree
                                            bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->nrn.size() + 4), treeFlags,
                                                (arti->getNrdName(nmNode->net[j].nrn[k].soma->index, nmName[i]) + " Synaptic Connections").c_str());

                                            if (synConnOpen) {
                                                mtxview sw = nmview->getSynWView().row(nmNode->net[j].nrn[k].soma->index);

                                                for (int m = 0; m < (int)nmNode->net[j].nrn[k].soma->synIndex.size(); ++m) {
                                                    if (sw[m] != 0.0f) {
                                                        ImGui::TreeNodeEx((void*)(intptr_t)((m + 1) + nmNode->net[j].nrn[k].soma->nrdIndex.size() +
                                                            nmNode->net[j].nrn[k].soma->modIndex.size()), leafFlags,
                                                            arti->getNrdName(nmNode->net[j].nrn[k].soma->synIndex[m], nmName[i]).c_str());

                                                        if (ImGui::IsItemClicked() || synTable == m) {
                                                            synTable = m;
                                                            tNmName = nmName[i];
                                                            tNnName = "";
                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                            tBrchName = "";
                                                            tNrdIdx = nmNode->net[j].nrn[k].soma->index;
                                                            tIndex = nmNode->net[j].nrn[k].soma->synIndex[m];

                                                            senTable = -1;
                                                            nrdTable = -1;
                                                            nrdcTable = -1;
                                                            modTable = -1;
                                                        }
                                                    }
                                                }

                                                ImGui::TreePop();
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create neuron's dendrite tree
                                        std::string nrndendname = (nmNode->net[j].nrn[k].name + " Dendrite Branches");

                                        if (ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrn.size() * 5), treeFlags, nrndendname.c_str())) {
                                            for (int m = 0; m < nmNode->net[j].nrn[k].dend.size(); ++m) {
                                                if (ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), treeFlags, nmNode->net[j].nrn[k].dend[m].name.c_str())) {
                                                    for (int n = 0; n < nmNode->net[j].nrn[k].dend[m].nrd.size(); ++n) {
                                                        bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(n + 1), treeFlags,
                                                            arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].index, nmName[i]).c_str());

                                                        if (nrdOpen) {
                                                            //Create neurode data tree
                                                            ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].dend[m].nrd.size() + 1), leafFlags, "Neurode");

                                                            if (ImGui::IsItemClicked() || nrdTable == n) {
                                                                nrdTable = n;
                                                                tNmName = nmName[i];
                                                                tNnName = "";
                                                                tNrnName = nmNode->net[j].nrn[k].name;
                                                                tBrchName = nmNode->net[j].nrn[k].dend[m].name;
                                                                tIndex = nmNode->net[j].nrn[k].dend[m].nrd[n].index;

                                                                senTable = -1;
                                                                nrdcTable = -1;
                                                                modTable = -1;
                                                                synTable = -1;
                                                            }

                                                            //Create neurode connections tree
                                                            bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].dend[m].nrd.size() + 2), 
                                                                treeFlags, (arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].index, nmName[i]) + 
                                                                    " Neurode Connections").c_str());

                                                            if (nrdConnOpen) {
                                                                mtxview nw = nmview->getNrdWView().row(nmNode->net[j].nrn[k].dend[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex.size(); ++o) {
                                                                    if (nw[nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)(o + 1), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || nrdcTable == o) {
                                                                            nrdcTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].dend[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].dend[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            modTable = -1;
                                                                            synTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            //Create modulatory connections tree
                                                            bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].dend[m].nrd.size() + 3), 
                                                                treeFlags, (arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].index, nmName[i]) + 
                                                                    " Modulatory Connections").c_str());

                                                            if (modConnOpen) {
                                                                mtxview mw = nmview->getModWView().row(nmNode->net[j].nrn[k].dend[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].dend[m].nrd[n].modIndex.size(); ++o) {
                                                                    if (mw[nmNode->net[j].nrn[k].dend[m].nrd[n].modIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)((o + 1) + 
                                                                            nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex.size()), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].modIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || modTable == o) {
                                                                            modTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].dend[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].dend[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].dend[m].nrd[n].modIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            nrdcTable = -1;
                                                                            synTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            //Create synaptic connections tree
                                                            bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].dend[m].nrd.size() + 4), 
                                                                treeFlags, (arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].index, nmName[i]) + 
                                                                    " Synaptic Connections").c_str());

                                                            if (synConnOpen) {
                                                                mtxview sw = nmview->getSynWView().row(nmNode->net[j].nrn[k].dend[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].dend[m].nrd[n].synIndex.size(); ++o) {
                                                                    if (sw[nmNode->net[j].nrn[k].dend[m].nrd[n].synIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)((o + 1) + 
                                                                            nmNode->net[j].nrn[k].dend[m].nrd[n].nrdIndex.size() +
                                                                            nmNode->net[j].nrn[k].dend[m].nrd[n].modIndex.size()), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].dend[m].nrd[n].synIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || synTable == o) {
                                                                            synTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].dend[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].dend[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].dend[m].nrd[n].synIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            nrdcTable = -1;
                                                                            modTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            ImGui::TreePop();
                                                        }
                                                    }

                                                    ImGui::TreePop();
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        //Create neuron's axon tree
                                        std::string nrnaxonname = (nmNode->net[j].nrn[k].name + " Axon Branches");

                                        if (ImGui::TreeNodeEx((void*)(intptr_t)((k + 1) + nmNode->nrn.size() * 6), treeFlags, nrnaxonname.c_str())) {
                                            for (int m = 0; m < nmNode->net[j].nrn[k].axon.size(); ++m) {
                                                if (ImGui::TreeNodeEx((void*)(intptr_t)(m + 1), treeFlags, nmNode->net[j].nrn[k].axon[m].name.c_str())) {
                                                    for (int n = 0; n < nmNode->net[j].nrn[k].axon[m].nrd.size(); ++n) {
                                                        bool nrdOpen = ImGui::TreeNodeEx((void*)(intptr_t)(n + 1), treeFlags,
                                                            arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].index, nmName[i]).c_str());

                                                        if (nrdOpen) {
                                                            //Create neurode data tree
                                                            ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].axon[m].nrd.size() + 1), leafFlags, "Neurode");

                                                            if (ImGui::IsItemClicked() || nrdTable == n) {
                                                                nrdTable = n;
                                                                tNmName = nmName[i];
                                                                tNnName = "";
                                                                tNrnName = nmNode->net[j].nrn[k].name;
                                                                tBrchName = nmNode->net[j].nrn[k].axon[m].name;
                                                                tIndex = nmNode->net[j].nrn[k].axon[m].nrd[n].index;

                                                                senTable = -1;
                                                                nrdcTable = -1;
                                                                modTable = -1;
                                                                synTable = -1;
                                                            }

                                                            //Create neurode connections tree
                                                            bool nrdConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].axon[m].nrd.size() + 2), 
                                                                treeFlags, (arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].index, nmName[i]) + 
                                                                    " Neurode Connections").c_str());

                                                            if (nrdConnOpen) {
                                                                mtxview nw = nmview->getNrdWView().row(nmNode->net[j].nrn[k].axon[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex.size(); ++o) {
                                                                    if (nw[nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)(o + 1), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || nrdcTable == o) {
                                                                            nrdcTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].axon[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].axon[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            modTable = -1;
                                                                            synTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            //Create modulatory connections tree
                                                            bool modConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].axon[m].nrd.size() + 3), 
                                                                treeFlags, (arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].index, nmName[i]) + 
                                                                    " Modulatory Connections").c_str());

                                                            if (modConnOpen) {
                                                                mtxview mw = nmview->getModWView().row(nmNode->net[j].nrn[k].axon[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].axon[m].nrd[n].modIndex.size(); ++o) {
                                                                    if (mw[nmNode->net[j].nrn[k].axon[m].nrd[n].modIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)((o + 1) + 
                                                                            nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex.size()), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].modIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || modTable == o) {
                                                                            modTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].axon[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].axon[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].axon[m].nrd[n].modIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            nrdcTable = -1;
                                                                            synTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            //Create synaptic connections tree
                                                            bool synConnOpen = ImGui::TreeNodeEx((void*)(intptr_t)(nmNode->net[j].nrn[k].axon[m].nrd.size() + 4), 
                                                                treeFlags,
                                                                (arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].index, nmName[i]) + 
                                                                    " Synaptic Connections").c_str());

                                                            if (synConnOpen) {
                                                                mtxview sw = nmview->getSynWView().row(nmNode->net[j].nrn[k].axon[m].nrd[n].index);

                                                                for (int o = 0; o < (int)nmNode->net[j].nrn[k].axon[m].nrd[n].synIndex.size(); ++o) {
                                                                    if (sw[nmNode->net[j].nrn[k].axon[m].nrd[n].synIndex[o]] != 0.0f) {
                                                                        ImGui::TreeNodeEx((void*)(intptr_t)((o + 1) + 
                                                                            nmNode->net[j].nrn[k].axon[m].nrd[n].nrdIndex.size() +
                                                                            nmNode->net[j].nrn[k].axon[m].nrd[n].modIndex.size()), leafFlags,
                                                                            arti->getNrdName(nmNode->net[j].nrn[k].axon[m].nrd[n].synIndex[o], nmName[i]).c_str());

                                                                        if (ImGui::IsItemClicked() || synTable == o) {
                                                                            synTable = o;
                                                                            tNmName = nmName[i];
                                                                            tNnName = "";
                                                                            tNrnName = nmNode->net[j].nrn[k].name;
                                                                            tBrchName = nmNode->net[j].nrn[k].axon[m].name;
                                                                            tNrdIdx = nmNode->net[j].nrn[k].axon[m].nrd[n].index;
                                                                            tIndex = nmNode->net[j].nrn[k].axon[m].nrd[n].synIndex[o];

                                                                            senTable = -1;
                                                                            nrdTable = -1;
                                                                            nrdcTable = -1;
                                                                            modTable = -1;
                                                                        }
                                                                    }
                                                                }

                                                                ImGui::TreePop();
                                                            }

                                                            ImGui::TreePop();
                                                        }
                                                    }

                                                    ImGui::TreePop();
                                                }
                                            }

                                            ImGui::TreePop();
                                        }

                                        ImGui::TreePop();
                                    }
                                }

                                ImGui::TreePop();
                            }
                        }

                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }

        ImGui::EndChild();

        //Create data table of selected tree node
        ImGui::SameLine();
        ImGui::BeginChild("Data Region", ImVec2(0.0f, 0.0f), false, NULL);

        //Display sensory input table
        if (senTable > -1)
            BeginSensoryInputTable(tNmName, tGrpName, tSenIdx, tIndex);
        //Display neurode table
        else if (nrdTable > -1)
            BeginNeurodeTable(tNmName, tNnName, tNrnName, tBrchName, tIndex);
        //Display neurode connection table
        else if (nrdcTable > -1)
            BeginNeurodeConnTable(tNmName, tNnName, tNrnName, tBrchName, tNrdIdx, tIndex);
        //Display modulatory connection table
        else if (modTable > -1)
            BeginModulatoryConnTable(tNmName, tNnName, tNrnName, tBrchName, tNrdIdx, tIndex);
        //Display synapse connection table
        else if (synTable > -1)
            BeginSynapseConnTable(tNmName, tNnName, tNrnName, tBrchName, tNrdIdx, tIndex);

        ImGui::EndChild();

        ImGui::End();
    }
}

//TreeViewWindow begin sensory input table function
void TreeViewWindow::BeginSensoryInputTable(C_STR_REF nmName, C_STR_REF grpName, size_t senIdx, size_t index) {
    const int COLUMNS = 6;
    int ROWS = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;
    const std::string cycleHdr = "Cycle";
    const std::string xHdr = "X";
    const std::string sHdr = "S";
    const std::string sdHdr = "SD";
    const std::string wHdr = "W";
    const std::string swHdr = "SW";
    if (ntree->neuralMatrixNode(nmName) == NULL) return;
    neuralmtxview* nmview = ntree->neuralMatrixNode(nmName)->nmv;
    mtxview x = nmview->getSenXView().row(senIdx);
    mtxview s = nmview->getSenSView().row(senIdx);
    mtxview sd = nmview->getSenSdView().row(senIdx);
    mtxview w = nmview->getSenWView().row(senIdx);
    mtxview sw = nmview->getSenSwView().row(senIdx);

    static std::string nm = "";
    static std::string grp = "";
    static size_t sen = 0;
    static size_t idx = 0;
    static std::vector<std::string> cyclestr(ROWS);
    static std::vector<std::string> xstr(ROWS);
    static std::vector<std::string> sstr(ROWS);
    static std::vector<std::string> sdstr(ROWS);
    static std::vector<std::string> wstr(ROWS);
    static std::vector<std::string> swstr(ROWS);
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Reorderable | 
        ImGuiTableFlags_NoSavedSettings;

    //If the same table is being displayed
    if (nm == nmName && grp == grpName && sen == senIdx && idx == index) {
        //Update data elements
        for (int i = (int)cyclestr.size() - 1; i > 0; --i) {
            cyclestr[i] = cyclestr[i - 1];
            xstr[i] = xstr[i - 1];
            sstr[i] = sstr[i - 1];
            sdstr[i] = sdstr[i - 1];
            wstr[i] = wstr[i - 1];
            swstr[i] = swstr[i - 1];
        }
    }
    else {
        //Set new table info
        nm = nmName;
        grp = grpName;
        sen = senIdx;
        idx = index;

        //Clear data elements
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            cyclestr[i] = "";
            xstr[i] = "";
            sstr[i] = "";
            sdstr[i] = "";
            wstr[i] = "";
            swstr[i] = "";
        }
    }

    //Get current data values
    cyclestr[0] = std::format("{}", *nmview->getCycle());
    xstr[0] = std::format("{:.3f}", x[index]);
    sstr[0] = std::format("{:.3f}", s[index]);
    sdstr[0] = std::format("{:.3f}", sd[index]);
    wstr[0] = std::format("{:.3f}", w[index]);
    swstr[0] = std::format("{:.3f}", sw[index]);

    //Create the table
    std::string title = std::format("Sensory Input {} at {}", sen + 1, nmview->getNeurodeName(idx)->c_str()).c_str();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
    ImGui::Text(title.c_str());

    if (ImGui::BeginTable("Sensory Input Table", COLUMNS, flags)) {
        //Display table column headers
        ImGui::TableSetupColumn(cycleHdr.c_str());
        ImGui::TableSetupColumn(xHdr.c_str());
        ImGui::TableSetupColumn(sHdr.c_str());
        ImGui::TableSetupColumn(sdHdr.c_str());
        ImGui::TableSetupColumn(wHdr.c_str());
        ImGui::TableSetupColumn(swHdr.c_str());
        ImGui::TableSetupScrollFreeze(6, 0);
        ImGui::TableHeadersRow();

        //Display data rows
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(cyclestr[i].c_str()).x);
            ImGui::Text(cyclestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xstr[i].c_str()).x);
            ImGui::Text(xstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sstr[i].c_str()).x);
            ImGui::Text(sstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sdstr[i].c_str()).x);
            ImGui::Text(sdstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(wstr[i].c_str()).x);
            ImGui::Text(wstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(swstr[i].c_str()).x);
            ImGui::Text(swstr[i].c_str());
        }

        ImGui::EndTable();
    }
}

//TreeViewWindow begin neurode table function
void TreeViewWindow::BeginNeurodeTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t index) {
    const int COLUMNS = 15;
    int ROWS = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;
    const std::string cycleHdr = "Cycle";
    const std::string oaHdr = "OA";
    const std::string ofHdr = "OF";
    const std::string opHdr = "OP";
    const std::string obHdr = "OB";
    const std::string oyHdr = "OY";
    const std::string sHdr = "S";
    const std::string sdHdr = "SD";
    const std::string ahHdr = "AH";
    const std::string alHdr = "AL";
    const std::string asHdr = "AS";
    const std::string aHdr = "A";
    const std::string tHdr = "T";
    const std::string rHdr = "R";
    const std::string yHdr = "Y";
    if (ntree->neuralMatrixNode(nmName) == NULL) return;
    neuralmtxview* nmview = ntree->neuralMatrixNode(nmName)->nmv;
    mtxview oa = nmview->getOscAView();
    mtxview of = nmview->getOscFView();
    mtxview op = nmview->getOscPView();
    mtxview ob = nmview->getOscBView();
    mtxview oy = nmview->getOscYView();
    mtxview s = nmview->getSView();
    mtxview sd = nmview->getSdView();
    mtxview ah = nmview->getAhView();
    mtxview al = nmview->getAlView();
    mtxview as = nmview->getAsView();
    mtxview a = nmview->getAView();
    mtxview t = nmview->getTView();
    mtxview r = nmview->getRView();
    mtxview y = nmview->getYView();

    static std::string nm = "";
    static std::string nn = "";
    static std::string nrn = "";
    static std::string brch = "";
    static size_t idx = 0;
    static std::vector<std::string> cyclestr(ROWS);
    static std::vector<std::string> oastr(ROWS);
    static std::vector<std::string> ofstr(ROWS);
    static std::vector<std::string> opstr(ROWS);
    static std::vector<std::string> obstr(ROWS);
    static std::vector<std::string> oystr(ROWS);
    static std::vector<std::string> sstr(ROWS);
    static std::vector<std::string> sdstr(ROWS);
    static std::vector<std::string> ahstr(ROWS);
    static std::vector<std::string> alstr(ROWS);
    static std::vector<std::string> asstr(ROWS);
    static std::vector<std::string> astr(ROWS);
    static std::vector<std::string> tstr(ROWS);
    static std::vector<std::string> rstr(ROWS);
    static std::vector<std::string> ystr(ROWS);

    //If the same table is being displayed
    if (nm == nmName && nn == nnName && nrn == nrnName && brch == brchName && idx == index) {
        //Update data elements
        for (int i = (int)cyclestr.size() - 1; i > 0; --i) {
            cyclestr[i] = cyclestr[i - 1];
            oastr[i] = oastr[i - 1];
            ofstr[i] = ofstr[i - 1];
            opstr[i] = opstr[i - 1];
            obstr[i] = obstr[i - 1];
            oystr[i] = oystr[i - 1];
            sstr[i] = sstr[i - 1];
            sdstr[i] = sdstr[i - 1];
            ahstr[i] = ahstr[i - 1];
            alstr[i] = alstr[i - 1];
            asstr[i] = asstr[i - 1];
            astr[i] = astr[i - 1];
            tstr[i] = tstr[i - 1];
            rstr[i] = rstr[i - 1];
            ystr[i] = ystr[i - 1];
        }
    }
    else {
        //Set new table info
        nm = nmName;
        nn = nnName;
        nrn = nrnName;
        brch = brchName;
        idx = index;

        //Clear data elements
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            cyclestr[i] = "";
            oastr[i] = "";
            ofstr[i] = "";
            opstr[i] = "";
            obstr[i] = "";
            oystr[i] = "";
            sstr[i] = "";
            sdstr[i] = "";
            ahstr[i] = "";
            alstr[i] = "";
            asstr[i] = "";
            astr[i] = "";
            tstr[i] = "";
            rstr[i] = "";
            ystr[i] = "";
        }
    }

    //Get current data values
    cyclestr[0] = std::format("{}", *nmview->getCycle());
    oastr[0] = std::format("{:.3f}", oa[index]);
    ofstr[0] = std::format("{:.3f}", of[index]);
    opstr[0] = std::format("{:.3f}", op[index]);
    obstr[0] = std::format("{:.3f}", ob[index]);
    oystr[0] = std::format("{:.3f}", oy[index]);
    sstr[0] = std::format("{:.3f}", s[index]);
    sdstr[0] = std::format("{:.3f}", sd[index]);
    ahstr[0] = std::format("{:.3f}", ah[index]);
    alstr[0] = std::format("{:.3f}", al[index]);
    asstr[0] = std::format("{:.3f}", as[index]);
    astr[0] = std::format("{:.3f}", a[index]);
    tstr[0] = std::format("{:.3f}", t[index]);
    rstr[0] = std::format("{:.3f}", r[index]);
    ystr[0] = std::format("{:.3f}", y[index]);

    //Create the table
    std::string title = std::format("{}", nmview->getNeurodeName(idx)->c_str()).c_str();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
    ImGui::Text(title.c_str());

    if (ImGui::BeginTable("Neurode Table", COLUMNS, tableFlags)) {
        //Display table column headers
        ImGui::TableSetupColumn(cycleHdr.c_str());
        ImGui::TableSetupColumn(oaHdr.c_str());
        ImGui::TableSetupColumn(ofHdr.c_str());
        ImGui::TableSetupColumn(opHdr.c_str());
        ImGui::TableSetupColumn(obHdr.c_str());
        ImGui::TableSetupColumn(oyHdr.c_str());
        ImGui::TableSetupColumn(sHdr.c_str());
        ImGui::TableSetupColumn(sdHdr.c_str());
        ImGui::TableSetupColumn(ahHdr.c_str());
        ImGui::TableSetupColumn(alHdr.c_str());
        ImGui::TableSetupColumn(asHdr.c_str());
        ImGui::TableSetupColumn(aHdr.c_str());
        ImGui::TableSetupColumn(tHdr.c_str());
        ImGui::TableSetupColumn(rHdr.c_str());
        ImGui::TableSetupColumn(yHdr.c_str());
        ImGui::TableSetupScrollFreeze(15, 0);
        ImGui::TableHeadersRow();

        //Display data rows
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(cyclestr[i].c_str()).x);
            ImGui::Text(cyclestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(oastr[i].c_str()).x);
            ImGui::Text(oastr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ofstr[i].c_str()).x);
            ImGui::Text(ofstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(opstr[i].c_str()).x);
            ImGui::Text(opstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(obstr[i].c_str()).x);
            ImGui::Text(obstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(oystr[i].c_str()).x);
            ImGui::Text(oystr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sstr[i].c_str()).x);
            ImGui::Text(sstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sdstr[i].c_str()).x);
            ImGui::Text(sdstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ahstr[i].c_str()).x);
            ImGui::Text(ahstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(alstr[i].c_str()).x);
            ImGui::Text(alstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(asstr[i].c_str()).x);
            ImGui::Text(asstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(astr[i].c_str()).x);
            ImGui::Text(astr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(tstr[i].c_str()).x);
            ImGui::Text(tstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(rstr[i].c_str()).x);
            ImGui::Text(rstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ystr[i].c_str()).x);
            ImGui::Text(ystr[i].c_str());
        }

        ImGui::EndTable();
    }
}

//TreeViewWindow begin neurode connection table function
void TreeViewWindow::BeginNeurodeConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index) {
    const int COLUMNS = 7;
    int ROWS = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;
    const std::string cycleHdr = "Cycle";
    const std::string xHdr = "X";
    const std::string wHdr = "W";
    const std::string xwHdr = "XW";
    const std::string ypHdr = "YP";
    const std::string ypwHdr = "YPW";
    if (ntree->neuralMatrixNode(nmName) == NULL) return;
    neuralmtxview* nmview = ntree->neuralMatrixNode(nmName)->nmv;
    mtxview x = nmview->getNrdXView().row(nrdIdx);
    mtxview w = nmview->getNrdWView().row(nrdIdx);
    mtxview xw = nmview->getNrdXwView().row(nrdIdx);
    mtxview yp = nmview->getNrdYpView().row(nrdIdx);
    mtxview ypw = nmview->getNrdYpwView().row(nrdIdx);

    static std::string nm = "";
    static std::string nn = "";
    static std::string nrn = "";
    static std::string brch = "";
    static size_t nrd = 0;
    static size_t idx = 0;
    static std::vector<std::string> cyclestr(ROWS);
    static std::vector<std::string> xstr(ROWS);
    static std::vector<std::string> wstr(ROWS);
    static std::vector<std::string> xwstr(ROWS);
    static std::vector<std::string> ypstr(ROWS);
    static std::vector<std::string> ypwstr(ROWS);

    //If the same table is being displayed
    if (nm == nmName && nn == nnName && nrn == nrnName && brch == brchName && nrd == nrdIdx && idx == index) {
        //Update data elements
        for (int i = (int)cyclestr.size() - 1; i > 0; --i) {
            cyclestr[i] = cyclestr[i - 1];
            xstr[i] = xstr[i - 1];
            wstr[i] = wstr[i - 1];
            xwstr[i] = xwstr[i - 1];
            ypstr[i] = ypstr[i - 1];
            ypwstr[i] = ypwstr[i - 1];
        }
    }
    else {
        //Set new table info
        nm = nmName;
        nn = nnName;
        nrn = nrnName;
        brch = brchName;
        nrd = nrdIdx;
        idx = index;

        //Clear data elements
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            cyclestr[i] = "";
            xstr[i] = "";
            wstr[i] = "";
            xwstr[i] = "";
            ypstr[i] = "";
            ypwstr[i] = "";
        }
    }

    //Get current data values
    cyclestr[0] = std::format("{}", *nmview->getCycle());
    xstr[0] = std::format("{:.3f}", x[index]);
    wstr[0] = std::format("{:.3f}", w[index]);
    xwstr[0] = std::format("{:.3f}", xw[index]);
    ypstr[0] = std::format("{:.3f}", yp[index]);
    ypwstr[0] = std::format("{:.3f}", ypw[index]);

    //Create the table
    std::string title = std::format("Neurode Connection from {} to {}", nmview->getNeurodeName(nrd)->c_str(), nmview->getNeurodeName(idx)->c_str()).c_str();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
    ImGui::Text(title.c_str());

    if (ImGui::BeginTable("Neurode Connection Table", COLUMNS, tableFlags)) {
        //Display table column headers
        ImGui::TableSetupColumn(cycleHdr.c_str());
        ImGui::TableSetupColumn(xHdr.c_str());
        ImGui::TableSetupColumn(wHdr.c_str());
        ImGui::TableSetupColumn(xwHdr.c_str());
        ImGui::TableSetupColumn(ypHdr.c_str());
        ImGui::TableSetupColumn(ypwHdr.c_str());
        ImGui::TableSetupScrollFreeze(6, 0);
        ImGui::TableHeadersRow();

        //Display data rows
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(cyclestr[i].c_str()).x);
            ImGui::Text(cyclestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xstr[i].c_str()).x);
            ImGui::Text(xstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(wstr[i].c_str()).x);
            ImGui::Text(wstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xwstr[i].c_str()).x);
            ImGui::Text(xwstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ypstr[i].c_str()).x);
            ImGui::Text(ypstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ypwstr[i].c_str()).x);
            ImGui::Text(ypwstr[i].c_str());
        }

        ImGui::EndTable();
    }
}

//TreeViewWindow begin modulatory connection table function
void TreeViewWindow::BeginModulatoryConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index) {
    const int COLUMNS = 7;
    int ROWS = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;
    const std::string cycleHdr = "Cycle";
    const std::string xHdr = "X";
    const std::string xbHdr = "XB";
    const std::string sHdr = "S";
    const std::string sdHdr = "SD";
    const std::string wHdr = "W";
    const std::string swHdr = "SW";
    if (ntree->neuralMatrixNode(nmName) == NULL) return;
    neuralmtxview* nmview = ntree->neuralMatrixNode(nmName)->nmv;
    mtxview x = nmview->getModXView().row(nrdIdx);
    mtxview xb = nmview->getModXbView().row(nrdIdx);
    mtxview s = nmview->getModSView().row(nrdIdx);
    mtxview sd = nmview->getModSdView().row(nrdIdx);
    mtxview w = nmview->getModWView().row(nrdIdx);
    mtxview sw = nmview->getModSwView().row(nrdIdx);

    static std::string nm = "";
    static std::string nn = "";
    static std::string nrn = "";
    static std::string brch = "";
    static size_t nrd = 0;
    static size_t idx = 0;
    static std::vector<std::string> cyclestr(ROWS);
    static std::vector<std::string> xstr(ROWS);
    static std::vector<std::string> xbstr(ROWS);
    static std::vector<std::string> sstr(ROWS);
    static std::vector<std::string> sdstr(ROWS);
    static std::vector<std::string> wstr(ROWS);
    static std::vector<std::string> swstr(ROWS);

    //If the same table is being displayed
    if (nm == nmName && nn == nnName && nrn == nrnName && brch == brchName && nrd == nrdIdx && idx == index) {
        //Update data elements
        for (int i = (int)cyclestr.size() - 1; i > 0; --i) {
            cyclestr[i] = cyclestr[i - 1];
            xstr[i] = xstr[i - 1];
            xbstr[i] = xbstr[i - 1];
            sstr[i] = sstr[i - 1];
            sdstr[i] = sdstr[i - 1];
            wstr[i] = wstr[i - 1];
            swstr[i] = swstr[i - 1];
        }
    }
    else {
        //Set new table info
        nm = nmName;
        nn = nnName;
        nrn = nrnName;
        brch = brchName;
        nrd = nrdIdx;
        idx = index;

        //Clear data elements
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            cyclestr[i] = "";
            xstr[i] = "";
            xbstr[i] = "";
            sstr[i] = "";
            sdstr[i] = "";
            wstr[i] = "";
            swstr[i] = "";
        }
    }

    //Get current data values
    cyclestr[0] = std::format("{}", *nmview->getCycle());
    xstr[0] = std::format("{:.3f}", x[index]);
    xbstr[0] = std::format("{:.3f}", xb[index]);
    sstr[0] = std::format("{:.3f}", s[index]);
    sdstr[0] = std::format("{:.3f}", sd[index]);
    wstr[0] = std::format("{:.3f}", w[index]);
    swstr[0] = std::format("{:.3f}", sw[index]);

    //Create the table
    std::string title = std::format("Modulatory Connection from {} to {}", nmview->getNeurodeName(nrd)->c_str(), nmview->getNeurodeName(idx)->c_str()).c_str();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
    ImGui::Text(title.c_str());

    if (ImGui::BeginTable("Modulatory Connection Table", COLUMNS, tableFlags)) {
        //Display table column headers
        ImGui::TableSetupColumn(cycleHdr.c_str());
        ImGui::TableSetupColumn(xHdr.c_str());
        ImGui::TableSetupColumn(xbHdr.c_str());
        ImGui::TableSetupColumn(sHdr.c_str());
        ImGui::TableSetupColumn(sdHdr.c_str());
        ImGui::TableSetupColumn(wHdr.c_str());
        ImGui::TableSetupColumn(swHdr.c_str());
        ImGui::TableSetupScrollFreeze(6, 0);
        ImGui::TableHeadersRow();

        //Display data rows
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(cyclestr[i].c_str()).x);
            ImGui::Text(cyclestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xstr[i].c_str()).x);
            ImGui::Text(xstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xbstr[i].c_str()).x);
            ImGui::Text(xbstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sstr[i].c_str()).x);
            ImGui::Text(sstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sdstr[i].c_str()).x);
            ImGui::Text(sdstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(wstr[i].c_str()).x);
            ImGui::Text(wstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(swstr[i].c_str()).x);
            ImGui::Text(swstr[i].c_str());
        }

        ImGui::EndTable();
    }
}

//TreeViewWindow begin sensory input table function
void TreeViewWindow::BeginSynapseConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index) {
    const int COLUMNS = 20;
    int ROWS = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;
    const std::string cycleHdr = "Cycle";
    const std::string xHdr = "X";
    const std::string xfHdr = "XF";
    const std::string xtHdr = "XT";
    const std::string xfdHdr = "XFD";
    const std::string fhHdr = "FH";
    const std::string flHdr = "FL";
    const std::string fsHdr = "FS";
    const std::string fHdr = "F";
    const std::string ypHdr = "YP";
    const std::string yptHdr = "YPT";
    const std::string sHdr = "S";
    const std::string sdHdr = "SD";
    const std::string oHdr = "O";
    const std::string pHdr = "P";
    const std::string wHdr = "W";
    const std::string swHdr = "SW";
    const std::string lpreHdr = "LPRE";
    const std::string lpostHdr = "LPOST";
    const std::string lHdr = "L";
    if (ntree->neuralMatrixNode(nmName) == NULL) return;
    neuralmtxview* nmview = ntree->neuralMatrixNode(nmName)->nmv;
    mtxview x = nmview->getSynXView().row(nrdIdx);
    mtxview xf = nmview->getSynXfView().row(nrdIdx);
    mtxview xt = nmview->getSynXtView().row(nrdIdx);
    mtxview xfd = nmview->getSynXfdView().row(nrdIdx);
    mtxview fh = nmview->getSynFhView().row(nrdIdx);
    mtxview fl = nmview->getSynFlView().row(nrdIdx);
    mtxview fs = nmview->getSynFsView().row(nrdIdx);
    mtxview f = nmview->getSynFView().row(nrdIdx);
    mtxview yp = nmview->getSynYpView().row(nrdIdx);
    mtxview ypt = nmview->getSynYptView().row(nrdIdx);
    mtxview s = nmview->getSynSView().row(nrdIdx);
    mtxview sd = nmview->getSynSdView().row(nrdIdx);
    mtxview o = nmview->getSynOView().row(nrdIdx);
    mtxview p = nmview->getSynPView().row(nrdIdx);
    mtxview w = nmview->getSynWView().row(nrdIdx);
    mtxview sw = nmview->getSynSwView().row(nrdIdx);
    mtxview lpre = nmview->getSynLpreView().row(nrdIdx);
    mtxview lpost = nmview->getSynLpostView().row(nrdIdx);
    mtxview l = nmview->getSynLView().row(nrdIdx);

    static std::string nm = "";
    static std::string nn = "";
    static std::string nrn = "";
    static std::string brch = "";
    static size_t nrd = 0;
    static size_t idx = 0;
    static std::vector<std::string> cyclestr(ROWS);
    static std::vector<std::string> xstr(ROWS);
    static std::vector<std::string> xfstr(ROWS);
    static std::vector<std::string> xtstr(ROWS);
    static std::vector<std::string> xfdstr(ROWS);
    static std::vector<std::string> fhstr(ROWS);
    static std::vector<std::string> flstr(ROWS);
    static std::vector<std::string> fsstr(ROWS);
    static std::vector<std::string> fstr(ROWS);
    static std::vector<std::string> ypstr(ROWS);
    static std::vector<std::string> yptstr(ROWS);
    static std::vector<std::string> sstr(ROWS);
    static std::vector<std::string> sdstr(ROWS);
    static std::vector<std::string> ostr(ROWS);
    static std::vector<std::string> pstr(ROWS);
    static std::vector<std::string> wstr(ROWS);
    static std::vector<std::string> swstr(ROWS);
    static std::vector<std::string> lprestr(ROWS);
    static std::vector<std::string> lpoststr(ROWS);
    static std::vector<std::string> lstr(ROWS);

    //If the same table is being displayed
    if (nm == nmName && nn == nnName && nrn == nrnName && brch == brchName && nrd == nrdIdx && idx == index) {
        //Update data elements
        for (int i = (int)cyclestr.size() - 1; i > 0; --i) {
            cyclestr[i] = cyclestr[i - 1];
            xstr[i] = xstr[i - 1];
            xfstr[i] = xfstr[i - 1];
            xtstr[i] = xtstr[i - 1];
            xfdstr[i] = xfdstr[i - 1];
            fhstr[i] = fhstr[i - 1];
            flstr[i] = flstr[i - 1];
            fsstr[i] = fsstr[i - 1];
            fstr[i] = fstr[i - 1];
            ypstr[i] = ypstr[i - 1];
            yptstr[i] = yptstr[i - 1];
            sstr[i] = sstr[i - 1];
            sdstr[i] = sdstr[i - 1];
            ostr[i] = ostr[i - 1];
            pstr[i] = pstr[i - 1];
            wstr[i] = wstr[i - 1];
            swstr[i] = swstr[i - 1];
            lprestr[i] = lprestr[i - 1];
            lpoststr[i] = lpoststr[i - 1];
            lstr[i] = lstr[i - 1];
        }
    }
    else {
        //Set new table info
        nm = nmName;
        nn = nnName;
        nrn = nrnName;
        brch = brchName;
        nrd = nrdIdx;
        idx = index;

        //Clear data elements
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            cyclestr[i] = "";
            xstr[i] = "";
            xfstr[i] = "";
            xtstr[i] = "";
            xfdstr[i] = "";
            fhstr[i] = "";
            flstr[i] = "";
            fsstr[i] = "";
            fstr[i] = "";
            ypstr[i] = "";
            yptstr[i] = "";
            sstr[i] = "";
            sdstr[i] = "";
            ostr[i] = "";
            pstr[i] = "";
            wstr[i] = "";
            swstr[i] = "";
            lprestr[i] = "";
            lpoststr[i] = "";
            lstr[i] = "";
        }
    }

    //Get current data values
    cyclestr[0] = std::format("{}", *nmview->getCycle());
    xstr[0] = std::format("{:.3f}", x[index]);
    xfstr[0] = std::format("{:.3f}", xf[index]);
    xtstr[0] = std::format("{:.3f}", xt[index]);
    xfdstr[0] = std::format("{:.3f}", xfd[index]);
    fhstr[0] = std::format("{:.3f}", fh[index]);
    flstr[0] = std::format("{:.3f}", fl[index]);
    fsstr[0] = std::format("{:.3f}", fs[index]);
    fstr[0] = std::format("{:.3f}", f[index]);
    ypstr[0] = std::format("{:.3f}", yp[index]);
    yptstr[0] = std::format("{:.3f}", ypt[index]);
    sstr[0] = std::format("{:.3f}", s[index]);
    sdstr[0] = std::format("{:.3f}", sd[index]);
    ostr[0] = std::format("{:.3f}", o[index]);
    pstr[0] = std::format("{:.3f}", p[index]);
    wstr[0] = std::format("{:.3f}", w[index]);
    swstr[0] = std::format("{:.3f}", sw[index]);
    lprestr[0] = std::format("{:.3f}", lpre[index]);
    lpoststr[0] = std::format("{:.3f}", lpost[index]);
    lstr[0] = std::format("{:.3f}", l[index]);

    //Create the table
    std::string title = std::format("Synapse Connection from {} to {}", nmview->getNeurodeName(nrd)->c_str(), 
        nmview->getNeurodeName(idx)->c_str()).c_str();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
    ImGui::Text(title.c_str());

    if (ImGui::BeginTable("Synapse Connection Table", COLUMNS, tableFlags)) {
        //Display table column headers
        ImGui::TableSetupColumn(cycleHdr.c_str());
        ImGui::TableSetupColumn(xHdr.c_str());
        ImGui::TableSetupColumn(xfHdr.c_str());
        ImGui::TableSetupColumn(xtHdr.c_str());
        ImGui::TableSetupColumn(xfdHdr.c_str());
        ImGui::TableSetupColumn(fhHdr.c_str());
        ImGui::TableSetupColumn(flHdr.c_str());
        ImGui::TableSetupColumn(fsHdr.c_str());
        ImGui::TableSetupColumn(fHdr.c_str());
        ImGui::TableSetupColumn(ypHdr.c_str());
        ImGui::TableSetupColumn(yptHdr.c_str());
        ImGui::TableSetupColumn(sHdr.c_str());
        ImGui::TableSetupColumn(sdHdr.c_str());
        ImGui::TableSetupColumn(oHdr.c_str());
        ImGui::TableSetupColumn(pHdr.c_str());
        ImGui::TableSetupColumn(wHdr.c_str());
        ImGui::TableSetupColumn(swHdr.c_str());
        ImGui::TableSetupColumn(lpreHdr.c_str());
        ImGui::TableSetupColumn(lpostHdr.c_str());
        ImGui::TableSetupColumn(lHdr.c_str());
        ImGui::TableSetupScrollFreeze(20, 0);
        ImGui::TableHeadersRow();

        //Display data rows
        for (int i = 0; i < (int)cyclestr.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(cyclestr[i].c_str()).x);
            ImGui::Text(cyclestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xstr[i].c_str()).x);
            ImGui::Text(xstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xfstr[i].c_str()).x);
            ImGui::Text(xfstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xtstr[i].c_str()).x);
            ImGui::Text(xtstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(xfdstr[i].c_str()).x);
            ImGui::Text(xfdstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(fhstr[i].c_str()).x);
            ImGui::Text(fhstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(flstr[i].c_str()).x);
            ImGui::Text(flstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(fsstr[i].c_str()).x);
            ImGui::Text(fsstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(fstr[i].c_str()).x);
            ImGui::Text(fstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ypstr[i].c_str()).x);
            ImGui::Text(ypstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(yptstr[i].c_str()).x);
            ImGui::Text(yptstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sstr[i].c_str()).x);
            ImGui::Text(sstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(sdstr[i].c_str()).x);
            ImGui::Text(sdstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(ostr[i].c_str()).x);
            ImGui::Text(ostr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(pstr[i].c_str()).x);
            ImGui::Text(pstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(wstr[i].c_str()).x);
            ImGui::Text(wstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(swstr[i].c_str()).x);
            ImGui::Text(swstr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(lprestr[i].c_str()).x);
            ImGui::Text(lprestr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(lpoststr[i].c_str()).x);
            ImGui::Text(lpoststr[i].c_str());
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(lstr[i].c_str()).x);
            ImGui::Text(lstr[i].c_str());
        }

        ImGui::EndTable();
    }
}
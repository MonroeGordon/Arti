/**********************************************************************************************************************/
//Artilab
//File: artilab.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
const float Artilab::DEF_WND_HEIGHT = 1080.0f;
const float Artilab::DEF_WND_WIDTH = 1920.0f;

//Artilab constructor function
Artilab::Artilab(float width, float height) : Frame("Artilab", width, height)
{
    if (!arti.createTicTacToeNeuralMatrix())
        throw ArtilabException("Artilab Exception: failed to create Arti's Tic-Tac-Toe neural matrix\n");

    nmview = arti.neuralMatrix("Tic-Tac-Toe Neural Matrix");
    statusbar = Statusbar(&arti);
    treeViewWnd = TreeViewWindow(&arti);
    tictactoeWnd = TicTacToeWindow(&arti);
    tableViewWnd = TableViewWindow(&arti),
    matrixViewWnd = MatrixViewWindow(&arti),
    showWnd = NONE;

    arti.activate();
}

//Artilab destructor function
Artilab::~Artilab() {
    free(nmview);
    showWnd = 0;
}

//Artilab Arti response function
void Artilab::artiResponse() {
    if (communicatorbar.communicationPending()) {
        std::stack<CommunicatorInput> log = communicatorbar.getLog();

        ImGui::OpenPopup("Arti Response Popup");
        if (ImGui::BeginPopup("Arti Response Popup"))
        {
            //Display Arti's response
            ImGui::Text("Arti:");
            ImGui::Separator();
            ImGui::Text("Greetings! I am Arti.");
            ImGui::EndPopup();

            //Close the popup when the mouse is clicked
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                communicatorbar.responded();
        }
    }
}

//Artilab open function
void Artilab::open() {
    //Open sidebars
    statusbar.open(&mainViewport);
    communicatorbar.open(&mainViewport);
    artibar.open(&mainViewport);

    //Handle sidebar selections
    switch (artibar.selectedOption()) {
    case Artibar::EDITOR_BUTTON:
        //Open editor sidebar
        editorbar.open(&mainViewport);

        //Handle sidebar selections
        switch (editorbar.selectedOption()) {
        default:
            break;
        }
        break;
    case Artibar::GAMES_BUTTON:
        //Open games sidebar
        gamesbar.open(&mainViewport);

        //Handle sidebar selections
        switch (gamesbar.selectedOption()) {
        case Gamesbar::CHESS_BUTTON:
            //Show chess window
            showWnd = CHESS;
            artibar.clearSelection();
            editorbar.clearSelection();
            viewerbar.clearSelection();
            gamesbar.hide(&mainViewport);
            break;
        case Gamesbar::TIC_TAC_TOE_BUTTON:
            //Show tic-tac-toe window
            showWnd = TIC_TAC_TOE;
            artibar.clearSelection();
            editorbar.clearSelection();
            viewerbar.clearSelection();
            gamesbar.hide(&mainViewport);
            break;
        default:
            break;
        }
        break;
    case Artibar::VIEWER_BUTTON:
        //Open viewer sidebar
        viewerbar.open(&mainViewport);

        //Handle sidebar selections
        switch (viewerbar.selectedOption()) {
        case Viewerbar::MATRIX_BUTTON:
            //Show neural matrix view window
            showWnd = MATRIXVIEW;
            artibar.clearSelection();
            editorbar.clearSelection();
            viewerbar.clearSelection();
            viewerbar.hide(&mainViewport);
            break;
        case Viewerbar::TABLE_BUTTON:
            //Show neural table view window
            showWnd = TABLEVIEW;
            artibar.clearSelection();
            editorbar.clearSelection();
            viewerbar.clearSelection();
            viewerbar.hide(&mainViewport);
            break;
        case Viewerbar::TREE_BUTTON:
            //Show neural tree view window
            showWnd = TREEVIEW;
            artibar.clearSelection();
            editorbar.clearSelection();
            viewerbar.clearSelection();
            viewerbar.hide(&mainViewport);
            break;
        default:
            break;
        };
        break;
    default:
        break;
    };

    //Show the specified window
    switch (showWnd) {
    case CHESS:
        break;
    case MATRIXVIEW:
        matrixViewWnd.resize();
        matrixViewWnd.open(&mainViewport);
        break;
    case TABLEVIEW:
        tableViewWnd.resize();
        tableViewWnd.open(&mainViewport);
        break;
    case TIC_TAC_TOE:
        tictactoeWnd.resize();
        tictactoeWnd.open(&mainViewport);
        break;
    case TREEVIEW:
        treeViewWnd.resize();
        treeViewWnd.open(&mainViewport);
        break;
    default:
        break;
    }

    //Handle Arti's communication response
    artiResponse();
}
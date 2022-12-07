/**********************************************************************************************************************/
//Artilab
//File: tttwnd.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//TicTacToeWindow constructor function
TicTacToeWindow::TicTacToeWindow(Arti* a) {
    arti = a;
    artiOutput = std::vector<float>(1);
    game = TicTacToeGame();
    for (int i = 0; i < TTT_BOARD_SIZE; ++i)
        squareImg[i] = ImageFile::loadFile("ttt_square.png");
    oSquareImg = ImageFile::loadFile("o_square.png");
    xSquareImg = ImageFile::loadFile("x_square.png");
    windlrImg = ImageFile::loadFile("ttt_win_diag_lr.png");
    windrlImg = ImageFile::loadFile("ttt_win_diag_rl.png");
    winhImg = ImageFile::loadFile("ttt_win_h.png");
    winvImg = ImageFile::loadFile("ttt_win_v.png");
}

//TicTacToeWindow destructor function
TicTacToeWindow::~TicTacToeWindow() {
    arti = NULL;
    artiOutput.clear();
    for (int i = 0; i < TTT_BOARD_SIZE; ++i)
        squareImg[i] = NULL;
    oSquareImg = NULL;
    xSquareImg = NULL;
}

//TicTacToeWindow open function
void TicTacToeWindow::open(MainViewport* mainViewport) {
    //Static variables
    static int player = 0;
    static bool moveError = false;

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

    //Set Arti's input source
    arti->setInputSource(&game, "Tic-Tac-Toe Neural Matrix");

    //Create window
    if (create("Tic-Tac-Toe", wndFlags)) {
        //Create game region
        if (ImGui::BeginChild("Game Region", ImVec2(xSquareImg->width() * TTT_BOARD_COLS, 0.0f))) {
            //Game variables
            int index = 0;

            //Draw the board
            if (ImGui::BeginTable("Tic-tac-toe Board", TTT_BOARD_COLS, 0,
                ImVec2(xSquareImg->width() * TTT_BOARD_COLS, xSquareImg->height() * TTT_BOARD_COLS))) {
                for (int i = 0; i < TTT_BOARD_ROWS; ++i) {
                    ImGui::TableNextRow();
                    for (int j = 0; j < TTT_BOARD_COLS; ++j) {
                        ImGui::TableNextColumn();
                        index = j + (i * TTT_BOARD_COLS);
                        ImVec2 pos = ImGui::GetCursorScreenPos();

                        ImGui::Image((void*)(intptr_t)squareImg[index]->texture(), ImVec2(squareImg[index]->width(), squareImg[index]->height()));

                        if (game.getBoard()[index] == TTT_PLAYER_X) {
                            ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)xSquareImg->texture(), pos,
                                ImVec2(pos.x + xSquareImg->width(), pos.y + xSquareImg->height()));
                        }
                        else if (game.getBoard()[index] == TTT_PLAYER_O) {
                            ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)oSquareImg->texture(), pos,
                                ImVec2(pos.x + oSquareImg->width(), pos.y + oSquareImg->height()));
                        }

                        //If a game is started
                        if (game.isStarted()) {
                            //If it's player's turn
                            if (game.getTurn() == game.getHumanPlayer()) {
                                //Update board if square is clicked
                                if (ImGui::IsItemClicked()) {
                                    game.move(index);
                                }
                            }
                            //If it's Arti's turn and he makes a move
                            else if (arti->hasOutput("Tic-Tac-Toe Neural Matrix")) {
                                artiOutput = arti->popOutput("Tic-Tac-Toe Neural Matrix");
                                if (artiOutput[0] > 0.0f) {
                                    if (game.move((uint32_t)(artiOutput[0] - 1.0f)) == -1)
                                        moveError = true;
                                }
                            }
                        }
                        //If a player has won the game
                        if (game.getWinner() != 0) {
                            //Draw line over winning spaces
                            const uint32_t* ws = game.getWinSpaces();

                            for (int i = 0; i < TTT_BOARD_COLS; ++i) {
                                if (index == ws[i]) {
                                    if ((ws[0] == 0 && ws[1] == 1 && ws[2] == 0) ||
                                        (ws[3] == 0 && ws[4] == 1 && ws[5] == 0) ||
                                        (ws[6] == 0 && ws[7] == 1 && ws[8] == 0)) {
                                        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)winhImg->texture(), pos,
                                            ImVec2(pos.x + winhImg->width(), pos.y + winhImg->height()));
                                    }
                                    else if ((ws[0] == 0 && ws[3] == 1 && ws[6] == 0) ||
                                        (ws[1] == 0 && ws[4] == 1 && ws[7] == 0) ||
                                        (ws[2] == 0 && ws[5] == 1 && ws[8] == 0)) {
                                        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)winvImg->texture(), pos,
                                            ImVec2(pos.x + winvImg->width(), pos.y + winvImg->height()));
                                    }
                                    else if (ws[0] == 0 && ws[4] == 1 && ws[8] == 0) {
                                        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)windlrImg->texture(), pos,
                                            ImVec2(pos.x + windlrImg->width(), pos.y + windlrImg->height()));
                                    }
                                    else if (ws[2] == 0 && ws[4] == 1 && ws[6] == 0) {
                                        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)windrlImg->texture(), pos,
                                            ImVec2(pos.x + windrlImg->width(), pos.y + windrlImg->height()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            ImGui::EndTable();

            //Show X's move
            if (ImGui::BeginChild("X Moves", ImVec2(xSquareImg->width(), -1.0f))) {
                ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("X Moves").x / 2));
                ImGui::Text("X Moves");
                ImGui::Separator();

                for (int i = 0; i < game.getXMoves().size(); ++i)
                    ImGui::Text("%d: Square %d", (i * 2) + 1, game.getXMoves()[i]);
            }
            ImGui::EndChild();

            //Show game options
            ImGui::SameLine();
            if (ImGui::BeginChild("Game", ImVec2(xSquareImg->width(), -1.0f))) {
                ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("Game Options").x / 2));
                ImGui::Text("Game");
                ImGui::Separator();

                if (ImGui::Button("New Game", ImVec2(xSquareImg->width(), 0.0f))) {
                    if (player == 0)
                        game.newGame(TTT_PLAYER_X, TTT_PLAYER_O);
                    else
                        game.newGame(TTT_PLAYER_O, TTT_PLAYER_X);
                }

                ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("0000Play as X").x / 2));
                ImGui::RadioButton("Play as X", &player, 0);
                ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("0000Play as O").x / 2));
                ImGui::RadioButton("Play as O", &player, 1);

                ImGui::Separator();
                if (game.isStarted()) {
                    ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("Playing").x / 2));
                    ImGui::Text("Playing");
                }
                else if (game.getWinner() != 0) {
                    ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("X wins!").x / 2));
                    ImGui::Text("%c wins!", game.getWinner());
                }
                else if (game.getMoveNum() == TTT_BOARD_SIZE) {
                    ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("Draw!").x / 2));
                    ImGui::Text("Draw!");
                }
            }
            ImGui::EndChild();

            //Show O's moves
            ImGui::SameLine();
            if (ImGui::BeginChild("O Moves", ImVec2(xSquareImg->width(), -1.0f))) {
                ImGui::SetCursorPosX((xSquareImg->width() / 2) - (ImGui::CalcTextSize("O Moves").x / 2));
                ImGui::Text("O Moves");
                ImGui::Separator();

                for (int i = 0; i < game.getOMoves().size(); ++i)
                    ImGui::Text("%d: Square %d", (i * 2) + 2, game.getOMoves()[i]);
            }
            ImGui::EndChild();

            //Show Arti move error popup if Arti made an invalid move
            if (moveError) {
                ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);
                if (ImGui::BeginPopup("Arti Move Error Popup", NULL)) {
                    ImGui::Text("Arti:");
                    ImGui::Separator();
                    ImGui::Text("Pardon me! I appear to have made an invalid move.");
                    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2.0f - ImGui::CalcTextSize("Acknowledged").x / 2.0f);
                    if (ImGui::Button("Acknowledged")) {
                        ImGui::CloseCurrentPopup();
                        moveError = false;
                    }
                    ImGui::EndPopup();
                }
                ImGui::OpenPopup("Arti Move Error Popup");
            }
        }
        ImGui::EndChild();

        //Create Arti's region
        ImGui::SameLine();
        if (ImGui::BeginChild("Arti Region")) {

        }
        ImGui::EndChild();

        ImGui::End();
    }
}
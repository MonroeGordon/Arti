/**********************************************************************************************************************/
//Artilab
//File: tictactoe.cpp
/**********************************************************************************************************************/

//Include files
#include "tictactoe.h"

//TicTacToeGame constructor function
TicTacToeGame::TicTacToeGame() {
	started = false;
	turn = 0;
	artiPlayer = 0;
	humanPlayer = 0;
	moveNum = 0;
	for (int i = 0; i < TTT_BOARD_SIZE; ++i)
		board[i] = 0;
	winner = 0;
	for (int i = 0; i < TTT_BOARD_COLS; ++i)
		winSpace[i] = 0;
}

//TicTacToeGame destructor function
TicTacToeGame::~TicTacToeGame() {
	started = false;
	turn = 0;
	artiPlayer = 0;
	humanPlayer = 0;
	moveNum = 0;
	for (int i = 0; i < TTT_BOARD_SIZE; ++i)
		board[i] = 0;
	winner = 0;
	for (int i = 0; i < TTT_BOARD_COLS; ++i)
		winSpace[i] = 0;
	xMove.clear();
	oMove.clear();
}

//TicTacToeGame find winner function
char TicTacToeGame::findWinner() {
	//Check X
	if (board[0] == TTT_PLAYER_X &&
		board[1] == TTT_PLAYER_X &&
		board[2] == TTT_PLAYER_X) {
		winSpace[0] = 0;
		winSpace[1] = 1;
		winSpace[2] = 2;
		return TTT_PLAYER_X;
	}

	if (board[0] == TTT_PLAYER_X &&
		board[4] == TTT_PLAYER_X &&
		board[8] == TTT_PLAYER_X) {
		winSpace[0] = 0;
		winSpace[1] = 4;
		winSpace[2] = 8;
		return TTT_PLAYER_X;
	}

	if (board[0] == TTT_PLAYER_X &&
		board[3] == TTT_PLAYER_X &&
		board[6] == TTT_PLAYER_X) {
		winSpace[0] = 0;
		winSpace[1] = 3;
		winSpace[2] = 6;
		return TTT_PLAYER_X;
	}

	if (board[1] == TTT_PLAYER_X &&
		board[4] == TTT_PLAYER_X &&
		board[7] == TTT_PLAYER_X) {
		winSpace[0] = 1;
		winSpace[1] = 4;
		winSpace[2] = 7;
		return TTT_PLAYER_X;
	}

	if (board[2] == TTT_PLAYER_X &&
		board[4] == TTT_PLAYER_X &&
		board[6] == TTT_PLAYER_X) {
		winSpace[0] = 2;
		winSpace[1] = 4;
		winSpace[2] = 6;
		return TTT_PLAYER_X;
	}

	if (board[2] == TTT_PLAYER_X &&
		board[5] == TTT_PLAYER_X &&
		board[8] == TTT_PLAYER_X) {
		winSpace[0] = 2;
		winSpace[1] = 5;
		winSpace[2] = 8;
		return TTT_PLAYER_X;
	}

	if (board[3] == TTT_PLAYER_X &&
		board[4] == TTT_PLAYER_X &&
		board[5] == TTT_PLAYER_X) {
		winSpace[0] = 3;
		winSpace[1] = 4;
		winSpace[2] = 5;
		return TTT_PLAYER_X;
	}

	if (board[6] == TTT_PLAYER_X &&
		board[7] == TTT_PLAYER_X &&
		board[8] == TTT_PLAYER_X) {
		winSpace[0] = 6;
		winSpace[1] = 7;
		winSpace[2] = 8;
		return TTT_PLAYER_X;
	}

	//Check O
	if (board[0] == TTT_PLAYER_O &&
		board[1] == TTT_PLAYER_O &&
		board[2] == TTT_PLAYER_O) {
		winSpace[0] = 0;
		winSpace[1] = 1;
		winSpace[2] = 2;
		return TTT_PLAYER_O;
	}

	if (board[0] == TTT_PLAYER_O &&
		board[4] == TTT_PLAYER_O &&
		board[8] == TTT_PLAYER_O) {
		winSpace[0] = 0;
		winSpace[1] = 4;
		winSpace[2] = 8;
		return TTT_PLAYER_O;
	}

	if (board[0] == TTT_PLAYER_O &&
		board[3] == TTT_PLAYER_O &&
		board[6] == TTT_PLAYER_O) {
		winSpace[0] = 0;
		winSpace[1] = 3;
		winSpace[2] = 6;
		return TTT_PLAYER_O;
	}

	if (board[1] == TTT_PLAYER_O &&
		board[4] == TTT_PLAYER_O &&
		board[7] == TTT_PLAYER_O) {
		winSpace[0] = 1;
		winSpace[1] = 4;
		winSpace[2] = 7;
		return TTT_PLAYER_O;
	}

	if (board[2] == TTT_PLAYER_O &&
		board[4] == TTT_PLAYER_O &&
		board[6] == TTT_PLAYER_O) {
		winSpace[0] = 2;
		winSpace[1] = 4;
		winSpace[2] = 6;
		return TTT_PLAYER_O;
	}

	if (board[2] == TTT_PLAYER_O &&
		board[5] == TTT_PLAYER_O &&
		board[8] == TTT_PLAYER_O) {
		winSpace[0] = 2;
		winSpace[1] = 5;
		winSpace[2] = 8;
		return TTT_PLAYER_O;
	}

	if (board[3] == TTT_PLAYER_O &&
		board[4] == TTT_PLAYER_O &&
		board[5] == TTT_PLAYER_O) {
		winSpace[0] = 3;
		winSpace[1] = 4;
		winSpace[2] = 5;
		return TTT_PLAYER_O;
	}

	if (board[6] == TTT_PLAYER_O &&
		board[7] == TTT_PLAYER_O &&
		board[8] == TTT_PLAYER_O) {
		winSpace[0] = 6;
		winSpace[1] = 7;
		winSpace[2] = 8;
		return TTT_PLAYER_O;
	}

	//Return 0 for no winner
	return 0;
}

//TicTacToeGame get Arti player function
char TicTacToeGame::getArtiPlayer() const {
	return artiPlayer;
}

//TicTacToeGame get board function
const char* TicTacToeGame::getBoard() const {
	return board;
}

//TicTacToeGame get human player function
char TicTacToeGame::getHumanPlayer() const {
	return humanPlayer;
}

//TicTacToeGame get move number function
uint32_t TicTacToeGame::getMoveNum() const {
	return moveNum;
}

//TicTacToeGame get O moves function
std::vector<int> TicTacToeGame::getOMoves() const {
	return oMove;
}

//TicTacToeGame get turn function
char TicTacToeGame::getTurn() const {
	return turn;
}

//TicTacToeGame get winner function
char TicTacToeGame::getWinner() const {
	return winner;
}

//TicTacToeGame get win spaces function
const uint32_t* TicTacToeGame::getWinSpaces() const {
	return winSpace;
}

//TicTacToeGame get X moves function
std::vector<int> TicTacToeGame::getXMoves() const {
	return xMove;
}

//TicTacToeGame get started function
bool TicTacToeGame::isStarted() const {
	return started;
}

//TicTacToeGame new game function
int TicTacToeGame::newGame(char p, char a) {
	//Check player settings
	if (p == a || (p != TTT_PLAYER_X && p != TTT_PLAYER_O) || (a != TTT_PLAYER_X && a != TTT_PLAYER_O))
		return -1;

	//Re-initialize for new game
	started = true;
	turn = TTT_PLAYER_X;
	humanPlayer = p;
	artiPlayer = a;
	moveNum = 0;
	for (int i = 0; i < TTT_BOARD_SIZE; ++i)
		board[i] = 0;
	winner = 0;
	xMove.clear();
	oMove.clear();

	return (int)started;
}

//TicTacToeGame move function
int TicTacToeGame::move(uint32_t space) {
	if (space >= TTT_BOARD_SIZE || board[space] != 0)
		return -1;

	board[space] = turn;
	moveNum++;
	(turn == TTT_PLAYER_X) ? xMove.emplace_back(space) : oMove.emplace_back(space);
	turn = (turn == humanPlayer) ? artiPlayer : humanPlayer;
	winner = findWinner();
	started = (winner == 0) && (moveNum < TTT_BOARD_SIZE);

	return (int)started;
}

//TicTacToeGame push inputs function
std::vector<float> TicTacToeGame::pushInputs(Arti* arti, size_t nSensoryInputs) {
	//Collect sensory inputs for Arti
	std::vector<float> artiInput = std::vector<float>(nSensoryInputs);

	if (started) {
		for (int i = 0; i < TTT_BOARD_SIZE * 3; i += 3) {
			if (board[i / 3] == TTT_PLAYER_X)
				artiInput[i] = 1.0f;
			else
				artiInput[i] = 0.0f;

			if (board[i / 3] == TTT_PLAYER_O)
				artiInput[i + 1] = 1.0f;
			else
				artiInput[i + 1] = 0.0f;

			if (moveNum == i / 3)
				artiInput[i + 2] = 1.0f;
			else
				artiInput[i + 2] = 0.0f;
		}

		if (turn == artiPlayer) {
			artiInput[27] = 0.0f;
			artiInput[28] = 1.0f;
		}
		else {
			artiInput[27] = 1.0f;
			artiInput[28] = 0.0f;
		}
	}

	return artiInput;
}
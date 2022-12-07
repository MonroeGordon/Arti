/**********************************************************************************************************************/
//Artilab
//File: tictactoe.h
/**********************************************************************************************************************/

//Include once
#pragma once

//Include files
#include "arti.h"
#include <inttypes.h>
#include <stdio.h>
#include <vector>

//Constant definitions
constexpr int TTT_BOARD_SIZE = 9;
constexpr int TTT_BOARD_ROWS = 3;
constexpr int TTT_BOARD_COLS = 3;
constexpr char TTT_PLAYER_X = 'X';
constexpr char TTT_PLAYER_O = 'O';

//TicTacToeGame class definition
class TicTacToeGame : public ArtiInputSource {
private:
	bool started;
	char turn;
	char artiPlayer;
	char humanPlayer;
	uint32_t moveNum;
	char board[TTT_BOARD_SIZE];
	char winner;
	uint32_t winSpace[TTT_BOARD_COLS];
	std::vector<int> xMove;
	std::vector<int> oMove;

	char findWinner();

public:
	TicTacToeGame();
	~TicTacToeGame();
	char getArtiPlayer() const;
	const char* getBoard() const;
	char getHumanPlayer() const;
	uint32_t getMoveNum() const;
	std::vector<int> getOMoves() const;
	char getTurn() const;
	char getWinner() const;
	const uint32_t* getWinSpaces() const;
	std::vector<int> getXMoves() const;
	bool isStarted() const;
	int move(uint32_t space);
	int newGame(char p, char a);
	std::vector<float> pushInputs(Arti* arti, size_t nSensoryInputs);
};
/**********************************************************************************************************************/
// ArtiCore
// File: imgbutton.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
int ImageButton::PRESSED_FRAMES = 5;

//ImageButton constructor function
ImageButton::ImageButton() :
	btnPos({ 0.0f, 0.0f }),
	btnSize({ 0.0f, 0.0f }),
	buttonImg(NULL),
	frame(0),
	hoveredImg(NULL),
	hovered(false),
	selectedImg(NULL),
	selected(false),
	staySelected(false),
	tooltipStr("")
{

}

//ImageButton destructor function
ImageButton::~ImageButton() {
	btnPos = { 0.0f, 0.0f };
	btnSize = { 0.0f, 0.0f };
	free(buttonImg);
	frame = 0;
	free(hoveredImg);
	hovered = false;
	free(selectedImg);
	selected = false;
	staySelected = false;
	tooltipStr = "";
}

//ImageButton deselect function
void ImageButton::deselect() {
	selected = false;
}

//ImageButton hovering function
void ImageButton::hovering(bool h) {
	hovered = h;
}

//ImageButton get hovered function
bool ImageButton::isHovered() const {
	return hovered;
}

//ImageButton get selected function
bool ImageButton::isSelected() const {
	return selected;
}

//ImageButton get button's position function
wndpos ImageButton::position() const {
	return btnPos;
}

//ImageButton process function
bool ImageButton::process() {
	if (selected) {
		ImGui::Image((void*)(intptr_t)selectedImg->texture(), ImVec2(btnSize.width, btnSize.height));

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(tooltipStr.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		if (staySelected) {
			if (ImGui::IsItemClicked())
				selected = false;
		}
		else {
			if (frame >= PRESSED_FRAMES && !ImGui::IsItemClicked()) {
				selected = false;
				frame = 0;
			}
			else
				frame++;
		}
	}
	else if (hovered) {
		ImGui::Image((void*)(intptr_t)hoveredImg->texture(), ImVec2(btnSize.width, btnSize.height));

		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(tooltipStr.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();

		if (!ImGui::IsItemHovered())
			hovered = false;

		if (ImGui::IsItemClicked())
			selected = true;
	}
	else {
		ImGui::Image((void*)(intptr_t)buttonImg->texture(), ImVec2(btnSize.width, btnSize.height));

		if (ImGui::IsItemHovered())
			hovered = true;

		if (ImGui::IsItemClicked())
			selected = true;
	}

	return selected;
}

//ImageButton select function
void ImageButton::select() {
	selected = true;
}

//ImageButton set position
void ImageButton::setPosition(float x, float y) {
	btnPos = { x, y };
}

//ImageButton set stay selected function
void ImageButton::setStaySelected(bool ss) {
	staySelected = ss;
}

//ImageButton set tooltip function
void ImageButton::setTooltip(const std::string& tip) {
	tooltipStr = tip;
}

//ImageButton get button's size function
wndsize ImageButton::size() const {
	return btnSize;
}

//ImageButton get stay selected function
bool ImageButton::staysSelected() const {
	return staySelected;
}

//ImageButton get tooltip function
std::string ImageButton::tooltip() const {
	return tooltipStr;
}

/**********************************************************************************************************************/
// Artibar Buttons
/**********************************************************************************************************************/
//ViewerButton constructor function
ViewerButton::ViewerButton(float x, float y) {
	buttonImg = ImageFile::loadFile("view_button.png");
	hoveredImg = ImageFile::loadFile("view_button_hovered.png");
	selectedImg = ImageFile::loadFile("view_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = true;
	tooltipStr = "Viewers";
}

//ViewerButton destructor function
ViewerButton::~ViewerButton() {

}

//EditorButton constructor function
EditorButton::EditorButton(float x, float y) {
	buttonImg = ImageFile::loadFile("edit_button.png");
	hoveredImg = ImageFile::loadFile("edit_button_hovered.png");
	selectedImg = ImageFile::loadFile("edit_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = true;
	tooltipStr = "Editors";
}

//EditorButton destructor function
EditorButton::~EditorButton() {

}

//GamesButton constructor function
GamesButton::GamesButton(float x, float y) {
	buttonImg = ImageFile::loadFile("game_button.png");
	hoveredImg = ImageFile::loadFile("game_button_hovered.png");
	selectedImg = ImageFile::loadFile("game_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = true;
	tooltipStr = "Games";
}

//GamesButton destructor function
GamesButton::~GamesButton() {

}

/**********************************************************************************************************************/
// Viewerbar Buttons
/**********************************************************************************************************************/
//TableButton constructor function
TableButton::TableButton(float x, float y) {
	buttonImg = ImageFile::loadFile("table_button.png");
	hoveredImg = ImageFile::loadFile("table_button_hovered.png");
	selectedImg = ImageFile::loadFile("table_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = false;
	tooltipStr = "Neural Table View";
}

//TableButton destructor function
TableButton::~TableButton() {

}

//TreeButton constructor function
TreeButton::TreeButton(float x, float y) {
	buttonImg = ImageFile::loadFile("tree_button.png");
	hoveredImg = ImageFile::loadFile("tree_button_hovered.png");
	selectedImg = ImageFile::loadFile("tree_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = false;
	tooltipStr = "Neural Tree View";
}

//TreeButton destructor function
TreeButton::~TreeButton() {

}

//MatrixButton constructor function
MatrixButton::MatrixButton(float x, float y) {
	buttonImg = ImageFile::loadFile("matrix_button.png");
	hoveredImg = ImageFile::loadFile("matrix_button_hovered.png");
	selectedImg = ImageFile::loadFile("matrix_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = false;
	tooltipStr = "Neural Matrix View";
}

//MatrixButton destructor function
MatrixButton::~MatrixButton() {

}

/**********************************************************************************************************************/
// Gamesbar Buttons
/**********************************************************************************************************************/
//ChessButton constructor function
ChessButton::ChessButton(float x, float y) {
	buttonImg = ImageFile::loadFile("chess_button.png");
	hoveredImg = ImageFile::loadFile("chess_button_hovered.png");
	selectedImg = ImageFile::loadFile("chess_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = false;
	tooltipStr = "Chess";
}

//ChessButton destructor function
ChessButton::~ChessButton() {

}

//TicTacToeButton constructor function
TicTacToeButton::TicTacToeButton(float x, float y) {
	buttonImg = ImageFile::loadFile("ttt_button.png");
	hoveredImg = ImageFile::loadFile("ttt_button_hovered.png");
	selectedImg = ImageFile::loadFile("ttt_button_selected.png");
	btnPos = { x, y };
	btnSize = { 64.0f, 64.0f };
	staySelected = false;
	tooltipStr = "Tic-tac-toe";
}

//TicTacToeButton destructor function
TicTacToeButton::~TicTacToeButton() {

}
/**********************************************************************************************************************/
//Artilab
//File: artilab.h
/**********************************************************************************************************************/

//Include once
#pragma once

//Include files
#define GLEW_STATIC
#include "arti.h"
#include <exception>
#include <format>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include <inttypes.h>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "tictactoe.h"
#include <vector>

//Foward declarations
enum WindowFlags;

struct ArtiException;
struct Position;
struct Size;
struct CommunicatorInput;

class ImageFile;
class MainViewport;
class Frame;
class Window;
class Statusbar;
class Communicatorbar;
class NeuralMatrixViewWindow;
class Artilab;

//Type definitions
typedef int wndflags;

typedef struct Position vppos, wndpos;
typedef struct Size vpsize, wndsize;

typedef class ImageFile imgfile;

enum WindowFlags {
	None = ImGuiWindowFlags_None,
	NoTitleBar = ImGuiWindowFlags_NoTitleBar,
	NoResize = ImGuiWindowFlags_NoResize,
	NoMove = ImGuiWindowFlags_NoMove,
	NoScrollBar = ImGuiWindowFlags_NoScrollbar,
	NoScrollWithMouse = ImGuiWindowFlags_NoScrollWithMouse,
	NoCollapse = ImGuiWindowFlags_NoCollapse,
	AlwaysAutoResize = ImGuiWindowFlags_AlwaysAutoResize,
	NoBackground = ImGuiWindowFlags_NoBackground,
	NoSavedSettings = ImGuiWindowFlags_NoSavedSettings,
	NoMouseInputs = ImGuiWindowFlags_NoMouseInputs,
	MenuBar = ImGuiWindowFlags_MenuBar,
	HorizontalScrollbar = ImGuiWindowFlags_HorizontalScrollbar,
	NoFocusOnAppearing = ImGuiWindowFlags_NoFocusOnAppearing,
	NoBringToFrontOnFocus = ImGuiWindowFlags_NoBringToFrontOnFocus,
	AlwaysVerticalScrollbar = ImGuiWindowFlags_AlwaysVerticalScrollbar,
	AlwaysHorizontalScrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
	AlwaysUseWindowPadding = ImGuiWindowFlags_AlwaysUseWindowPadding,
	NoNavInputs = ImGuiWindowFlags_NoNavInputs,
	NoNavFocus = ImGuiWindowFlags_NoNavFocus,
	UnsavedDocument = ImGuiWindowFlags_UnsavedDocument,
	NoNav = ImGuiWindowFlags_NoNav,
	NoDecoration = ImGuiWindowFlags_NoDecoration,
	NoInputs = ImGuiWindowFlags_NoInputs,
	Closable = 1 << 31
};

//Artilab exception struct definition
struct ArtilabException : public std::exception {
	const char* msg;

	ArtilabException(const char* message) {
		msg = message;
	}

	ArtilabException(const std::string& message) {
		msg = message.c_str();
	}

	const char* what() const throw() {
		return msg;
	}
};

//Position struct definition
typedef struct Position {
	float x;
	float y;
} vppos, wndpos;

//Size struct definition
typedef struct Size {
	float width;
	float height;
} vpsize, wndsize;

//Communicator input struct definition
struct CommunicatorInput {
	bool user;
	std::string input;
};

//ImageFile class definition
typedef class ImageFile {
private:
	static std::string PATH_NAME;

	float w;
	float h;
	GLuint t;

	ImageFile(GLuint tex = 0, float x = 0.0f, float y = 0.0f);

public:
	~ImageFile();
	static ImageFile* loadFile(const std::string& filename);
	float width();
	float height();
	GLuint texture();
} imgfile;

//ImageButton class definition
class ImageButton {
protected:
	static int PRESSED_FRAMES;

	wndpos btnPos;
	wndsize btnSize;
	imgfile* buttonImg;
	int frame;
	imgfile* hoveredImg;
	bool hovered;
	imgfile* selectedImg;
	bool selected;
	bool staySelected;
	std::string tooltipStr;

public:
	ImageButton();
	virtual ~ImageButton();
	void deselect();
	void hovering(bool h = true);
	bool isHovered() const;
	bool isSelected() const;
	wndpos position() const;
	bool process();
	void select();
	void setPosition(float x, float y);
	void setStaySelected(bool ss);
	void setTooltip(const std::string& tip);
	wndsize size() const;
	bool staysSelected() const;
	std::string tooltip() const;
};

//ViewerButton class definition (Artibar button)
class ViewerButton : public ImageButton {
public:
	ViewerButton(float x = 0.0f, float y = 0.0f);
	~ViewerButton();
};

//EditorButton class definition (Artibar button)
class EditorButton : public ImageButton {
public:
	EditorButton(float x = 0.0f, float y = 0.0f);
	~EditorButton();
};

//GamesButton class definition (Artibar button)
class GamesButton : public ImageButton {
public:
	GamesButton(float x = 0.0f, float y = 0.0f);
	~GamesButton();
};

//TableButton class definition (Viewerbar button)
class TableButton : public ImageButton {
public:
	TableButton(float x = 0.0f, float y = 0.0f);
	~TableButton();
};

//TreeButton class definition (Viewerbar button)
class TreeButton : public ImageButton {
public:
	TreeButton(float x = 0.0f, float y = 0.0f);
	~TreeButton();
};

//MatrixButton class definition (Viewerbar button)
class MatrixButton : public ImageButton {
public:
	MatrixButton(float x = 0.0f, float y = 0.0f);
	~MatrixButton();
};

//ChessButton class definition (Gamesbar button)
class ChessButton : public ImageButton {
public:
	ChessButton(float x = 0.0f, float y = 0.0f);
	~ChessButton();
};

//TicTacToeButton class definition (Gamesbar button)
class TicTacToeButton : public ImageButton {
public:
	TicTacToeButton(float x = 0.0f, float y = 0.0f);
	~TicTacToeButton();
};

//MainViewport class definition
class MainViewport {
private:
	ImGuiViewport* viewport;
	vppos wPos;
	vpsize wSize;

public:
	MainViewport();
	~MainViewport();
	void addSidebar(float x, float y, float width, float height);
	void removeSidebar(float x, float y, float width, float height);
	void setViewport();
	vppos workPosition() const;
	vpsize workSize() const;
};

//Frame class definition
class Frame {
protected:
	GLFWwindow* window;
	ImGuiIO io;
	ImVec4 clrClear;
	bool close;
	wndpos pos;
	wndsize wndSize;
	MainViewport mainViewport;

	virtual void open() = 0;

public:
	static ImVec4 DEF_CLEAR_COLOR;

	Frame(const char* title, float width, float height);
	~Frame();
	void launch();
};

//Window class definition
class Window {
protected:
	MainViewport* mainViewport;
	wndpos wndPos;
	wndsize wndSize;
	bool pOpen;
	wndflags wndFlags;
	bool anchor;
	bool resizeWnd;

	bool create(const char* name = "##", wndflags wndFlags = 0);

public:
	Window();
	virtual ~Window();
	virtual void open(MainViewport* mainViewport) = 0;
	wndpos position() const;
	void resize();
	wndsize size() const;
};

//Statusbar class definition
class Statusbar : public Window {
private:
	Arti* arti;

public:
	Statusbar(Arti* a = NULL);
	~Statusbar();
	void open(MainViewport* mainViewport);
};

//Sidebar class definition
class Sidebar : public Window {
protected:
	int selection;

public:
	static float DEF_WND_WIDTH;

	Sidebar();
	virtual ~Sidebar();
	virtual void clearSelection() = 0;
	virtual void hide(MainViewport* mainViewport) = 0;
	int selectedOption() const;
};

//Artibar class definition
class Artibar : public Sidebar {
private:
	ViewerButton viewerBtn;
	EditorButton editorBtn;
	GamesButton gamesBtn;

public:
	enum {
		NONE,
		VIEWER_BUTTON,
		EDITOR_BUTTON,
		GAMES_BUTTON
	};

	Artibar();
	~Artibar();
	void clearSelection();
	void hide(MainViewport* mainViewport);
	void open(MainViewport* mainViewport);
};

//Viewerbar class definition
class Viewerbar : public Sidebar {
private:
	TableButton tableBtn;
	TreeButton treeBtn;
	MatrixButton matrixBtn;

public:
	enum {
		NONE,
		TABLE_BUTTON,
		TREE_BUTTON,
		MATRIX_BUTTON
	};

	Viewerbar();
	~Viewerbar();
	void clearSelection();
	void hide(MainViewport* mainViewport);
	void open(MainViewport* mainViewport);
};

//Editorbar class definition
class Editorbar : public Sidebar {
private:

public:
	enum {
		NONE
	};

	Editorbar();
	~Editorbar();
	void clearSelection();
	void hide(MainViewport* mainViewport);
	void open(MainViewport* mainViewport);
};

//Gamesbar class definition
class Gamesbar : public Sidebar {
private:
	TicTacToeButton tttBtn;
	ChessButton chessBtn;

public:
	enum {
		NONE,
		TIC_TAC_TOE_BUTTON,
		CHESS_BUTTON
	};

	Gamesbar();
	~Gamesbar();
	void clearSelection();
	void hide(MainViewport* mainViewport);
	void open(MainViewport* mainViewport);
};

//Communicatorbar class definition
class Communicatorbar : public Window {
private:
	char inputBuf[256];
	std::stack<CommunicatorInput> log;
	bool communicate;

public:
	static ImVec4 ARTI_INPUT_COLOR;
	static float DEF_WND_WIDTH;
	static ImVec4 USER_INPUT_COLOR;

	Communicatorbar();
	~Communicatorbar();
	void addLog(bool user, const char* fmt, ...);
	bool communicationPending() const;
	std::stack<CommunicatorInput> getLog() const;
	void open(MainViewport* mainViewport);
	void responded();
};

//TreeViewWindow class definition
class TreeViewWindow : public Window {
private:
	Arti* arti;
	NeuralTree* ntree;
	ImGuiTreeNodeFlags treeFlags;
	ImGuiTableFlags tableFlags;

	void BeginSensoryInputTable(C_STR_REF nmName, C_STR_REF grpName, size_t senIdx, size_t index);
	void BeginNeurodeTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t index);
	void BeginNeurodeConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index);
	void BeginModulatoryConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index);
	void BeginSynapseConnTable(C_STR_REF nmName, C_STR_REF nnName, C_STR_REF nrnName, C_STR_REF brchName, size_t nrdIdx, size_t index);

public:
	TreeViewWindow(Arti* a = NULL);
	~TreeViewWindow();
	void open(MainViewport* mainViewport);
};

//TableViewWindow class definition
class TableViewWindow : public Window {
private:
	static size_t MATRIX_NAME_SIZE;
	static std::string MATRIX_NAME[49];

	enum {
		SENSORY_INPUT,
		SENSORY_INPUT_SUM,
		SENSORY_INPUT_SUM_DECAY,
		SENSORY_WEIGHT,
		SENSORY_WEIGHTED_INPUT_SUM,
		SYNAPTIC_INPUT,
		SYNAPTIC_INPUT_FREQUENCY,
		SYNAPTIC_INPUT_TIME,
		SYNAPTIC_INPUT_FREQUENCY_DECAY,
		SYNAPTIC_FILTER_HIGHPASS_PEAK,
		SYNAPTIC_FILTER_LOWPASS_PEAK,
		SYNAPTIC_FILTER_SENSITIVITY,
		SYNAPTIC_FILTER_INPUT,
		SYNAPTIC_BACKPROPAGATED_OUTPUT,
		SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME,
		SYNAPTIC_INPUT_SUM,
		SYNAPTIC_INPUT_SUM_DECAY,
		SYNAPTIC_OCCURRENCE,
		SYNAPTIC_PERMANENCE,
		SYNAPTIC_WEIGHT,
		SYNAPTIC_WEIGHTED_INPUT_SUM,
		SYNAPTIC_LEARNING_PRESPIKE_WINDOW,
		SYNAPTIC_LEARNING_POSTSPIKE_WINDOW,
		SYNAPTIC_LEARNING,
		MODULATORY_INPUT,
		MODULATORY_BASE_INPUT,
		MODULATORY_INPUT_SUM,
		MODULATORY_INPUT_SUM_DECAY,
		MODULATORY_WEIGHT,
		MODULATORY_WEIGHTED_INPUT_SUM,
		NEURODE_INPUT,
		NEURODE_WEIGHT,
		NEURODE_WEIGHTED_INPUT,
		NEURODE_BACKPROPAGATED_OUTPUT,
		NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT,
		OSCILLATOR_AMPLITUDE,
		OSCILLATOR_FREQUENCY,
		OSCILLATOR_PHASE,
		OSCILLATOR_BASE_OUTPUT,
		OSCILLATOR_OUTPUT,
		NEURODE_INPUT_SUM,
		NEURODE_INPUT_SUM_DECAY,
		NEURODE_ACTIVATION_HIGHPASS_PEAK,
		NEURODE_ACTIVATION_LOWPASS_PEAK,
		NEURODE_ACTIVATION_SENSITIVITY,
		NEURODE_ACTIVATION,
		NEURODE_THRESHOLD,
		NEURODE_REFRACTORY_PERIOD,
		NEURODE_OUTPUT
	};

	Arti* arti;
	ImGuiTableFlags tableFlags;
	std::string title;
	int columns;
	int rows;
	std::vector<std::string> header;
	std::vector<mtxview> mview;
	std::vector<std::vector<std::string>> column;

public:
	TableViewWindow(Arti* a = NULL);
	~TableViewWindow();
	void open(MainViewport* mainViewport);
};

//MatrixViewWindow definition
class MatrixViewWindow : public Window {
private:
	Arti* arti;

public:
	MatrixViewWindow(Arti* a = NULL);
	~MatrixViewWindow();
	void open(MainViewport* mainViewport);
};

//TicTacToeWindow definition
class TicTacToeWindow : public Window {
private:
	Arti* arti;
	std::vector<float> artiOutput;
	TicTacToeGame game;
	ImageFile* squareImg[TTT_BOARD_SIZE];
	ImageFile* oSquareImg;
	ImageFile* xSquareImg;
	ImageFile* windlrImg;
	ImageFile* windrlImg;
	ImageFile* winhImg;
	ImageFile* winvImg;

public:
	TicTacToeWindow(Arti* a = NULL);
	~TicTacToeWindow();
	void open(MainViewport* mainViewport);
};

//Artilab class definition
class Artilab : public Frame {
private:
	Statusbar statusbar;
	Artibar artibar;
	Viewerbar viewerbar;
	Editorbar editorbar;
	Gamesbar gamesbar;
	Communicatorbar communicatorbar;
	TreeViewWindow treeViewWnd;
	TableViewWindow tableViewWnd;
	MatrixViewWindow matrixViewWnd;
	TicTacToeWindow tictactoeWnd;
	Arti arti;
	neuralmtxview* nmview;
	int showWnd;

	void open();

	enum {
		NONE,
		CHESS,
		MATRIXVIEW,
		TABLEVIEW,
		TIC_TAC_TOE,
		TREEVIEW
	};

public:
	static const float DEF_WND_HEIGHT;
	static const float DEF_WND_WIDTH;

	Artilab(float width = DEF_WND_WIDTH, float height = DEF_WND_HEIGHT);
	~Artilab();
	void artiResponse();
};
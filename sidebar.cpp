/**********************************************************************************************************************/
//Artilab
//File: sidebar.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
float Sidebar::DEF_WND_WIDTH = 66.0f;

//Sidebar constructor function
Sidebar::Sidebar() :
	selection(0)
{

}

//Sidebar destructor function
Sidebar::~Sidebar() {
	selection = 0;
}

//Sidebar get selected option function
int Sidebar::selectedOption() const {
	return selection;
}
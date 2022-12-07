/**********************************************************************************************************************/
//Artilab
//File: mainviewport.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//MainViewport constructor function
MainViewport::MainViewport() :
	viewport(NULL),
	wSize{ 0.0f, 0.0f } 
{

}

//MainViewport destructor function
MainViewport::~MainViewport() {
	viewport = NULL;
	wSize = { 0.0f, 0.0f };
}

//MainViewport add sidebar function
void MainViewport::addSidebar(float x, float y, float width, float height) {
	if (viewport != NULL) {
		wPos = { wPos.x + x, wPos.y + y };
		wSize = { wSize.width - width, wSize.height - height };
	}
}

//MainViewport remove sidebar function
void MainViewport::removeSidebar(float x, float y, float width, float height) {
	if (viewport != NULL) {
		wPos = { wPos.x - x, wPos.y - y };
		wSize = { wSize.width + width, wSize.height + height };
	}
}

//MainViewport set viewport function
void MainViewport::setViewport() {
	viewport = ImGui::GetMainViewport();
	wPos = { viewport->WorkPos.x, viewport->WorkPos.y };
	wSize = { viewport->WorkSize.x, viewport->WorkSize.y };
}

//MainViewport work position function
vppos MainViewport::workPosition() const {
	return wPos;
}

//MainViewport work size function
vpsize MainViewport::workSize() const {
	return wSize;
}
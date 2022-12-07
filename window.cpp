/**********************************************************************************************************************/
//Artilab
//File: window.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Window constructor function
Window::Window() :
	mainViewport(NULL),
	wndPos{ 0.0f, 0.0f },
	wndSize{ 0.0f, 0.0f },
	pOpen(true),
	wndFlags(0),
	anchor(true),
	resizeWnd(false)
{

}

//Window destructor function
Window::~Window() {
	wndPos = { 0.0f, 0.0f };
	wndSize = { 0.0f, 0.0f };
	pOpen = false;
	wndFlags = 0;
	anchor = false;
	resizeWnd = false;
}

//Window create function
bool Window::create(const char* name, wndflags wndFlags) {
	if (strcmp(name, "##") == 0 || strcmp(name, "") == 0)
		name = "##";

	if (pOpen) {
		if (!ImGui::Begin(name, ((wndFlags & Closable) == Closable) ? &pOpen : NULL, wndFlags))
		{
			ImGui::End();
			return false;
		}

		wndSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
		wndPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };

		return true;
	}

	return false;
}

//Window position function
wndpos Window::position() const {
	return wndPos;
}

//Window resize function
void Window::resize() {
	resizeWnd = true;
}

//Window size function
wndsize Window::size() const {
	return wndSize;
}
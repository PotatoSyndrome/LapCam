#include "MakePhotoButton.h"

MakePhotoButton::MakePhotoButton(int x, int y, int hMenu, HWND parentWindow, CamManager* aCamManager)
{
	control = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"������� ����",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        x,         // x position 
        y,         // y position 
        180,        // Button width
        30,        // Button height
        parentWindow,     // Parent window
        (HMENU)hMenu,       // No menu.
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    camManager = aCamManager;
}

MakePhotoButton::~MakePhotoButton()
{
}

void MakePhotoButton::processMessage()
{
    camManager->makePhoto();
}

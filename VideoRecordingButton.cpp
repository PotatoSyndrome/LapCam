#include "VideoRecordingButton.h"

VideoRecordingButton::VideoRecordingButton(int x, int y, int hMenu, HWND parentWindow, CamManager* aCamManager)
{
	control = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Почати запис відео",      // Button text 
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

VideoRecordingButton::~VideoRecordingButton()
{
}

void VideoRecordingButton::processMessage()
{
    if (camManager->getWriteNow())
    {
        camManager->stopVideo();
        SendMessage(control, WM_SETTEXT, 0, (LPARAM)L"Почати запис відео");
    }
    else
    {
        camManager->startVideo();
        SendMessage(control, WM_SETTEXT, 0, (LPARAM)L"Припинити запис відео");
    }
}

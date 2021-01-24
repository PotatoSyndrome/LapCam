#include "QualityDropDown.h"
#include "CommCtrl.h"

QualityDropDown::QualityDropDown(int x, int y, int hMenu, HWND parentWindow)
{
	control = CreateWindow(
        WC_COMBOBOX,  // Predefined class; Unicode assumed 
        TEXT(""),      // text 
        CBS_DROPDOWNLIST | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,  // Styles 
        40,         // x position 
        360,         // y position 
        180,        // Button width
        30,        // Button height
        parentWindow,     // Parent window
        (HMENU)hMenu,       // No hand.
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    SendMessage(control, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"480p");
    SendMessage(control, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"720p");
    SendMessage(control, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"1080p");
    SendMessage(control, CB_SETCURSEL, (WPARAM)0 /* ¹ of chosen parameter */, (LPARAM)0);
}

QualityDropDown::~QualityDropDown()
{
}

void QualityDropDown::processMessage()
{
}

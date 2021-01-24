#include "ExtensionDropDown.h"
#include "CommCtrl.h"

ExtensionDropDown::ExtensionDropDown(int x, int y, int hMenu, HWND parentWindow)
{
    control = CreateWindow(
        WC_COMBOBOX,  // Predefined class; Unicode assumed 
        TEXT(""),      // text 
        CBS_DROPDOWNLIST | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,  // Styles 
        40,         // x position 
        280,         // y position 
        180,        // Button width
        30,        // Button height
        parentWindow,     // Parent window
        (HMENU)hMenu,       // No hand.
        (HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    SendMessage(control, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"AVI");
    SendMessage(control, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"MP4");
    SendMessage(control, CB_SETCURSEL, (WPARAM)0 /* ¹ of chosen parameter */, (LPARAM)0);
}

ExtensionDropDown::~ExtensionDropDown()
{
}

void ExtensionDropDown::processMessage()
{
}

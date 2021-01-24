#pragma once
#include "ControlElement.h"
class ExtensionDropDown :
    public ControlElement
{
public:
    ExtensionDropDown(int x, int y, int hMenu, HWND parentWindow);
    ~ExtensionDropDown();
    void processMessage();
};


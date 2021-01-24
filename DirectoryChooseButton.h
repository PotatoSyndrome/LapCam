#pragma once
#include "ControlElement.h"
class DirectoryChooseButton :
    public ControlElement
{
public:
    DirectoryChooseButton(int x, int y, int hMenu, HWND parentWindow);
    ~DirectoryChooseButton();
    void processMessage();
};


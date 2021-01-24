#pragma once
#include "ControlElement.h"
class QualityDropDown :
    public ControlElement
{
public:
    QualityDropDown(int x, int y, int hMenu, HWND parentWindow);
    ~QualityDropDown();
    void processMessage();
};


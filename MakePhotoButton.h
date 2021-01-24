#pragma once
#include "ControlElement.h"
#include "CamManager.h"

class MakePhotoButton :
    public ControlElement
{
private:
    CamManager* camManager;

public:
    MakePhotoButton(int x, int y, int hMenu, HWND parentWindow, CamManager* aCamManager);
    ~MakePhotoButton();
    void processMessage();
};


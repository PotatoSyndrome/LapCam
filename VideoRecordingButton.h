#pragma once
#include "ControlElement.h"
#include "CamManager.h"

class VideoRecordingButton :
    public ControlElement
{
private:
    CamManager* camManager;

public:
    VideoRecordingButton(int x, int y, int hMenu, HWND parentWindow, CamManager* aCamManager);
    ~VideoRecordingButton();
    void processMessage();
};


#pragma once

#include "framework.h"

class ControlElement
{
protected:
	HWND control;

public:
	virtual ~ControlElement();
	virtual void processMessage() = 0;
};


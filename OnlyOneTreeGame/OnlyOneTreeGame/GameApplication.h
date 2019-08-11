#pragma once
#include <SpelSylt/Application.h>

class CGameApplication
	: public SS::CApplication
{
public:

	virtual void SetUpWindow() override;

	virtual void PushStartUpStates() override;

};

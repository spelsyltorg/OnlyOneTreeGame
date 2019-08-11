#pragma once

#include <SpelSylt/Application.h>

namespace tree
{
	class CGameApplication final
		: public SS::CApplication
	{
	private:
		virtual void SetUpWindow() override;
		virtual void PushStartUpStates() override;
	};
}
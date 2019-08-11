#pragma once
#include "SpelSylt/UI/Base/UIState.h"

namespace tree
{
	class CInGameUIState : public UI::UIState
	{
	public:
		CInGameUIState(unsigned int aWidth, unsigned int aHeight);
		virtual ~CInGameUIState() = default;

	private:
		virtual void OnInit(SGameContext& InGameContext) override;
		virtual void OnUpdate(SGameContext& InGameContext) override;
		virtual void OnRender(sf::RenderTarget& InTarget) override;
	};
}
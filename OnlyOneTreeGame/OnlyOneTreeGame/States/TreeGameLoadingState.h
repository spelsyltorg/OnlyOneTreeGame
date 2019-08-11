#pragma once
#include <SpelSylt/UI/Base/UIState.h>

#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Rendering/Sprite/Texture.h>

namespace tree
{
	class CTreeGameLoadingState
		: public UI::UIState
	{
	public:
		CTreeGameLoadingState(unsigned int InWidth, unsigned int InHeight, const std::string& InLayout);
	private:
		virtual void OnInit(SGameContext& InGameContext) override;
		virtual void OnUpdate(SGameContext& InGameContext) override;
		virtual void OnRender(sf::RenderTarget& InTarget) override;

		SS::CTexture FinishedCheckTexture;
		SS::CSprite LoadingSymbol;
		SS::CSprite StartSymbol;
		float LoadingSymbolRotationSpeed;
		bool IsReady;
	};
}
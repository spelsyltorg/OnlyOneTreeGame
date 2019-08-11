#include "GameJamGame/States/TreeGameLoadingState.h"

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/FileHandling/Asset/Assets.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Utility/Time/TimeGetter.h>
#include <SpelSylt/Utility/Input/InputEventGetter.h>

#include <SFML/Graphics/RenderTarget.hpp>

#include "GameJamGame/States/TreeGameState.h"

//------------------------------------------------------------------

using namespace tree;


//------------------------------------------------------------------

CTreeGameLoadingState::CTreeGameLoadingState(unsigned int InWidth, unsigned int InHeight, const std::string& InLayout)
	: UI::UIState(InWidth, InHeight, InLayout)
	, LoadingSymbolRotationSpeed(45.f)
	, IsReady(false)
{
}

//------------------------------------------------------------------

void CTreeGameLoadingState::OnInit(SGameContext& InGameContext)
{
	StartSymbol.SetTextureAsset(InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/start_prompt.png", Synchronous));
	StartSymbol.setOrigin(256.f, 16.f);
	StartSymbol.setPosition(1920.f / 2.f, 1080.f / 2.f);
	
	LoadingSymbol.SetTextureAsset(InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/loading_sprite.png", Synchronous));
	LoadingSymbol.setOrigin(256.f, 256.f);
	LoadingSymbol.setPosition(1920.f / 2.f, 1080.f / 2.f);

	//All things that need to be loaded before the game starts

	//Animations
	InGameContext.AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/explosion.anmbndl");
	InGameContext.AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/splatter.anmbndl");
	InGameContext.AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/water_cloud.anmbndl");
	InGameContext.AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/player_standing.anmbndl");
	InGameContext.AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/minigun_ch.anmbndl");

	//Textures
	InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/tree0.png");
	InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/hitscan.png");

	//Needs to be last in the queue
	FinishedCheckTexture = InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/area.png");
}

//------------------------------------------------------------------

void CTreeGameLoadingState::OnUpdate(SGameContext& InGameContext)
{
	LoadingSymbol.rotate(LoadingSymbolRotationSpeed * InGameContext.Time.GetDeltaTime());

	if (IsReady)
	{
		if (InGameContext.Input.IsKeyPressed(EKeyCode::Space))
		{
			CState::PopAndPushNew(std::make_shared<CTreeGameState>());
		}
	}

	if (FinishedCheckTexture.Get().GetLoadStatus() == ELoadRequestStatus::Done)
	{
		IsReady = true;
	}
}

//------------------------------------------------------------------

void CTreeGameLoadingState::OnRender(sf::RenderTarget& InTarget)
{
	if (!IsReady)
	{
		InTarget.draw(LoadingSymbol);
	}
	else
	{
		InTarget.draw(StartSymbol);
	}
}

//------------------------------------------------------------------
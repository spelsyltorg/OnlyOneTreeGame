#include "GameJamGame/Gameplay/Water/WaterPit.h"

#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>

#include "GameJamGame/Core/AnimationSequencer.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CWaterPit::CWaterPit(SpelSylt::CAssetManager& InAssetManager)
	: Position(0.f, 0.f)
	, Full(false)
	, Filling(false)
{
	PitSprite.SetTextureAsset(InAssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/water_pit.png"));
	FillingAnimation = InAssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/water_fill.anmbndl");
	FillingAnimation.SetPlayType(SS::EAnimationPlayType::OneShot);
	FullAnimation = InAssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/water_full.anmbndl");

	PitSprite.setOrigin(64.f, 64.f);
	FillingAnimation.setOrigin(64.f, 64.f);
	FullAnimation.setOrigin(64.f, 64.f);
}

//------------------------------------------------------------------

void CWaterPit::SetPosition(const sf::Vector2f& InPosition)
{
	Position = InPosition;

	PitSprite.setPosition(Position);
	FillingAnimation.setPosition(Position);
	FullAnimation.setPosition(Position);
}

//------------------------------------------------------------------

const sf::Vector2f& tree::CWaterPit::GetPosition() const
{
	return Position;
}

//------------------------------------------------------------------

void CWaterPit::Update(float InDT)
{
	if (Filling)
	{
		FillingAnimation.Tick(InDT);

		if (FillingAnimation.IsFinished())
		{
			Filling = false;
			Full = true;
		}
	}
	else if (Full)
	{
		FullAnimation.Tick(InDT);
	}
}

//------------------------------------------------------------------

void CWaterPit::Render(SpelSylt::CRenderQueue& aRenderQueue)
{
	aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(PitSprite));

	if (Filling)
	{
		aRenderQueue.Enqueue(ERenderLayer::Game ,SS::SSpriteAnimationRenderCommand(FillingAnimation));
	}
	else if (Full)
	{
		aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteAnimationRenderCommand(FullAnimation));
	}
}

//------------------------------------------------------------------

void CWaterPit::TakeWater()
{
	Filling = false;
	Full = false;
	FillingAnimation.Restart();
}

//------------------------------------------------------------------

void CWaterPit::SetFilling()
{
	Filling = true;

	CAnimationSequencer::PlayAnimationAtPosition("Graphics/Animations/water_cloud.anmbndl", Position);
}

//------------------------------------------------------------------

bool CWaterPit::IsFull() const
{
	return Full;
}

//------------------------------------------------------------------
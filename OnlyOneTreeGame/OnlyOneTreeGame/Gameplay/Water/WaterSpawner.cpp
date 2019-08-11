#include "GameJamGame/Gameplay/Water/WaterSpawner.h"
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Rendering/RenderCommand.h>

#include <SpelSylt/Messaging/Messages/AudioMessages.h>

#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Messaging/MessageQueue.h>

#include "GameJamGame/Core/GameMessages.h"
#include "GameJamGame/Gameplay/Pawn.h"

#include "GameJamGame/Core/AnimationSequencer.h"
#include "GameJamGame/Core/WindowDefines.h"

tree::CWaterSpawner::CWaterSpawner(SpelSylt::CMessageQueue& aMessageQueue, SpelSylt::CAssetManager& aAssetManager, const CPawn& aPlayerPawn) :
	myPlayerPawn(aPlayerPawn),
	myIsPaused(false),
	TopLeft(aAssetManager),
	TopRight(aAssetManager),
	BottomLeft(aAssetManager),
	BottomRight(aAssetManager),
	ActivePit(nullptr)
{
	sf::Vector2f Position = POS_TOP_LEFT;
	TopLeft.SetPosition(Position);
	
	Position = POS_TOP_RIGHT;
	Position.x -= 64.f;
	TopRight.SetPosition(Position);

	Position = POS_BOT_LEFT;
	Position.y -= 64.f;
	BottomLeft.SetPosition(Position);

	Position = POS_BOT_RIGHT;
	Position.x -= 64.f;
	Position.y -= 64.f;
	BottomRight.SetPosition(Position);

	myWater.SetTextureAsset(aAssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/water.png"));
	myWater.setOrigin(32, 32);
	DespawnWater();

	myTimer.Init(CTimedEvent::EType::Repeat, 3.f, [this, &aMessageQueue]() {
		SpawnWater();
		myIsPaused = true;
		aMessageQueue.DispatchEvent<SSoundMessage>("rain");
	});
	myTimer.Start();

	aMessageQueue.Subscribe<InteractMsg>([this, &aMessageQueue](const auto& msg) {
		sf::Vector2f WaterPitPositon(500000.f, 500000.f);
		if (ActivePit)
		{
			WaterPitPositon = ActivePit->GetPosition();
		}

		sf::Vector2f toPlayer = myPlayerPawn.GetPosition() - WaterPitPositon;
		if (Math::Length2(toPlayer) < 64.f * 64.f)
		{
			DespawnWater();
			aMessageQueue.DispatchEvent<PickedUpWater>();
			myIsPaused = false;
		}
	}, mySubs);
}

void tree::CWaterSpawner::Update(float aDT)
{
	if (!myIsPaused)
	{
		myTimer.Update(aDT);
	}

	TopRight.Update(aDT);
	TopLeft.Update(aDT);
	BottomLeft.Update(aDT);
	BottomRight.Update(aDT);
}

void tree::CWaterSpawner::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	//aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(myWater));
	TopRight.Render(aRenderQueue);
	TopLeft.Render(aRenderQueue);
	BottomRight.Render(aRenderQueue);
	BottomLeft.Render(aRenderQueue);
}

void tree::CWaterSpawner::SpawnWater()
{
	int Pick = rand() % 4;

	switch (Pick)
	{
	case 0:
		TopLeft.SetFilling();
		ActivePit = &TopLeft;
		break;
	case 1:
		TopRight.SetFilling();
		ActivePit = &TopRight;
		break;
	case 2:
		BottomRight.SetFilling();
		ActivePit = &BottomRight;
		break;
	case 3:
		BottomLeft.SetFilling();
		ActivePit = &BottomLeft;
		break;
	default:
		break;
	}

	//float halfWidth = 1920.f / 2.f;
	//float halfHeight = 1080.f / 2.f;
	//
	//float x = 1920.f * (rand() % 2) - halfWidth;
	//float y = 1080.f * (rand() % 2) - halfHeight;
	//
	//myWater.setPosition(x - Math::Sign(x) * 32.f, y - Math::Sign(y) * 32.f);
}

void tree::CWaterSpawner::DespawnWater()
{
	if (ActivePit == nullptr)
	{
		return;
	}
	ActivePit->TakeWater();
	ActivePit = nullptr;
}
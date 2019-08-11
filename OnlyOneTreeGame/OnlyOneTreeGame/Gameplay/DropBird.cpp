#include "DropBird.h"
#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Messaging/Messages/AudioMessages.h>
#include <SpelSylt/Math/CommonMath.h>

#include "GameJamGame/Core/GameMessages.h"
#include "GameJamGame/Gameplay/Pawn.h"

tree::DropBird::DropBird(SpelSylt::SGameContext & aGameContext, const CPawn & aPlayerPawn) :
	myContext(aGameContext),
	myPlayerPawn(aPlayerPawn)
{
	myAmmoTextures[EItemType::ShotgunAmmo] = aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/shotgun_ammo.png");
	myAmmoTextures[EItemType::MinigunAmmo] = aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/minigun_ammo.png");
	myAmmoTextures[EItemType::GrenadeAmmo] = aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/grenade_ammo.png");

	myDropBird.SetTextureAsset(aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/drop_bird.png"));
	myDropBird.setOrigin(64, 64);
	myDropBird.setPosition(sf::Vector2f(-10000, -10000));
	myDropBirdShadow.SetTextureAsset(aGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/drop_bird.png"));
	myDropBirdShadow.setOrigin(64, 64);
	myDropBirdShadow.setPosition(sf::Vector2f(-10000, -10000));
	myDropBirdShadow.setColor(sf::Color(0, 0, 0, 130));

	myDropTimer.Init(CTimedEvent::EType::Repeat, 15.f, [this]() {
		int side = Math::Sign((rand() % 100) - 50);
		myDropPos.x = side * (200 + 550) * ((float)rand() / RAND_MAX);
		myDropPos.y = Math::Sign((rand() % 100) - 50) * 450.f * ((float)rand() / RAND_MAX);
		myDir = side;
		myDropBird.setPosition(-side * 1000.f, myDropPos.y);
		myDropBird.setRotation(side * 90.f);
		myShouldDrop = true;
		myContext.MessageQueue.DispatchEvent<SSoundMessage>("eagle");
	});

	myContext.MessageQueue.Subscribe<TreeLevelMsg>([this](const auto& msg) {
		int current = myTreeLevel;
		myTreeLevel = msg.Param;
		if (current == 1 && myTreeLevel == 2)
		{
			myDropTimer.Start();
		}
	}, mySubs);
}

#include <iostream>
void tree::DropBird::Update(float aDT)
{
	myDropTimer.Update(aDT);

	if (myTreeLevel <= 1)
		return;

	myDropBird.move(aDT * myDir * sf::Vector2f(500.f, 0.f));
	if (myShouldDrop && Math::Length(myDropPos - myDropBird.getPosition()) < 50.f)
	{
		SItemDrop item;
		item.id = static_cast<EItemType>(Math::Clamp(myTreeLevel > 2 ? (rand() % (myTreeLevel-1)) : 0, 0, 2));
		std::cout << (int)item.id << std::endl;
		item.pos = myDropBird.getPosition();
		myDroppedItems.emplace_back(item);
		myShouldDrop = false;
	}

	for (int i = 0; i < myDroppedItems.size(); ++i)
	{
		if (Math::Length(myDroppedItems[i].pos - myPlayerPawn.GetPosition()) < 64.f)
		{
			myContext.MessageQueue.DispatchEvent<ItemPickupMsg>(myDroppedItems[i].id);
			myDroppedItems[i] = myDroppedItems.back();
			myDroppedItems.pop_back();
			myContext.MessageQueue.DispatchEvent<SSoundMessage>("pickup");
		}
	}
}

void tree::DropBird::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	SS::CSprite itemSprite;
	for (auto& item : myDroppedItems)
	{
		itemSprite.setPosition(item.pos);
		itemSprite.SetTextureAsset(myAmmoTextures[item.id].Get());
		aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(itemSprite));
	}

	aRenderQueue.Enqueue(ERenderLayer::UI, SS::SSpriteRenderCommand(myDropBird));
	myDropBirdShadow.setPosition(myDropBird.getPosition() + sf::Vector2f(50.f, 50.f));
	myDropBirdShadow.setScale(0.75f, 0.75f);
	myDropBirdShadow.setRotation(myDropBird.getRotation());
	aRenderQueue.Enqueue(ERenderLayer::Background, SS::SSpriteRenderCommand(myDropBirdShadow));
}

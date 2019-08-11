#include "GameJamGame/Gameplay/BloodManager.h"

#include <SpelSylt/Messaging/MessageQueue.h>

#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>

#include <SpelSylt/FileHandling/Asset/AssetManager.h>

#include "GameJamGame/Core/WindowDefines.h"


#include <SFML/Graphics/Sprite.hpp>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CBloodManager::CBloodManager(SpelSylt::CAssetManager& InAssetManager, SpelSylt::CMessageQueue& InMessageQueue)
{
	BloodTexture = InAssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/blood_types.png");

	BloodSprites[(int)EBloodType::Small].setTexture(BloodTexture.Get());
	BloodSprites[(int)EBloodType::Small].setTextureRect(sf::IntRect(0, 0, 64, 64));

	BloodSprites[(int)EBloodType::Medium].setTexture(BloodTexture.Get());
	BloodSprites[(int)EBloodType::Medium].setTextureRect(sf::IntRect(64, 0, 64, 64));

	BloodSprites[(int)EBloodType::Big].setTexture(BloodTexture.Get());
	BloodSprites[(int)EBloodType::Big].setTextureRect(sf::IntRect(128, 0, 64, 64));

	for (int i = 0; i < (int)EBloodType::Count; ++i)
	{
		BloodSprites[i].setOrigin(32.f, 32.f);
	}

	InMessageQueue.Subscribe<EnemyKilledMsg>([this](auto msg) { OnEnemyKilledMsg(msg); }, Subscriptions);

	BloodLayerTexture.create(1920, 1080);
}

//------------------------------------------------------------------

void CBloodManager::Render(SpelSylt::CRenderQueue& aRenderQueue)
{
	aRenderQueue.Enqueue(ERenderLayer::Background, SS::SSFSpriteRenderCommand(BloodLayerTexture.getTexture()));
}

//------------------------------------------------------------------

void CBloodManager::OnEnemyKilledMsg(const EnemyKilledMsg& InMsg)
{
	int Rotation = rand() % 360;
	float RotationDeg = static_cast<float>(Rotation);

	sf::Sprite ThisBloodSprite = BloodSprites[InMsg.Param];
	ThisBloodSprite.rotate(RotationDeg);
	ThisBloodSprite.setPosition(InMsg.ParamTwo.x + RIGHT_X, InMsg.ParamTwo.y + BOTTOM_Y);
	BloodLayerTexture.draw(ThisBloodSprite);
	BloodLayerTexture.display();
}

//------------------------------------------------------------------
#pragma once
#include "GameJamGame/Core/System.h"

#include <SpelSylt/Utility/TimedEvent.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Rendering/Sprite/Texture.h>
#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

#include "GameJamGame/Gameplay/ItemTypes.h"

#include <unordered_map>

namespace SpelSylt
{
	struct SGameContext;
}

namespace tree
{
	class CPawn;

	class DropBird : public ISystem
	{
	public:
		DropBird(SpelSylt::SGameContext& aGameContext, const CPawn& aPlayerPawn);

		virtual void Update(float aDT);
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue);

	private:

		CTimedEvent myDropTimer;

		bool myShouldDrop = false;
		sf::Vector2f myDropPos;
		int myDir;

		const CPawn& myPlayerPawn;
		SpelSylt::SGameContext& myContext;

		SS::CSprite myDropBird;
		SS::CSprite myDropBirdShadow;

		std::unordered_map<EItemType, SS::CTexture> myAmmoTextures;

		struct SItemDrop
		{
			EItemType id;
			sf::Vector2f pos;
		};

		std::vector<SItemDrop> myDroppedItems;

		int myTreeLevel = 1;
		SS::CSubscriptions mySubs;
	};
}
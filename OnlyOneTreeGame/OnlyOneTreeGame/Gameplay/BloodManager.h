#pragma once

#include "GameJamGame/Core/System.h"

#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Rendering/Sprite/Texture.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/TextureAsset.h>

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace SpelSylt
{
	class CAssetManager;
	class CMessageQueue;
}

namespace tree
{
	enum EBloodType : unsigned int
	{
		Small = 0,
		Medium = 1,
		Big = 2,
		Count = 3
	};

	struct SBloodData
	{
		SBloodData(float InRotation, const sf::Vector2f& InPosition, EBloodType InType)
			: Rotation(InRotation)
			, Position(InPosition)
			, Type(InType)
		{}

		float Rotation;
		sf::Vector2f Position;
		EBloodType Type;
	};

	class CBloodManager final
		: public ISystem
	{
	public:
		CBloodManager(SpelSylt::CAssetManager& InAssetManager, SpelSylt::CMessageQueue& InMessageQueue);
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) override;
	private:
		void OnEnemyKilledMsg(const EnemyKilledMsg& InMsg);

		SS::CSubscriptions Subscriptions;
		SS::CTexture BloodTexture;
		sf::Sprite BloodSprites[(int)EBloodType::Count];

		sf::RenderTexture BloodLayerTexture;

	};
}
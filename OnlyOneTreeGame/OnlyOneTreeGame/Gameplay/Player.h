#pragma once
#include "GameJamGame/Gameplay/Pawn.h"

#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

#include "GameJamGame/Core/GameMessages.h"

namespace SpelSylt
{
	class CRenderQueue;
	class CAssetManager;
	class IInputEventGetter;
	class CMessageQueue;
}

namespace tree
{
	enum class EWeaponType
	{
		HandGun,
		ShotGun,
		MiniGun,
		GrenadeGun,
		Count
	};

	class CPlayer : public CPawn
	{
	public:
		CPlayer(SpelSylt::CAssetManager& InAssetManager, SpelSylt::IInputEventGetter& InInputManager, SpelSylt::CMessageQueue& InMessageQueue);

		virtual void Tick(float InDT) override;
		void Draw(SpelSylt::CRenderQueue& InRenderQueue);
	private:
		void OnWeapnChangeMsg(const SwitchWeaponMsg& InMsg);

		SS::CSpriteAnimation RunningAnimation;
		SS::CSpriteAnimation StandingAnimation;
		SS::CSpriteAnimation* ActiveAnimation;
		
		SS::CSpriteAnimation CrosshairAnimation;

		SS::CSprite GunSprites[(int)EWeaponType::Count];
		SS::CSprite BucketSprite;
		SS::CSprite* ActiveHandItem;

		SS::CSprite ArmSprite;
		SS::CSprite ShadowSprite;

		SS::CSubscriptions Subscriptions;

		SpelSylt::IInputEventGetter& InputManager;

	};
}
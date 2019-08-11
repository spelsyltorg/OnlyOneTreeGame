#pragma once
#include "GameJamGame/Core/System.h"

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

#include <array>
#include <unordered_map>

#include "GameJamGame/Gameplay/Pawn.h"

#include "GameJamGame/Gameplay/Weapon/Weapon.h"

namespace SpelSylt
{
	class CDebugDrawer;
	class CMessageQueue;
	class CAssetManager;
}

namespace tree
{
	class CPawn;
	class IWeapon;

	class CWeaponSystem : public ISystem
	{
	public:
		CWeaponSystem(SpelSylt::CDebugDrawer& aDebugDrawer, SpelSylt::SGameContext& aGameContext, const CPawn& aPlayerPawn);

		void Update(float aDT);
		void Render(SpelSylt::CRenderQueue& aRenderQueue);

		const sf::Vector2f& GetPlayerPos() const { return myPlayerPawn.GetPosition(); }
		const sf::Vector2f& GetAimPos() const { return myAimPos; }

	private:

		void SwitchWeapon(int aID);

		const CPawn& myPlayerPawn;
		sf::Vector2f myAimPos;

		SpelSylt::CSubscriptions mySubs;
		SpelSylt::CDebugDrawer& myDebugDrawer;

		std::array<std::unique_ptr<IWeapon>, 4> myWeapons;
		IWeapon* myCurrentWeapon = nullptr;

		std::unordered_map<int, bool> myUnlockedWeapons;
	};
}
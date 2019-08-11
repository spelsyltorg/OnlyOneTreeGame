#include "WeaponSystem.h"
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Debugging/Rendering/DebugDrawer.h>

#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Math/CommonMath.h>

#include "GameJamGame/Gameplay/Weapon/ShotGun.h"
#include "GameJamGame/Gameplay/Weapon/HandGun.h"
#include "GameJamGame/Gameplay/Weapon/MiniGun.h"
#include "GameJamGame/Gameplay/Weapon/GrenadeLauncher.h"
#include "GameJamGame/Gameplay/ItemTypes.h"


tree::CWeaponSystem::CWeaponSystem(SpelSylt::CDebugDrawer & aDebugDrawer, SpelSylt::SGameContext& aGameContext, const CPawn & aPlayerPawn) :
	myDebugDrawer(aDebugDrawer),
	myPlayerPawn(aPlayerPawn),
	myWeapons{	std::make_unique<HandGun>(aGameContext),
				std::make_unique<ShotGun>(aGameContext),
				std::make_unique<MiniGun>(aGameContext),
				std::make_unique<GrenadeLauncher>(aGameContext) }
{
	for (auto& weapon : myWeapons)
		weapon->SetWeaponSystem(this);

	myUnlockedWeapons[0] = true;
	myUnlockedWeapons[1] = false;
	myUnlockedWeapons[2] = false;
	myUnlockedWeapons[3] = false;

	SwitchWeapon(0);
	aGameContext.MessageQueue.DispatchEvent<SwitchWeaponMsg>(0);

	aGameContext.MessageQueue.Subscribe<FireWeaponMsg>([this](const auto& msg) {
		myAimPos = msg.Param - sf::Vector2f(0.5f*1920.f, 0.5f*1080.f);
		if (myCurrentWeapon->CanFire())
			myCurrentWeapon->Shoot();
		//myDebugDrawer.DrawLine(myPlayerPawn.GetPosition(), msg.Param - sf::Vector2f(0.5f*1920.f,0.5f*1080.f), sf::Color::Red);
	}, mySubs);

	aGameContext.MessageQueue.Subscribe<SwitchWeaponMsg>([this](const auto& msg) {
		SwitchWeapon(msg.Param);
	}, mySubs);

	aGameContext.MessageQueue.Subscribe<ItemPickupMsg>([this, &msgQueue = aGameContext.MessageQueue](const auto& msg) {
		switch (msg.Param)
		{
		case EItemType::ShotgunAmmo:
			msgQueue.DispatchEvent<UnlockShotgun>();
			msgQueue.DispatchEvent<ShotgunAmmoMsg>(myWeapons[1]->AddAmmo(12 * 6));
			myUnlockedWeapons[1] = true;
			break;
		case EItemType::MinigunAmmo:
			msgQueue.DispatchEvent<UnlockMinigun>();
			msgQueue.DispatchEvent<MinigunAmmoMsg>(myWeapons[2]->AddAmmo(3 * 36));
			myUnlockedWeapons[2] = true;
			break;
		case EItemType::GrenadeAmmo:
			msgQueue.DispatchEvent<UnlockGrenadeLauncher>();
			 msgQueue.DispatchEvent<GrenadeAmmoMsg>(myWeapons[3]->AddAmmo(1 * 5));
			 myUnlockedWeapons[3] = true;
			break;
		default:
			break;
		}
	}, mySubs);
}

void tree::CWeaponSystem::Update(float aDT)
{
	for (auto& weapon : myWeapons)
	{
		weapon->Update(aDT);
	}
}

void tree::CWeaponSystem::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	for (auto& weapon : myWeapons)
		weapon->Render(aRenderQueue);
}

void tree::CWeaponSystem::SwitchWeapon(int aID)
{
	if (myUnlockedWeapons[aID])
	{
		myCurrentWeapon = myWeapons[Math::Clamp(aID, 0, (int)myWeapons.size() - 1)].get();
		myCurrentWeapon->OnActivated();
	}
}
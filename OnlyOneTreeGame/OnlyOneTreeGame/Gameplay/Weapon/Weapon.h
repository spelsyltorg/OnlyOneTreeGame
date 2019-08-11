#pragma once

#include <SpelSylt/Utility/TimedEvent.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

namespace SpelSylt
{
	class CRenderQueue;
	class CAssetManager;
	struct SGameContext;
}

namespace tree
{
	class CWeaponSystem;

	class IWeapon
	{
	public:
		IWeapon(SpelSylt::SGameContext& aGameContext);

		virtual void Update(float aDt);
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) {}
		virtual void Shoot() = 0;

		void SetWeaponSystem(CWeaponSystem* aWeaponSystem);

		int AddAmmo(int aAmmo);
		bool CanFire() const { return myIsLocked ? false : myCanFire; };

		virtual void OnActivated() {};

	protected:
		bool myCanFire;
		bool myIsLocked;
		int myAmmo;
		SpelSylt::SGameContext& myContext;

		CTimedEvent myTimer;
		CWeaponSystem* WeaponSystem = nullptr;

		SS::CSubscriptions mySubscriptions;
	};
}
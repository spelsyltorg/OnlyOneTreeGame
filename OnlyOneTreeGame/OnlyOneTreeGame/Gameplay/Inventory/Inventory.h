#pragma once
#include "GameJamGame/Core/System.h"

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

namespace SpelSylt
{
	class CMessageQueue;
}

namespace tree
{
	class CInventory : public ISystem
	{
	public:
		CInventory(SpelSylt::CMessageQueue& aMessageQueue);

		virtual void Update(float aDT) override;

	private:

		bool myRefilledThisFrame;
		bool myHasWater;

		SpelSylt::CSubscriptions mySubs;
	};
}
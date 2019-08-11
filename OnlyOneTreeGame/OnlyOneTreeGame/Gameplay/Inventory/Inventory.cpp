#include "Inventory.h"

#include <SpelSylt/Messaging/MessageQueue.h>
#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Messaging/Messages/AudioMessages.h>

tree::CInventory::CInventory(SpelSylt::CMessageQueue & aMessageQueue)
{
	aMessageQueue.Subscribe<PickedUpWater>([this, &aMessageQueue](const auto& msg) {
		myRefilledThisFrame = true;
		myHasWater = true;
		aMessageQueue.DispatchEvent<SSoundMessage>("water_fill");
	}, mySubs);

	aMessageQueue.Subscribe<InteractMsg>([this, &aMessageQueue](const auto& msg) {
		if (!myRefilledThisFrame && myHasWater)
		{
			myHasWater = false;
			aMessageQueue.DispatchEvent<WaterMsg>();
			aMessageQueue.DispatchEvent<SSoundMessage>("water_pour");
		}
	}, mySubs);
}

void tree::CInventory::Update(float aDT)
{
	myRefilledThisFrame = false;
}

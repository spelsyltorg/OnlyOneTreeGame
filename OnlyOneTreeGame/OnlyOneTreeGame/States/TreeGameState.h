#pragma once

#include <SpelSylt/GameState2D.h>

#include "GameJamGame/Gameplay/Controller/ControllerContainer.h"
#include "GameJamGame/Gameplay/Player.h"

#include "GameJamGame/Core/System.h"

#include "GameJamGame/Gameplay/World/WorldState.h"
#include "GameJamGame/Gameplay/World/ProbeConstructor.h"

#include "GameJamGame/Gameplay/Enemy/EnemyManager.h"
#include <SpelSylt/Rendering/Sprite/Texture.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

namespace tree
{
	class CTreeGameState
		: public SS::GameState2D
	{
	public:
		CTreeGameState();
	private:
		virtual void OnInit(SS::SGameContext& InGameContext) override;
		virtual void OnUpdate(SS::SGameContext& InGameContext) override;
		virtual void OnRender(SS::CRenderQueue& InRenderQueue) override;

		void ReadPlayerPawnSpeedFromConfig();

		float CurrentZoom;

		CProbeConstructor ProbeConstructor;
		CControllerContainer Controllers;

		CPlayer* PlayerPawn;
		FWorldObjectID PlayerWorldObjectID;

		std::vector<std::unique_ptr<ISystem>> Systems;

		SS::CSprite AreaBG;
		SS::CTexture EnemyTexture;

		SS::CSubscriptions Subscriptions;

		CWorldState WorldState;
	};
}
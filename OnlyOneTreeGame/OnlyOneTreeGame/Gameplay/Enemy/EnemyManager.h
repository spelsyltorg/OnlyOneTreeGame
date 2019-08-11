#pragma once

#include "GameJamGame/Core/System.h"
#include "GameJamGame/Gameplay/Pawn.h"
#include "GameJamGame/Gameplay/Enemy/Enemy.h"

#include <SpelSylt/Rendering/Sprite/Texture.h>
#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>
#include "GameJamGame/Core/GameMessages.h"

#include "GameJamGame/Gameplay/World/WorldState.h"

#include <vector>

#include <SpelSylt/Utility/TimedEvent.h>

#define MAX_SIMPLE_ENEMY_TYPE 64

namespace SpelSylt
{
	struct SGameContext;
}

namespace tree
{
	class CControllerContainer;

	class CEnemyManager
		: public ISystem
	{
	public:	
		CEnemyManager(CControllerContainer& InControllerContainer, SpelSylt::SGameContext& InGameContext, CWorldState& InWorldState);

		void SetTexture(SS::CTexture& InTexture);

		void Update(float InDT) override;
		void Render(SpelSylt::CRenderQueue& aRenderQueue) override;
	private:
		void SpawnEnemy();
		void KillEnemies(std::vector<int>& InEnemiesMarkedForKill);

		void OnHitscanMsg(const HitscanShotMsg& InMsg);
		void OnTreeLvlMsg(const TreeLevelMsg& InMsg);
		void OnHitscanMsg(const HitscanExplosionMsg& InMsg);

		SS::CSubscriptions Subscriptions;

		SpelSylt::SGameContext& GameContext;
		CWorldState& WorldState;

		SS::CTexture SimpleEnemyTexture;
		
		float TimeBetweenSpawns;
		float TimeUntilNextSpawn;
		size_t NextSimpleEnemy;
		std::vector<CEnemy*> ActiveEnemies;
		std::vector<FWorldObjectID> ActiveWorldObjectIDs;
		std::vector<CEnemy> SimpleEnemyList;

		CTimedEvent mySoundEmitter;
	};
}
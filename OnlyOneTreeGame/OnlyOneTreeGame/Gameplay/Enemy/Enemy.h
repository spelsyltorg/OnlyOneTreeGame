#pragma once

#include "GameJamGame/Gameplay/Enemy/EnemyPawn.h"
#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>
#include <SpelSylt/Rendering/Sprite/Sprite.h>

namespace SpelSylt
{
	struct SGameContext;
	class CRenderQueue;
	class CAssetManager;
}

namespace tree
{
	class CEnemy
	{
	public:
		CEnemy(SpelSylt::CAssetManager& InAssetManager);

		void Update(SpelSylt::SGameContext& InGameContext);
		void Render(SpelSylt::CRenderQueue& InRenderQueue);

		const CEnemyPawn& GetPawn() const;
		CEnemyPawn& GetPawn();

	private:
		CEnemyPawn Pawn;

		SS::CSpriteAnimation WalkAnimation;
		SS::CSpriteAnimation StandingAnimation;
		SS::CSpriteAnimation AttackAnimation;
		SS::CSpriteAnimation* ActiveAnimation;
		SS::CSprite ShadowSprite;

		bool CanAttack(float InDT);

		float TimeToNextAttack;
		float TimeBetweenAttacks;
	};
}
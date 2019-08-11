#include "GameJamGame/Gameplay/Enemy/Enemy.h"

#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Contexts/GameContext.h>

#include <SpelSylt/Utility/Time/Time.h>
#include <SpelSylt/Messaging/MessageQueue.h>

#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/AnimationAsset.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/TextureAsset.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>

#include "GameJamGame/Gameplay/Pawn.h"
#include "GameJamGame/Gameplay/Controller/ControllerInterface.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CEnemy::CEnemy(SpelSylt::CAssetManager& InAssetManager)
	: Pawn()
	, TimeToNextAttack(0.f)
	, TimeBetweenAttacks(5.f)
{
	WalkAnimation = InAssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/zombie.anmbndl");
	StandingAnimation = InAssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/zombie_standing.anmbndl");
	AttackAnimation = InAssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/zombie_attack.anmbndl");

	ShadowSprite.SetTextureAsset(InAssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/shadow.png"));
	ShadowSprite.setOrigin({ 32.f, 28.f });

	WalkAnimation.setOrigin({ 32.f, 32.f });
	StandingAnimation.setOrigin({ 32.f, 32.f });
	AttackAnimation.setOrigin({ 32.f, 32.f });

	AttackAnimation.SetPlayType(EAnimationPlayType::OneShot);
}

//------------------------------------------------------------------

void CEnemy::Update(SpelSylt::SGameContext& InGameContext)
{
	const float DT = InGameContext.Time.GetDeltaTime();

	if (Pawn.HasReachedTarget() && CanAttack(DT))
	{
		InGameContext.MessageQueue.DispatchEvent<TreeAttackedMsg>();
		AttackAnimation.Restart();
		ActiveAnimation = &AttackAnimation;
	}
	else if (Pawn.HasReachedTarget())
	{
		if (ActiveAnimation == &AttackAnimation)
		{
			if (ActiveAnimation->IsFinished())
			{
				ActiveAnimation = &StandingAnimation;
			}
		}
		else
		{
			ActiveAnimation = &StandingAnimation;
		}
	}
	else
	{
		ActiveAnimation = &WalkAnimation;
		Pawn.Tick(DT);
	}

	ActiveAnimation->Tick(DT);
}

//------------------------------------------------------------------

void CEnemy::Render(SpelSylt::CRenderQueue& InRenderQueue)
{
	ShadowSprite.setPosition(Pawn.GetPosition());
	InRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(ShadowSprite));

	const float MoveX = Pawn.GetController().GetVelocity().x;
	if (MoveX < 0.f)
	{
		ActiveAnimation->setScale(-1.f, 1.f);
	}
	else if (MoveX > 0.f)
	{
		ActiveAnimation->setScale(1.f, 1.f);
	}
	ActiveAnimation->setPosition(Pawn.GetPosition());
	InRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteAnimationRenderCommand(*ActiveAnimation));
}

//------------------------------------------------------------------

const CEnemyPawn& tree::CEnemy::GetPawn() const
{
	return Pawn;
}

//------------------------------------------------------------------

CEnemyPawn& tree::CEnemy::GetPawn()
{
	return Pawn;
}

//------------------------------------------------------------------

bool CEnemy::CanAttack(float InDT)
{
	TimeToNextAttack -= InDT;
	if (TimeToNextAttack <= 0.f)
	{
		TimeToNextAttack = TimeBetweenAttacks;
		return true;
	}

	return false;
}

//------------------------------------------------------------------


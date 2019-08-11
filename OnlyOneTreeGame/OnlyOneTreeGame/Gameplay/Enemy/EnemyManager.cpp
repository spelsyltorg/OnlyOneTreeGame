#include "GameJamGame/Gameplay/Enemy/EnemyManager.h"

#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>

#include <SpelSylt/Physics/Collision/CircleCollider.h>
#include <SpelSylt/Physics/Collision/PointCollider.h>
#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/AnimationAsset.h>

#include "GameJamGame/Gameplay/Controller/ControllerContainer.h"

#include "GameJamGame/Core/WindowDefines.h"

#include <SpelSylt/Debugging/Logging/DebugLogger.h>

#include "GameJamGame/Core/AnimationSequencer.h"

#include <SpelSylt/Messaging/Messages/AudioMessages.h>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CEnemyManager::CEnemyManager(CControllerContainer& InControllerContainer, SpelSylt::SGameContext& InGameContext, CWorldState& InWorldState)
	: GameContext(InGameContext)
	, WorldState(InWorldState)
{
	TimeBetweenSpawns = 2.5f;
	TimeUntilNextSpawn = 0.f;
	NextSimpleEnemy = 0;

	SimpleEnemyList.reserve(MAX_SIMPLE_ENEMY_TYPE);

	for (int i = 0; i < MAX_SIMPLE_ENEMY_TYPE; ++i)
	{
		SimpleEnemyList.emplace_back(InGameContext.AssetManager);
		SimpleEnemyList.back().GetPawn().AttachController(InControllerContainer.CreateAIController());
		SimpleEnemyList.back().GetPawn().SetSpeed(64.f);
	}

	InGameContext.MessageQueue.Subscribe<HitscanShotMsg>([this](auto & Msg) { OnHitscanMsg(Msg); }, Subscriptions);
	InGameContext.MessageQueue.Subscribe<TreeLevelMsg>([this](auto & Msg) { OnTreeLvlMsg(Msg); }, Subscriptions);
	InGameContext.MessageQueue.Subscribe<HitscanExplosionMsg>([this](auto & Msg) { OnHitscanMsg(Msg); }, Subscriptions);

	mySoundEmitter.Init(CTimedEvent::EType::Repeat, 5.f, [this]() {
		GameContext.MessageQueue.DispatchEvent<SS::SSoundMessage>("zombie" + std::to_string(rand() % 4));
		mySoundEmitter.SetDuration(5.f + 5.f * ((float)rand() / RAND_MAX));
	});
	mySoundEmitter.Start();
}

//------------------------------------------------------------------

void CEnemyManager::SetTexture(SS::CTexture& InTexture)
{
	SimpleEnemyTexture = InTexture;
}

//------------------------------------------------------------------

void CEnemyManager::Update(float InDT)
{
	TimeUntilNextSpawn -= InDT;

	mySoundEmitter.Update(InDT);


	if (TimeUntilNextSpawn <= 0.f && ActiveEnemies.size() != MAX_SIMPLE_ENEMY_TYPE)
	{
		SpawnEnemy();
	}

	for (CEnemy* ActiveEnemy : ActiveEnemies)
	{
		ActiveEnemy->Update(GameContext);
	}
}

//------------------------------------------------------------------

void CEnemyManager::Render(SpelSylt::CRenderQueue& aRenderQueue)
{
	for (CEnemy* ActiveEnemy : ActiveEnemies)
	{
		ActiveEnemy->Render(aRenderQueue);
	}

	//SS::CSprite Sprite;
	//Sprite.SetTextureAsset(SimpleEnemyTexture.Get());
	//sf::Vector2f TextureRectF = { static_cast<float>(SimpleEnemyTexture.Get().GetSize().x), static_cast<float>(SimpleEnemyTexture.Get().GetSize().y) };
	//Sprite.setOrigin(TextureRectF / 2.f);
	//
	//for (CEnemy* ActiveEnemyPawn : ActiveEnemies)
	//{
	//	Sprite.setPosition(ActiveEnemyPawn->GetPawn().GetPosition());
	//	aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(Sprite));
	//}
}

//------------------------------------------------------------------

void CEnemyManager::SpawnEnemy()
{
	TimeUntilNextSpawn = TimeBetweenSpawns;

	CEnemy& NextEnemy = SimpleEnemyList[NextSimpleEnemy];
	NextSimpleEnemy++;
	NextSimpleEnemy %= MAX_SIMPLE_ENEMY_TYPE;
	NextEnemy.GetPawn().ResetTarget();

	int RandomVal = rand() % 4;

	sf::Vector2f PositionToSpawn = { 0.f, 0.f };

	float RandomOffset = static_cast<float>(rand() % 800) - 400.f;

	switch (RandomVal)
	{
	case 0:
		PositionToSpawn = POS_TOP_CENTER;
		PositionToSpawn.y -= 100.f;
		PositionToSpawn.x += RandomOffset;
		break;
	case 1:
		PositionToSpawn = POS_CENTER_RIGHT;
		PositionToSpawn.x += 100.f;
		PositionToSpawn.y += RandomOffset;
		break;
	case 2:
		PositionToSpawn = POS_BOT_CENTER;
		PositionToSpawn.y += 100.f;
		PositionToSpawn.x += RandomOffset;
		break;
	case 3:
		PositionToSpawn = POS_CENTER_LEFT;
		PositionToSpawn.x -= 100.f;
		PositionToSpawn.y += RandomOffset;
	default:
		break;
	}

	NextEnemy.GetPawn().SetPositon(PositionToSpawn); //Todo: Should be random

	ActiveEnemies.push_back(&NextEnemy);
	ActiveWorldObjectIDs.push_back(WorldState.AddToWorld<CEnemyPawn>(PositionToSpawn, sf::Vector2f(32.f, 32.f)));
}

//------------------------------------------------------------------

void CEnemyManager::KillEnemies(std::vector<int>& InEnemiesMarkedForKill)
{

	for (int i = InEnemiesMarkedForKill.size() - 1; i >= 0; --i)
	{
		int IndexToErase = InEnemiesMarkedForKill[i];
		GameContext.MessageQueue.DispatchEvent<EnemyKilledMsg>(rand() % 3, ActiveEnemies[IndexToErase]->GetPawn().GetPosition());
		CAnimationSequencer::PlayAnimationAtPosition("Graphics/Animations/splatter.anmbndl", ActiveEnemies[IndexToErase]->GetPawn().GetPosition());
		ActiveEnemies.erase(ActiveEnemies.begin() + IndexToErase);
		WorldState.RemoveFromWorld(ActiveWorldObjectIDs[IndexToErase]);
		ActiveWorldObjectIDs.erase(ActiveWorldObjectIDs.begin() + IndexToErase);
	}

	if (InEnemiesMarkedForKill.size() > 0)
	{
		GameContext.MessageQueue.DispatchEvent<SS::SSoundMessage>("Splash");
		GameContext.MessageQueue.DispatchEvent<SS::SSoundMessage>("zombie_death" + std::to_string(rand() % 3));
	}

}

//------------------------------------------------------------------

void CEnemyManager::OnHitscanMsg(const HitscanShotMsg & InMsg)
{
	std::vector<int> EnemiesToKill;
	EnemiesToKill.reserve(MAX_SIMPLE_ENEMY_TYPE);

	const sf::Vector2f& LineStart = InMsg.Param.first;
	const sf::Vector2f& LineEnd = InMsg.Param.second;

	CPointCollider LineStartCollider;
	LineStartCollider.setPosition(LineStart);
	CPointCollider LineEndCollider;
	LineEndCollider.setPosition(LineEnd);

	for (int i = 0; i < ActiveEnemies.size(); ++i)
	{
		CEnemy* Enemy = ActiveEnemies[i];

		CCircleCollider EnemyCollider;
		EnemyCollider.SetRadius(32.f);
		EnemyCollider.setPosition(Enemy->GetPawn().GetPosition());

		if (LineStartCollider.IsColliding(EnemyCollider) || LineEndCollider.IsColliding(EnemyCollider))
		{
			EnemiesToKill.push_back(i);
			continue;
		}

		float LineLength = Math::Length(LineStart - LineEnd);
		float Dot = ((EnemyCollider.getPosition().x - LineStart.x) * (LineEnd.x - LineStart.x)) + ((EnemyCollider.getPosition().y - LineStart.y) * (LineEnd.y - LineStart.y));
		Dot /= (LineLength * LineLength);

		sf::Vector2f ClosestPoint = LineStart + (Dot * (LineEnd - LineStart));

		//Line vs point collision
		float Tolerance = 0.1f;
		float StartToClosestPoint = Math::Length(LineStart - ClosestPoint);
		float EndToClosestPoint = Math::Length(LineEnd - ClosestPoint);

		if (StartToClosestPoint + EndToClosestPoint >= LineLength - Tolerance
			&& StartToClosestPoint + EndToClosestPoint <= LineLength + Tolerance)
		{
			if (Math::Length(ClosestPoint - EnemyCollider.getPosition()) < 32.f)
			{
				EnemiesToKill.push_back(i);
			}
		}
		else
		{
			continue;
		}
	}

	KillEnemies(EnemiesToKill);
}

void tree::CEnemyManager::OnHitscanMsg(const HitscanExplosionMsg & InMsg)
{
	std::vector<int> EnemiesToKill;
	EnemiesToKill.reserve(MAX_SIMPLE_ENEMY_TYPE);

	const sf::Vector2f& explosionPos = InMsg.Param;
	const float radius = InMsg.ParamTwo;

	CCircleCollider ExplosionCollider;
	ExplosionCollider.SetRadius(radius);
	ExplosionCollider.setPosition(explosionPos);

	for (int i = 0; i < ActiveEnemies.size(); ++i)
	{
		CEnemy* Enemy = ActiveEnemies[i];

		CCircleCollider EnemyCollider;
		EnemyCollider.SetRadius(32.f);
		EnemyCollider.setPosition(Enemy->GetPawn().GetPosition());

		if (ExplosionCollider.IsColliding(EnemyCollider))
		{
			EnemiesToKill.push_back(i);
			continue;
		}
	}

	KillEnemies(EnemiesToKill);
}

//------------------------------------------------------------------

void CEnemyManager::OnTreeLvlMsg(const TreeLevelMsg & InMsg)
{
	float X = static_cast<float>(InMsg.Param);
	float Y = 2.5f + (0.1f * X * X) * -1.f;
	TimeBetweenSpawns = Y;
}

//------------------------------------------------------------------
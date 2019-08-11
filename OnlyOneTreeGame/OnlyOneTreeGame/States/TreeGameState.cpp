#include "GameJamGame/States/TreeGameState.h"

#include "GameJamGame/Gameplay/Controller/InputController.h"
#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Contexts/RenderingContext.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/Utility/Input/InputManager.h>
#include <SpelSylt/Messaging/MessageQueue.h>

#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Utility/Time/TimeGetter.h>
#include <SpelSylt/FileHandling/ConfigReader.h>
#include <SpelSylt/FileHandling/FileWatcher.h>
#include <SpelSylt/Math/CommonMath.h>

#include <SpelSylt/Messaging/Messages/AudioMessages.h>


#include "GameJamGame/Gameplay/Tree/Tree.h"
#include "GameJamGame/Gameplay/Water/WaterSpawner.h"
#include "GameJamGame/Gameplay/Inventory/Inventory.h"
#include "GameJamGame/Gameplay/Weapon/WeaponSystem.h"
#include "GameJamGame/Core/AnimationSequencer.h"
#include "GameJamGame/Gameplay/DropBird.h"
#include "GameJamGame/Gameplay/BloodManager.h"
#include "GameJamGame/Gameplay/ZombieSmoke.h"

#include "GameJamGame/States/InGameUIState.h"
//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CTreeGameState::CTreeGameState()
	: GameState2D()
	, ProbeConstructor(WorldState)
	, Controllers()
	, PlayerPawn(nullptr)
	, Systems()
	, AreaBG()
	, WorldState()
{
}

//------------------------------------------------------------------

void CTreeGameState::OnInit(SS::SGameContext& InGameContext)
{
	CState::Push(std::make_shared<CInGameUIState>(1920, 1080));

	InGameContext.MessageQueue.DispatchEvent<SS::SMusicMessage>("soundtrack", false);

	PlayerPawn = new CPlayer(InGameContext.AssetManager, InGameContext.Input, InGameContext.MessageQueue);
	
	Systems.emplace_back(std::make_unique<CTree>(InGameContext.MessageQueue, InGameContext.AssetManager, *PlayerPawn));
	Systems.emplace_back(std::make_unique<CWaterSpawner>(InGameContext.MessageQueue, InGameContext.AssetManager, *PlayerPawn));
	Systems.emplace_back(std::make_unique<CInventory>(InGameContext.MessageQueue));
	Systems.emplace_back(std::make_unique<CWeaponSystem>(myDebugDrawer, InGameContext, *PlayerPawn));
	Systems.emplace_back(std::make_unique<CAnimationSequencer>(InGameContext.AssetManager));
	Systems.emplace_back(std::make_unique<CBloodManager>(InGameContext.AssetManager, InGameContext.MessageQueue));
	Systems.emplace_back(std::make_unique<ZombieSmoke>(InGameContext));

	Systems.emplace_back(std::make_unique<CEnemyManager>(Controllers, InGameContext, WorldState));
	Systems.emplace_back(std::make_unique<DropBird>(InGameContext, *PlayerPawn));
	CEnemyManager* EnemySpawner = reinterpret_cast<CEnemyManager*>(Systems.back().get());


	PlayerPawn->AttachController(Controllers.CreateInputController(InGameContext.Input, InGameContext.MessageQueue));
	PlayerWorldObjectID = WorldState.AddToWorld<CTree>({0.f, 0.f}, { 32.f, 32.f });

	ReadPlayerPawnSpeedFromConfig();

	GetCamera().setCenter({ 0,0 });
	GetCamera().setSize(1920.f, 1080.f);

	EnemyTexture = InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/enemy_simple.png");
	EnemySpawner->SetTexture(EnemyTexture);

	AreaBG.SetTextureAsset(InGameContext.AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/area.png"));
	AreaBG.setOrigin(960+100, 540+100);
}

//------------------------------------------------------------------

void CTreeGameState::OnUpdate(SS::SGameContext& InGameContext)
{
	for (auto& system : Systems)
	{
		system->Update(InGameContext.Time.GetDeltaTime());
	}

	if (InGameContext.Input.IsKeyPressed(EKeyCode::Escape))
	{
		CState::PopAll();
	}

	if (InGameContext.Input.IsKeyPressed(EKeyCode::BackSpace))
	{
		InGameContext.MessageQueue.DispatchEvent<WaterMsg>();
	}

	//WorldState.SetWorldObjectPosition(PlayerWorldObjectID, PlayerPawn.GetPosition());

	Controllers.Update();
	PlayerPawn->Tick(InGameContext.Time.GetDeltaTime());

	if (Math::Length2(PlayerPawn->GetPosition()) < 100 * 100.f)
	{
		InGameContext.MessageQueue.DispatchEvent<ShowWaterPrompt>();
	}
	else
	{
		InGameContext.MessageQueue.DispatchEvent<HideWaterPrompt>();
	}


	myDebugDrawer.DrawCircle({}, 100.f, false, sf::Color(0,0,0, 200));

	auto lerp = GetCamera().getCenter();
	sf::Vector2f target = { PlayerPawn->GetPosition().x / 10, PlayerPawn->GetPosition().y / 10.f };
	lerp = lerp + InGameContext.Time.GetDeltaTime() * 5.f * (target - lerp);
	GetCamera().setCenter(lerp);
	
}

//------------------------------------------------------------------

void CTreeGameState::OnRender(SS::CRenderQueue& InRenderQueue)
{
	InRenderQueue.Enqueue(ERenderLayer::Background, SS::SSpriteRenderCommand(AreaBG));
	for (auto& system : Systems)
	{
		system->Render(InRenderQueue);
	}

	PlayerPawn->Draw(InRenderQueue);
}

//------------------------------------------------------------------

void CTreeGameState::ReadPlayerPawnSpeedFromConfig()
{
	SS::CConfigReader ConfigReader;
	ConfigReader.ReadConfigFile("player.cfg");

	PlayerPawn->SetSpeed(ConfigReader.GetAsFloat("speed"));
}

//------------------------------------------------------------------
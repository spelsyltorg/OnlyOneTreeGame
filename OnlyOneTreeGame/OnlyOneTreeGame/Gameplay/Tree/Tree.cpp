#include "Tree.h"

#include <SpelSylt/Messaging/MessageQueue.h>

#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/TextureAsset.h>

#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/Rendering/RenderCommand.h>

#include <SpelSylt/Math/CommonMath.h>

#include "GameJamGame/Gameplay/Pawn.h"
#include "GameJamGame/Core/GameMessages.h"

using namespace tree;

CTree::CTree(SS::CMessageQueue & aMsgQueue, SS::CAssetManager& aAssetManager, const CPawn& aPawn) : 
	myMsgQueue(aMsgQueue),
	myPlayerPawn(aPawn),
	myLevelProgressions{
		0, 4, 12, 24, 40
	}
{
	for (int i = 0; i < mySprites.size(); ++i)
	{
		mySprites[i].SetTextureAsset(aAssetManager.GetAsset<SS::STextureAsset>((std::string("Graphics/Sprites/tree_")+std::to_string(i)+".png").c_str()));
		mySprites[i].setOrigin(256, 512);
	}

	myMsgQueue.Subscribe<WaterMsg>([this](const auto& msg) {
		float distToPlayer = Math::Length2(myPlayerPawn.GetPosition());
		if (distToPlayer < 100.f * 100.f)
		{
			myWaterLevel += 10;
			UpdateTreeLevel();
		}
	}, mySubs);

	myMsgQueue.Subscribe<TreeAttackedMsg>([this](const auto & msg) {
		myWaterLevel -= 2;
		UpdateTreeLevel();
	}, mySubs);
}

tree::CTree::~CTree()
{
}

void tree::CTree::Render(SS::CRenderQueue & aRenderQueue)
{
	aRenderQueue.Enqueue(ERenderLayer::Foreground, SS::SSpriteRenderCommand(mySprites[myCurrentLevel]));
}

void tree::CTree::UpdateTreeLevel()
{
	myWaterLevel = Math::Max(0, myWaterLevel);

	for (int i = 0; i < myLevelProgressions.size(); ++i)
	{
		if ((myWaterLevel / 10) >= myLevelProgressions[i])
			myCurrentLevel = i;
	}

	if (myCurrentLevel < 4)
	{
		float currentReq = myLevelProgressions[myCurrentLevel];
		float nextReq = myLevelProgressions[myCurrentLevel + 1];
		float totalThisLevelReq = nextReq - currentReq;
		float current = (myWaterLevel/10) - myLevelProgressions[myCurrentLevel];
		myMsgQueue.DispatchEvent<WaterLevelMsg>(current / totalThisLevelReq);
	}
	else
	{
		myMsgQueue.DispatchEvent<WaterLevelMsg>(1.f);
	}
	myMsgQueue.DispatchEvent<TreeLevelMsg>(myCurrentLevel + 1);
}

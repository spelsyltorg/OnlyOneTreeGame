#include "GameJamGame/Core/AnimationSequencer.h"

#include <SpelSylt/Rendering/RenderQueue.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>
#include <SpelSylt/FileHandling/Asset/AssetTypes/AnimationAsset.h>

#include <SpelSylt/Rendering/RenderCommand.h>

//------------------------------------------------------------------

using namespace tree;

std::vector<SS::CSpriteAnimation> CAnimationSequencer::Animations;
SpelSylt::CAssetManager* CAnimationSequencer::AssetManager;

//------------------------------------------------------------------

CAnimationSequencer::CAnimationSequencer(SpelSylt::CAssetManager& InAssetManager)
{
	AssetManager = &InAssetManager;
}

//------------------------------------------------------------------

SS::CSpriteAnimation& CAnimationSequencer::PlayAnimationAtPosition(const char* InAnimation, const sf::Vector2f& InPosition)
{
	Animations.emplace_back();
	SS::CSpriteAnimation& NewAnimation = Animations.back();
	NewAnimation.SetPlayType(SS::EAnimationPlayType::OneShot);
	NewAnimation = AssetManager->GetAsset<SS::SAnimationAsset>(InAnimation);
	NewAnimation.setPosition(InPosition);
	NewAnimation.setOrigin(static_cast<float>(NewAnimation.GetFrameSize().x) / 2.f, static_cast<float>(NewAnimation.GetFrameSize().y) / 2.f);

	return NewAnimation;
}

//------------------------------------------------------------------

void CAnimationSequencer::Update(float aDT)
{
	std::vector<int> ToRemove;
	ToRemove.reserve(Animations.size());

	for (int i = 0; i < Animations.size(); ++i)
	{
		auto& Animation = Animations[i];

		Animation.Tick(aDT);

		if (Animation.IsFinished())
		{
			ToRemove.push_back(i);
		}
	}

	for (int i = ToRemove.size() - 1; i >= 0; --i)
	{
		int RemoveIndex = ToRemove[i];

		Animations.erase(Animations.begin() + RemoveIndex);
	}
}

//------------------------------------------------------------------

void CAnimationSequencer::Render(SpelSylt::CRenderQueue & aRenderQueue)
{
	for (auto& Animation : Animations)
	{
		aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteAnimationRenderCommand(Animation));
	}
}

//------------------------------------------------------------------
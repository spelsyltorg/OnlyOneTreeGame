#include "GameJamGame/Gameplay/Controller/InputController.h"

#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Utility/Input/InputEventGetter.h>

#include <SpelSylt/Messaging/MessageQueue.h>
#include "GameJamGame/Core/GameMessages.h"

#include "GameJamGame/Gameplay/Pawn.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CInputController::CInputController(const SS::IInputEventGetter& InInputGetter, SpelSylt::CMessageQueue& InMessageQueue)
	: InputGetter(InInputGetter)
	, MessageQueue(InMessageQueue)
	, CurrentVelocity(0.f, 0.f)
	, WeaponsLocked(false)
{
	InMessageQueue.Subscribe<LockWeaponsMsg>([this](auto msg) { WeaponsLocked = msg.Param; }, Subscriptions);
}

//------------------------------------------------------------------

void CInputController::RegisterPawn(const CPawn& InPawn) const
{
	OwningPawn = &InPawn;
}

//------------------------------------------------------------------

void CInputController::Update()
{
	const float Speed = OwningPawn->GetSpeed();

	CurrentVelocity = { 0.f, 0.f };

	if (InputGetter.IsKeyDown(EKeyCode::Up) || InputGetter.IsKeyDown(EKeyCode::W))
	{
		CurrentVelocity.y -= 1;
	}
	if (InputGetter.IsKeyDown(EKeyCode::Down) || InputGetter.IsKeyDown(EKeyCode::S))
	{
		CurrentVelocity.y += 1;
	}
	if (InputGetter.IsKeyDown(EKeyCode::Right) || InputGetter.IsKeyDown(EKeyCode::D))
	{
		CurrentVelocity.x += 1;
	}
	if(InputGetter.IsKeyDown(EKeyCode::Left) || InputGetter.IsKeyDown(EKeyCode::A))
	{
		CurrentVelocity.x -= 1;
	}
	if (InputGetter.IsKeyPressed(EKeyCode::Space))
	{
		MessageQueue.DispatchEvent<InteractMsg>();
	}
	if (InputGetter.IsKeyDown(EKeyCode::MouseLeft))
	{
		MessageQueue.DispatchEvent<FireWeaponMsg>(InputGetter.GetMousePosFloat());
	}

	if (!WeaponsLocked)
	{
		if (InputGetter.IsKeyDown(EKeyCode::One))
		{
			MessageQueue.DispatchEvent<SwitchWeaponMsg>(0);
		}
		if (InputGetter.IsKeyDown(EKeyCode::Two))
		{
			MessageQueue.DispatchEvent<SwitchWeaponMsg>(1);
		}
		if (InputGetter.IsKeyDown(EKeyCode::Three))
		{
			MessageQueue.DispatchEvent<SwitchWeaponMsg>(2);
		}
		if (InputGetter.IsKeyDown(EKeyCode::Four))
		{
			MessageQueue.DispatchEvent<SwitchWeaponMsg>(3);
		}
	}

	if (Math::Length2(CurrentVelocity) > 1.f)
	{
		Math::Normalize(CurrentVelocity);
	}

	CurrentVelocity *= Speed;
}

//------------------------------------------------------------------

const sf::Vector2f& tree::CInputController::GetVelocity() const
{
	return CurrentVelocity;
}

//------------------------------------------------------------------

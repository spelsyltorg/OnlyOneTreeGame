#include "GameJamGame/Gameplay/Pawn.h"

#include "GameJamGame/Gameplay/Controller/ControllerInterface.h"

#include <SpelSylt/Debugging/Logging/DebugLogger.h>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CPawn::CPawn()
	: Speed(0.f)
	, Controller(nullptr)
	, Position(0.f, 0.f)
{
}

//------------------------------------------------------------------

void CPawn::SetSpeed(float InSpeed)
{
	Speed = InSpeed;
}

//------------------------------------------------------------------

float CPawn::GetSpeed() const
{
	return Speed;
}

//------------------------------------------------------------------

void CPawn::AttachController(const IController& InController)
{
	Controller = &InController;
	Controller->RegisterPawn(*this);
}

//------------------------------------------------------------------

const IController& tree::CPawn::GetController() const
{
	return *Controller;
}

//------------------------------------------------------------------

void CPawn::Tick(float InDT)
{
	if (!Controller)
	{
		LOG_WARNING(CPawn, "Pawn was ticked with unset controller. Ensure you call AttachController to Pawns!");
		return;
	}

	sf::Vector2f Velocity = Controller->GetVelocity();
	Position += Velocity * InDT;
}

//------------------------------------------------------------------

void tree::CPawn::SetPositon(const sf::Vector2f& InPosition)
{
	Position = InPosition;
}

//------------------------------------------------------------------

const sf::Vector2f& CPawn::GetPosition() const
{
	return Position;
}

//------------------------------------------------------------------
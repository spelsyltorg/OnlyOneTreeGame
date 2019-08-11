#include "GameJamGame/Gameplay/Controller/AIController.h"

#include "GameJamGame/Gameplay/World/WorldProbe.h"

#include "GameJamGame/Gameplay/Tree/Tree.h"
#include "GameJamGame/Gameplay/Enemy/EnemyPawn.h"
#include "GameJamGame/Gameplay/Pawn.h"

#include "GameJamGame/Gameplay/World/ProbeConstructor.h"

#include <SpelSylt/Math/CommonMath.h>


//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CAIController::CAIController()
	: Probe(CProbeConstructor::ConstructProbe())
	, OwningPawn(nullptr)
	, GotTarget(false)
	, Velocity(0.f, 0.f)
	, Target(0.f, 0.f)
{
	WanderAngle = 0.f;
	Probe.AddFilter<CTree>();
	Probe.AddFilter<CEnemyPawn>();
}

//------------------------------------------------------------------

void CAIController::RegisterPawn(const CPawn& InPawn) const
{
	OwningPawn = &InPawn;
}

//------------------------------------------------------------------

void CAIController::Update()
{
	Probe.ProbeForData();

	const sf::Vector2f& ControlledPawnPosition = OwningPawn->GetPosition();
	sf::Vector2f Steering(0.f, 0.f);

	Probe.ForEachProbed([this, &Steering, &ControlledPawnPosition](const SWorldObject & InProbedObject) {
		
		if (Math::Length(ControlledPawnPosition - InProbedObject.Position) < 0.01f)
		{
			return;
		}

		if (InProbedObject.ObjectType == typeid(CTree).hash_code())
		{
			//Closest point on circle should be target
			if (!GotTarget)
			{
				sf::Vector2f ToTarget = ControlledPawnPosition - InProbedObject.Position;
				float ToTargetMag = Math::Length(ToTarget);
				Target = InProbedObject.Position + ToTarget / ToTargetMag * 64.f;
				GotTarget = true;
			}

			Steering += Seek(Velocity, ControlledPawnPosition, Target, OwningPawn->GetSpeed(), 1.f);
		}

		if (InProbedObject.ObjectType == typeid(CEnemyPawn).hash_code())
		{
	//		Steering += Flee(Velocity, ControlledPawnPosition, InProbedObject.Position, OwningPawn->GetSpeed(), 0.05f);
		}
		});

//	Steering += Wander(Velocity, ControlledPawnPosition, { 0.f,0.f }, OwningPawn->GetSpeed(), 10.f);

	Velocity += Steering;
	Math::Normalize(Velocity);
	Velocity *= OwningPawn->GetSpeed();
}

//------------------------------------------------------------------

const sf::Vector2f& CAIController::GetVelocity() const
{
	return Velocity;
}

//------------------------------------------------------------------

const sf::Vector2f& CAIController::GetTarget() const
{
	return Target;
}

//------------------------------------------------------------------

void CAIController::ResetTarget() const
{
	GotTarget = false;
}

//------------------------------------------------------------------


sf::Vector2f CAIController::Seek(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance)
{
	sf::Vector2f DesiredVelocity = Math::GetNormalized(TargetPosition - CurrentPosition) * InSpeed;
	return (DesiredVelocity - Direction) * InRelevance;
}

//------------------------------------------------------------------

sf::Vector2f CAIController::Flee(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance)
{
	sf::Vector2f DesiredVelocity = Math::GetNormalized(CurrentPosition - TargetPosition) * InSpeed;
	return (DesiredVelocity - Direction) * InRelevance;
}

//------------------------------------------------------------------

sf::Vector2f CAIController::Wander(const sf::Vector2f& Direction, const sf::Vector2f& CurrentPosition, const sf::Vector2f& TargetPosition, const float InSpeed, const float InRelevance)
{
	sf::Vector2f WanderCircleCenter = Direction;
	Math::Normalize(WanderCircleCenter);
	WanderCircleCenter *= 512.f;

	sf::Vector2f Displacement(0.f, -1.f);
	Displacement *= 128.f;

	float Len = Math::Length(Displacement);
	Displacement.x = cosf(WanderAngle) * Len;
	Displacement.y = sinf(WanderAngle) * Len;

	WanderAngle += (static_cast<float>(rand() % 100) / 100.f) * 10.f - 10.f * 0.5f;

	return (WanderCircleCenter += Displacement) * InRelevance;
}

//------------------------------------------------------------------

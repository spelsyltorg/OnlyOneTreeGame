#include "GameJamGame/Gameplay/Enemy/EnemyPawn.h"
#include "GameJamGame/Gameplay/Controller/AIController.h"

#include <SpelSylt/Math/CommonMath.h>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CEnemyPawn::CEnemyPawn()
	: CPawn()
	, ReachedTarget(false)
{
}

//------------------------------------------------------------------

void CEnemyPawn::Tick(float InDT)
{
	CPawn::Tick(InDT);
	const CAIController* ControllerAsAIController = reinterpret_cast<const CAIController*>(Controller);
	float DistanceToTarget = Math::Length2(ControllerAsAIController->GetTarget() - Position);

	if (DistanceToTarget < 0.1f)
	{
		ReachedTarget = true;
	}
}

//------------------------------------------------------------------

void CEnemyPawn::ResetTarget() const
{
	Controller->ResetTarget();
	ReachedTarget = false;
}

//------------------------------------------------------------------


bool CEnemyPawn::HasReachedTarget() const
{
	return ReachedTarget;
}

//------------------------------------------------------------------
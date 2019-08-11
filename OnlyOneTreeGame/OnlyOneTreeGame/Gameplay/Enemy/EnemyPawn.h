#pragma once
#include "GameJamGame/Gameplay/Pawn.h"

namespace tree
{
	class CEnemyPawn
		: public CPawn
	{
	public:
		CEnemyPawn();
		virtual void Tick(float InDT) override;

		bool HasReachedTarget() const;
		void ResetTarget() const;
	private:
		mutable bool ReachedTarget : 1;
	};
}
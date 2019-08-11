#pragma once

#define MAX_INPUT_CONTROLLERS 2
#define MAX_AI_CONTROLLERS 64
#define TOTAL_CONTROLLER_COUNT MAX_INPUT_CONTROLLERS + MAX_AI_CONTROLLERS //Add all others here

#include "GameJamGame/Gameplay/Controller/InputController.h"
#include "GameJamGame/Gameplay/Controller/AIController.h"

#include <vector>

namespace SpelSylt
{
	class IInputEventGetter;
	class CMessageQueue;
}

namespace tree
{
	class CInputController;

	class CControllerContainer
	{
	public:
		CControllerContainer();
		CInputController& CreateInputController(const SpelSylt::IInputEventGetter& InInputHandler, SpelSylt::CMessageQueue& InMessageQueue);
		CAIController& CreateAIController();
		void Update();
	private:
		std::vector<IController*> AllControllers;
		std::vector<CInputController> InputControllers;
		std::vector<CAIController> AIControllers;
	};
}
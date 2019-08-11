#include "OnlyOneTreeGame/GameApplication.h"
#include "OnlyOneTreeGame/OnlyOneTreeGameState.h"

void CGameApplication::SetUpWindow()
{
	//Todo: Set your own window size
	CreateWindow(800, 800);
	SetWindowTitle("Game Window");
}

void CGameApplication::PushStartUpStates()
{
	PushState(std::make_shared<COnlyOneTreeGameState>());
}

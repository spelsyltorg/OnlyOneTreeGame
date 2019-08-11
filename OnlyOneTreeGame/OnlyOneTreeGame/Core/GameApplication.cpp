#include "GameJamGame/Core/GameApplication.h"

#include "GameJamGame/States/TreeGameLoadingState.h"
//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

void CGameApplication::SetUpWindow()
{
	CApplication::SetWindowTitle("Game");
	CApplication::CreateWindow(1920, 1080, false, false);
}

//------------------------------------------------------------------

void CGameApplication::PushStartUpStates()
{
	CApplication::PushState(std::make_shared<CTreeGameLoadingState>(1920, 1080, "UI/Layouts/loading_layout.xml"));
}

//------------------------------------------------------------------
#include <SpelSylt/EngineStartUp.h>
#include "OnlyOneTreeGame/GameApplication.h"

int main()
{
	CGameApplication GameApp;

	GameApp.Initialize();
	bool ShouldRun = true;
	while(ShouldRun)
	{

		ShouldRun = GameApp.Run();
	}

	return 0;
}

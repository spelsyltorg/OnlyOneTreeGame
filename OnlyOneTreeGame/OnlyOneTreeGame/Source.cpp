#include <SpelSylt/EngineStartUp.h>
#include <SpelSylt/EngineMinimal.h>
#include <SpelSylt/Application.h>

#include "GameJamGame/Core/GameApplication.h"

//#define FPS_PROFILING_ENABLED

#ifdef FPS_PROFILING_ENABLED
#include "ittnotify.h"

#pragma comment(lib, "jitprofiling")
#pragma comment(lib, "libittnotify")
#pragma comment(lib, "prog_api")
#endif

void LoggingSetup()
{
	/*
	Uncomment the log level you want
	Verbose		= Log all logs
	Log			= Log all logs except verbose
	Warning		= Log warnings and errors only
	Error		= Log errors only

	Note: Only the last uncommented log level will be regarded!
	*/
	SET_LOG_LEVEL_VERBOSE;
	//SET_LOG_LEVEL_LOG;
	//SET_LOG_LEVEL_WARNING;
	//SET_LOG_LEVEL_ERROR;

	/* Uncomment this line to output all logs withing the set log level to file */
	//LOG_TO_FILE;
}

int main()
{
	LoggingSetup();

#ifdef FPS_PROFILING_ENABLED
	__itt_domain* Domain = __itt_domain_create("SpelSyltDomain");
	Domain->flags = 1;
#endif

	tree::CGameApplication GameApplication;
	GameApplication.Initialize();

	bool ShouldRun = true;
	while (ShouldRun)
	{
#ifdef FPS_PROFILING_ENABLED
		__itt_frame_begin_v3(Domain, NULL);
#endif

		ShouldRun = GameApplication.Run();

#ifdef FPS_PROFILING_ENABLED
		__itt_frame_end_v3(Domain, NULL);
#endif
	}

	return 0;
}
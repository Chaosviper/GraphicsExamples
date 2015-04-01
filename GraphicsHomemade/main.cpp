#include "ZbufferCubes.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	ZbufferCubes app;

	app.InitAll(hInstance, cmdShow);

	return app.Run();
}

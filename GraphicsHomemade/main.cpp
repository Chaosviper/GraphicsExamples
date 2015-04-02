#include "Lighting.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	Lighting app;

	app.InitAll(hInstance, cmdShow);

	return app.Run();
}

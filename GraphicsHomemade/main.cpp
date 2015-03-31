#include "Cube3D.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	Cube3D app;

	app.InitAll(hInstance, cmdShow);

	return app.Run();
}

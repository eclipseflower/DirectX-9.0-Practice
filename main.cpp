#include "main.h"
ToonEffectApp *app = new ToonEffectApp(width, height);

bool Display(float timeDelta)
{
	return app->Display(timeDelta);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	if (!D3DUtil::InitD3D(hInstance, width, height, true, D3DDEVTYPE_HAL, &app->device))
	{
		MessageBox(0, "InitD3D Fail", 0, 0);
		return 0;
	}

	if (!app->Setup())
	{
		MessageBox(0, "Setup Fail", 0, 0);
		return 0;
	}

	D3DUtil::EnterMsgLoop(Display);

	app->Cleanup();

	return 0;
}
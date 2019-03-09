#include "InitApp.h"

InitApp::InitApp()
{

}

InitApp::InitApp(int width, int height)
{
	cfgWidth = width;
	cfgHeight = height;
}

InitApp::~InitApp()
{
}

bool InitApp::Setup()
{
	return true;
}

void InitApp::Cleanup()
{
	device->Release();
}

bool InitApp::Display(float timeDelta)
{
	if (device != NULL)
	{
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		device->Present(0, 0, 0, 0);
	}
	return true;
}

void InitApp::LeftMouseDown(LPARAM lParam)
{
}

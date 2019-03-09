#pragma once
#include "D3DUtil.h"
#include "MathLib.h"

class InitApp
{
public:
	IDirect3DDevice9 *device = NULL;
	int cfgWidth, cfgHeight;

	InitApp();
	InitApp(int width, int height);
	~InitApp();
	virtual bool Setup();
	virtual void Cleanup();
	virtual bool Display(float timeDelta);
	void LeftMouseDown(LPARAM lParam);

private:

};
#pragma once

#include <d3dx9.h>

namespace D3DUtil 
{
	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, 
		IDirect3DDevice9 **device);

	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	template<class T> void Release(T t)
	{
		if (t != NULL) 
		{
			t->Release();
			t = NULL;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t != NULL)
		{
			delete t;
			t = NULL;
		}
	}

	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));

	void InitMaterial(D3DMATERIAL9 *pOut, D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
	DWORD FtoDw(float f);
	float GetRandomFloat(float lowBound, float highBound);
	void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);
}
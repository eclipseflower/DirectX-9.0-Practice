#include "D3DUtil.h"
#include "main.h"

bool D3DUtil::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType,
	IDirect3DDevice9 **device)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "InitApp";

	if (!RegisterClass(&wc)) 
	{
		MessageBox(0, "RegisterClass Fail", 0, 0);
		return false;
	}

	HWND hwnd = NULL;
	hwnd = CreateWindow("InitApp", "InitApp", WS_EX_TOPMOST, 0, 0, width, height, 0, 0, hInstance, 0);

	if (hwnd == NULL)
	{
		MessageBox(0, "CreateWindow Fail", 0, 0);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	HRESULT hr = 0;

	IDirect3D9 *d3d9 = NULL;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d9 == NULL)
	{
		MessageBox(0, "Direct3DCreate9 Fail", 0, 0);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, 
		device);
	if (FAILED(hr))
	{
		MessageBox(0, "CreateDevice Fail", 0, 0);
		return false;
	}
	d3d9->Release();
	return true;
}

int D3DUtil::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float curTime = (float)timeGetTime();
			float timeDelta = (curTime - lastTime) * 0.001f;
			ptr_display(timeDelta);
			lastTime = curTime;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK D3DUtil::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		break;

	case WM_LBUTTONDOWN:
		app->LeftMouseDown(lParam);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

}

void D3DUtil::InitMaterial(D3DMATERIAL9 * pOut, D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	pOut->Ambient = a;
	pOut->Diffuse = d;
	pOut->Specular = s;
	pOut->Emissive = e;
	pOut->Power = p;
}

DWORD D3DUtil::FtoDw(float f)
{
	return *((DWORD*)&f);
}

float D3DUtil::GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

void D3DUtil::GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}
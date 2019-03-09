#pragma once

#include <d3dx9.h>

// Build a lookat matrix. (left-handed)
D3DXMATRIX* MathLibMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp);

D3DXVECTOR3* MathLibPlaneNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *p0, const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2);

// Build a matrix which translates by (x, y, z)
D3DXMATRIX* MathLibMatrixTranslation(D3DXMATRIX *pOut, float x, float y, float z);

// Build a perspective projection matrix. (left-handed)
D3DXMATRIX* MathLibMatrixPerspectiveFovLH(D3DXMATRIX *pOut, float fovy, float Aspect, float zn, float zf);

// Build a matrix which rotates around the X axis
D3DXMATRIX* MathLibMatrixRotationX(D3DXMATRIX *pOut, float Angle);

// Build a matrix which rotates around the Y axis
D3DXMATRIX* MathLibMatrixRotationY(D3DXMATRIX *pOut, float Angle);
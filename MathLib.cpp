#include "MathLib.h"

D3DXMATRIX* MathLibMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp) 
{
	D3DXVECTOR3 forwardVec = *pAt - *pEye;
	D3DXVec3Normalize(&forwardVec, &forwardVec);
	D3DXVECTOR3 rightVec;
	D3DXVec3Cross(&rightVec, pUp, &forwardVec);
	D3DXVec3Normalize(&rightVec, &rightVec);
	D3DXVECTOR3 upVec;
	D3DXVec3Cross(&upVec, &forwardVec, &rightVec);
	D3DXVec3Normalize(&upVec, &upVec);

	D3DXVECTOR3 tranVec = -*pEye;
	D3DXVECTOR3 tmpVec{ D3DXVec3Dot(&tranVec, &rightVec), D3DXVec3Dot(&tranVec, &upVec), D3DXVec3Dot(&tranVec, &forwardVec) };
	*pOut = D3DXMATRIX(
		rightVec.x,	upVec.x,	forwardVec.x,	0,
		rightVec.y,	upVec.y,	forwardVec.y,	0,
		rightVec.z,	upVec.z,	forwardVec.z,	0,
		tmpVec.x,	tmpVec.y,	tmpVec.z,		1
	);

	return pOut;
}

D3DXVECTOR3* MathLibPlaneNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *p0, const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2)
{
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p1;
	D3DXVec3Cross(pOut, &u, &v);
	D3DXVec3Normalize(pOut, pOut);
	return pOut;
}

D3DXMATRIX* MathLibMatrixTranslation(D3DXMATRIX *pOut, float x, float y, float z) 
{
	*pOut = D3DXMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 0
	);
	return pOut;
}

D3DXMATRIX* MathLibMatrixPerspectiveFovLH(D3DXMATRIX *pOut, float fovy, float Aspect, float zn, float zf)
{
	float cot = 1.0f / tanf(fovy * 0.5f);
	*pOut = D3DXMATRIX(
		cot / Aspect,	0,		0,					0,
		0,				cot,	0,					0,
		0,				0,		zf / (zf - zn),		1,
		0,				0,		zf*zn / (zn - zf),	0
	);
	return pOut;
}

D3DXMATRIX* MathLibMatrixRotationX(D3DXMATRIX *pOut, float Angle)
{
	float sinVal = sinf(Angle);
	float cosVal = cosf(Angle);
	*pOut = D3DXMATRIX(
		1,	0,			0,		0,
		0,	cosVal,		sinVal,	0,
		0,	-sinVal,	cosVal,	0,
		0,	0,			0,		1
	);
	return pOut;
}

D3DXMATRIX* MathLibMatrixRotationY(D3DXMATRIX *pOut, float Angle)
{
	float sinVal = sinf(Angle);
	float cosVal = cosf(Angle);
	*pOut = D3DXMATRIX(
		cosVal,	0,	-sinVal,	0,
		0,		1,	0,			0,
		sinVal,	0,	cosVal,		0,
		0,		0,	0,			1
	);
	return pOut;
}

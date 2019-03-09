#pragma once
#include "CameraApp.h"
#include <list>

struct Particle
{
	D3DXVECTOR3 _position;
	D3DCOLOR _color;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

struct Attribute
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float _lifeTime;
	float _age;
	D3DXCOLOR _color;
	D3DXCOLOR _colorFade;
	bool _isAlive;

	Attribute()
	{
		_lifeTime = 0.0f;
		_age = 0.0f;
		_isAlive = true;
	}
};

struct BoundingBox
{
	D3DXVECTOR3 _min, _max;
	bool IsPointInside(D3DXVECTOR3& p);
};

class PSystemApp : public CameraApp
{
public:
	PSystemApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	virtual ~PSystemApp();
	virtual bool Init(IDirect3DDevice9 *device, const char *texFileName);
	virtual void Reset();
	virtual void ResetParticle(Attribute *attribute) = 0;
	virtual void AddParticle();
	virtual void Update(float timeDelta) = 0;
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	bool IsEmpty();
	bool IsDead();

protected:
	virtual void RemoveDeadParticles();
	IDirect3DDevice9 *_device;
	D3DXVECTOR3 _origin;
	BoundingBox _boundingBox;
	float _emitRate;
	float _size;
	IDirect3DTexture9 *_tex;
	IDirect3DVertexBuffer9 *_vb;
	std::list<Attribute> _particles;
	int _maxParticles;
	DWORD _vbSize;
	DWORD _vbOffset;
	DWORD _vbBatchSize;
};


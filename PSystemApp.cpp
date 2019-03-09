#include "PSystemApp.h"

bool BoundingBox::IsPointInside(D3DXVECTOR3& p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

PSystemApp::PSystemApp(int width, int height) :CameraApp(width, height)
{
	_device = NULL;
	_vb = NULL;
	_tex = NULL;
}


bool PSystemApp::Setup()
{
	return false;
}

void PSystemApp::Cleanup()
{
}

bool PSystemApp::Display(float timeDelta)
{
	return false;
}

PSystemApp::~PSystemApp()
{
}

bool PSystemApp::Init(IDirect3DDevice9 * device, const char * texFileName)
{
	_device = device;
	_device->CreateVertexBuffer(_vbSize * sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, Particle::FVF, D3DPOOL_DEFAULT, &_vb, 0);
	D3DXCreateTextureFromFile(_device, texFileName, &_tex);
	return false;
}

void PSystemApp::Reset()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); ++i)
	{
		ResetParticle(&*i);
	}
}

void PSystemApp::AddParticle()
{
	Attribute attribute;
	ResetParticle(&attribute);
	_particles.push_back(attribute);
}

void PSystemApp::PreRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSIZE, D3DUtil::FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, D3DUtil::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_A, D3DUtil::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, D3DUtil::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, D3DUtil::FtoDw(1.0f));
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void PSystemApp::Render()
{
	if (!_particles.empty())
	{
		PreRender();
		_device->SetTexture(0, _tex);
		_device->SetFVF(Particle::FVF);
		_device->SetStreamSource(0, _vb, 0, sizeof(Particle));
		if (_vbOffset >= _vbSize)
			_vbOffset = 0;
		Particle *v;
		_vb->Lock(_vbOffset * sizeof(Particle), _vbBatchSize * sizeof(Particle), (void **)&v, 
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		DWORD numParticlesInBatch = 0;
		std::list<Attribute>::iterator i;
		for (i = _particles.begin(); i != _particles.end(); ++i)
		{
			if (i->_isAlive)
			{
				v->_position = i->_position;
				v->_color = (D3DCOLOR)i->_color;
				v++;
				numParticlesInBatch++;
				if (numParticlesInBatch == _vbBatchSize)
				{
					_vb->Unlock();
					_device->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, _vbBatchSize);
					_vbOffset += _vbBatchSize;
					if (_vbOffset >= _vbSize)
						_vbOffset = 0;
					_vb->Lock(_vbOffset * sizeof(Particle), _vbBatchSize * sizeof(Particle), (void **)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					numParticlesInBatch = 0;
				}
			}
		}
		_vb->Unlock();
		if (numParticlesInBatch)
		{
			_device->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, numParticlesInBatch);
		}
		_vbOffset += _vbBatchSize;
		PostRender();
	}
}

void PSystemApp::PostRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool PSystemApp::IsEmpty()
{
	return _particles.empty();
}

bool PSystemApp::IsDead()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); ++i)
	{
		if (i->_isAlive == true)
			return false;
	}
	return true;
}

void PSystemApp::RemoveDeadParticles()
{
	std::list<Attribute>::iterator i = _particles.begin();
	while(i != _particles.end())
	{
		if (i->_isAlive == false)
		{
			i = _particles.erase(i);
		}
		else
		{
			++i;
		}
	}
}

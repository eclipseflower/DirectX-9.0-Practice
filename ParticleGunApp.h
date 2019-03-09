#pragma once
#include "PSystemApp.h"
class ParticleGunApp : public PSystemApp
{
public:
	ParticleGunApp(int width, int height);
	bool Setup() override;
	bool Display(float timeDelta) override;
	void Cleanup() override;
	void ResetParticle(Attribute *attribute);
	void Update(float timeDelta);
	~ParticleGunApp();
};


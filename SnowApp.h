#pragma once
#include "PSystemApp.h"
class SnowApp : public PSystemApp
{
public:
	SnowApp(int width, int height);
	bool Setup() override;
	bool Display(float timeDelta) override;
	void Cleanup() override;
	void ResetParticle(Attribute *attribute);
	void Update(float timeDelta);
	~SnowApp();
};


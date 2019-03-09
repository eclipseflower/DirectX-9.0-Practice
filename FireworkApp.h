#pragma once
#include "PSystemApp.h"
class FireworkApp : public PSystemApp
{
public:
	FireworkApp(int width, int height);
	bool Setup() override;
	bool Display(float timeDelta) override;
	void Cleanup() override;
	void ResetParticle(Attribute *attribute);
	void Update(float timeDelta);
	void PreRender();
	void PostRender();
	~FireworkApp();
};


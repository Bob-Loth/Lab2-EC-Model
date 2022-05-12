#pragma once
#include "Component.h"
#include "Transform.h"

class LightComponent : Component
{
public:
	LightComponent(vec3 sunDir) : Component("LightComponent"), sunDir(sunDir) {}
	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager* compMan);
	static LightComponent& GetInstance(vec3 sunDir) {
		static LightComponent instance(sunDir);
		return instance;
	}
	// Sun rotation around x axis
	float sunRotation = 0;
	vec3 sunDir = vec3(0, 0, 1);
private:
	float timeLeft = 60;
	float startSunRotation = 7;
	float endSunRotation = 5.982;
	float startTime = 60;
};
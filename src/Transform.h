#pragma once
#include "Component.h"
class ComponentManager;
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


class Transform : public Component
{
public:
	Transform(std::string name) : Component(name) {}

	vec3 GetPos();
	quat GetRot();
	vec3 GetScale();
	const mat4 GetModelMat() const { return M; };

	void SetPos(vec3);
	void SetRot(quat);
	void SetScale(vec3);

	void ApplyTranslation(vec3);
	void ApplyRotation(float, vec3);
	void ApplyScale(vec3);

	void Update(float frameTime);
	void Init(ComponentManager& manager);

	void CalcModelMat();
private:
	vec3 position;
	quat rotation;
	vec3 scale;
	mat4 M = mat4(1.0f);
};


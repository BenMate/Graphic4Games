#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

class FlyCamera : public Camera
{
public:
	virtual void Update();
	void SetSpeed(float speed) { m_speed = speed; };

private:

	float m_speed = 1.5f;
	glm::vec3 up = glm::vec3(0, 1 ,0);

};


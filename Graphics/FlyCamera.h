#pragma once
#include <glm/glm.hpp>

#include "Camera.h"
#include "Input.h"

class FlyCamera : public Camera
{
public:
	virtual void Update(float deltaTime);
	void SetSpeed(float speed) { m_speed = speed; };

private:

	float m_speed = 1.5f;
	glm::vec3 up = glm::vec3(0, 1 ,0);

};


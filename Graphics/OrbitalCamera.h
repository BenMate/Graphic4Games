#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

enum class RotationalDirection
{
	CW = -1,
	CCW = 1,
	NONE = 0

};

class OrbitalCamera : public Camera
{
	OrbitalCamera();
	OrbitalCamera(float a_rotationSpeed, float a_distFromTarget, RotationalDirection a_rotationalDir);
	~OrbitalCamera();

	virtual void Update(float deltaTime);


	void SetSpeed(float speed) { m_speed = speed; };
	float GetSpeed() { return m_speed; }
	glm::vec3 GetPos() { return m_pos; }


	float m_speed = 1.5f;;
	float m_rotationSpeed = 1;
	float m_rotation;
	float m_distanceFromCenter;

	RotationalDirection m_rotationalDir;

	glm::vec3 m_pos = glm::vec3(0);
	glm::vec4 m_colour = glm::vec4(0);
};


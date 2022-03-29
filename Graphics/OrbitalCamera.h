#pragma once

#include <glm/glm.hpp>
#include "Camera.h"


enum class RotationalDirection
{
	CW = -1,
	CCW = 1,
	NONE = 0
};

class OrbitalCamera : public Camera
{
public:
	OrbitalCamera(float a_rotationSpeed,float a_rotation, float a_distFromTarget, RotationalDirection a_rotationalDir);
	~OrbitalCamera();

	virtual void Update(float deltaTime);

protected:

	float m_speed = 1.5f;;
	float m_rotationSpeed = 1;
	float m_rotation = 0;
	float m_distanceFromCenter = 0;

	RotationalDirection m_rotationalDir;

};


#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera(float a_rotationSpeed,float a_rotation, float a_distFromTarget, RotationalDirection a_rotationalDir) :
	m_rotationSpeed(a_rotationSpeed), m_rotation(a_rotation), m_distanceFromCenter(a_distFromTarget), m_rotationalDir(a_rotationalDir)
{

}

OrbitalCamera::~OrbitalCamera()
{

}

void OrbitalCamera::Update(float deltaTime)
{

	m_rotation += (m_rotationalDir == RotationalDirection::CW ? m_rotationSpeed : -m_rotationSpeed) * deltaTime;
	m_position = glm::vec3(cos(m_rotation), 0, sin(m_rotation)) * m_distanceFromCenter;

}

#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera()
{

}

OrbitalCamera::OrbitalCamera(float rotationSpeed,float dist, RotationalDirection rotationalDir) :
	m_rotationSpeed(rotationSpeed), m_distanceFromCenter(dist), m_rotationalDir(rotationalDir)
{

}

OrbitalCamera::~OrbitalCamera()
{
}

void OrbitalCamera::Update(float deltaTime)
{

	m_rotation += (m_rotationalDir == RotationalDirection::CW ? m_rotationSpeed : -m_rotationSpeed) * deltaTime;
	m_pos = glm::vec3(cos(m_rotation), 0, sin(m_rotation)) * m_distanceFromCenter;

}

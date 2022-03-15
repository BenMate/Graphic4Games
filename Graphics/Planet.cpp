#include "Planet.h"
#include "Gizmos.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Planet::Planet()
{

}

Planet::Planet(float size, float rotationSpeed,float rotation, float dist, RotationalDirection rotationalDir, glm::vec4 colour) :
	m_size(size), m_rotationSpeed(rotationSpeed), m_rotation(rotation), m_distanceFromCenter(dist), m_rotationalDir(rotationalDir), m_colour(colour)
{

}

Planet::~Planet()
{

}

void Planet::Update(float dt)
{
	//rotate around center 

	m_rotation += (m_rotationalDir == RotationalDirection::CW ? m_rotationSpeed : -m_rotationSpeed) * dt;
	m_pos = glm::vec3(cos(m_rotation), 0, sin(m_rotation)) * m_distanceFromCenter;
}

void Planet::Draw()
{
	aie::Gizmos::addSphere(m_pos, m_size, 8, 8, m_colour);
	//aie::Gizmos::addDisk(m_pos, m_size, 10, m_colour);
	//aie::Gizmos::addRing(m_pos, m_size / 2, m_size, 10, m_colour);
	//aie::Gizmos::addArcRing(m_pos, m_rotation, m_size / 2, m_size, 90, 4, m_colour);
}

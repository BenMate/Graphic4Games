#pragma once

#include <glm/glm.hpp>

enum class RotationalDirection
{
	CW = -1,
	CCW = 1,
	NONE = 0
	
};

class Planet
{

public:

	Planet();
	Planet(float size, float rotationSpeed, float rotation, float dist, RotationalDirection rotationalDir, glm::vec4 colour);
	~Planet();

	void Update(float dt);
	void Draw();

	glm::vec3 GetPos() { return m_pos; }


protected:


	float m_size = 1;
	float m_rotationSpeed = 1;
	float m_rotation;
	float m_distanceFromCenter;

	RotationalDirection m_rotationalDir;

	glm::vec3 m_pos = glm::vec3(0);
	glm::vec4 m_colour = glm::vec4(0);

private:



};


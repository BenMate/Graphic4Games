#include "SolarSystem.h"
#include "Planet.h"


SolarSystem::SolarSystem()
{
	//creates a list of planets
	//size, rotationspeed, rotation, distancefromcenter, rotationdir, position, colour

	Planet* sun = new Planet(1.5, 0, 0, 0, RotationalDirection::NONE, glm::vec4(1,0.15f,0.1f,1));
	Planet* mercary = new Planet(0.2f, 1.1f, 1, 3, RotationalDirection::CCW, glm::vec4(0.3,0.3,0.3,1));
	Planet* venus = new Planet(0.4f, 1.15f, 1, 3.5, RotationalDirection::CCW, glm::vec4(0.4, 0.4, 0.4, 1));
	Planet* earth = new Planet(0.5f, 1, 1, 4, RotationalDirection::CCW, glm::vec4(0,1,1,1));
	Planet* mars = new Planet(0.25f, 0.8f, 1, 4.5f, RotationalDirection::CCW, glm::vec4(1, 0, 0, 1));
	Planet* jupitor = new Planet(1.1f, 0.5f, 1, 5.5f, RotationalDirection::CCW, glm::vec4(1, 1, 0, 1));
	Planet* saturn = new Planet(0.8f, 0.35f, 1, 6.5f, RotationalDirection::CCW, glm::vec4(1, 1, 0.5f, 1));
	Planet* uranus = new Planet(0.6f, 0.25f, 1, 7.5f, RotationalDirection::CCW, glm::vec4(0, 0.6f, 1, 1));
	Planet* neptune = new Planet(0.55f, 0.2f, 1, 8.5f, RotationalDirection::CCW, glm::vec4(0, 0.3f, 1, 1));

	m_planet.push_back(sun);
	m_planet.push_back(mercary);
	m_planet.push_back(venus);
	m_planet.push_back(earth);
	m_planet.push_back(mars);
	m_planet.push_back(jupitor);
	m_planet.push_back(saturn);
	m_planet.push_back(uranus);
	m_planet.push_back(neptune);

	//todo: add moons and rings
	
}

SolarSystem::~SolarSystem()
{
	//deletes all planets in list
	m_planet.clear();
}

void SolarSystem::Update(float dt)
{
	for (auto& planets : m_planet)
	{
		planets->Update(dt);
	}
}

void SolarSystem::Draw()
{
	for (auto& planets : m_planet)
	{
		planets->Draw();
	}
}

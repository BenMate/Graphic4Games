#pragma once
#include <vector>

class Planet;

class SolarSystem
{

	

public:

	SolarSystem();
	~SolarSystem();

	void Update(float dt);
	void Draw();


protected:

private:

	std::vector<Planet*> m_planet;

};


#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light
{
	Light() {}
	Light(glm::vec3 pos, glm::vec3 col, float intensity) 
	{
		direction = pos;
		colour = col * intensity;
	}
	
	glm::vec3 direction;
	glm::vec3 colour;
};

class Scene
{
public:
	
	Scene(Camera* a_camera, glm::vec2 a_windowSize, 
		Light& a_light, glm::vec3 a_ambientLight);
	~Scene();

	void AddInstance(Instance* a_instance);
	void Draw();

	Camera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }

	Light& GetGlobalLight() { return m_globalDirLight; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
	std::vector<Light>& GetPointLight() { return m_pointLights; }
	int GetNumLights() { return (int)m_pointLights.size(); }

	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColours() { return &m_pointLightColours[0]; }

	void AddPointLight(Light a_light) { m_pointLights.push_back(a_light); }
	void AddPointLight(glm::vec3 a_direction, glm::vec3 a_colour, float a_intencity)
	{ m_pointLights.push_back(Light(a_direction, a_colour, a_intencity)); }

protected:

	//camera
	Camera* m_camera;

	//lights
	Light m_globalDirLight;
	glm::vec3 m_ambientLight;
	std::vector<Light> m_pointLights;

	//instances
	std::list<Instance*> m_instances;
	glm::vec2 m_windowSize;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
};

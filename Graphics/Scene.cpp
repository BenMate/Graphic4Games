#include "Scene.h"
#include "Instance.h"
#include <imgui.h>
#include <string>
#include <Gizmos.h>

Scene::Scene(Camera* a_camera, glm::vec2 a_windowSize,
	Light& a_light, glm::vec3 a_ambientLight) :
	m_camera(a_camera), m_windowSize(a_windowSize),
	m_globalDirLight(a_light), m_ambientLight(a_ambientLight)
{

}

Scene::~Scene()
{
	//for every instance
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete (*it);
	}
}

void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

void Scene::Draw()
{
	//for every point light
	for (int i = 0; i < MAX_LIGHTS && i < GetNumLights(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;

		aie::Gizmos::addSphere(m_pointLightPositions[i], 0.1f, 10, 10, glm::vec4(m_pointLightColours[i], 1));
	}

	//for every instance
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::Update(float a_deltaTime)
{
	ImGui::Begin("Instances Data");
	//for every instance
	int i = 0;
	for (auto it = m_instances.begin(); it != m_instances.end(); it++, i++)
	{
		Instance* instance = *it;

		glm::vec3 pos = instance->GetPosition();
		ImGui::DragFloat3(std::string("Position##").append(std::to_string(i)).c_str(), &pos[0], 0.4f, -20, 20); 
		instance->SetPosition(pos);
		
		glm::vec3 rotation = instance->GetRotation();
		ImGui::DragFloat3(std::string("Rotation##").append(std::to_string(i)).c_str(), &rotation[0], 0.4f, -20, 20);
		instance->SetRotation(rotation);

		glm::vec3 scale = instance->GetScale();
		ImGui::DragFloat3(std::string("Scale##").append(std::to_string(i)).c_str(), &scale[0], 0.4f, -20, 20);
		instance->Setscale(scale);


		ImGui::Spacing();
	}

	ImGui::End();
}

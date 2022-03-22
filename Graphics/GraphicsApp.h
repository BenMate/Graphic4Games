#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"

#include "SolarSystem.h"
#include "Camera.h"



class SolarSystem;

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	bool LaunchShaders();
	void CamControls(float deltaTime);

	bool havePlanets = false;
	


	glm::mat4 m_modelTransform;


	// camera transforms
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	//todo: make vectors of m_camera
	//pushback new flyCamera etc.... make an index value 
	//look in imgui for each camera etc...
	Camera* m_camera;

	SolarSystem* m_solarSystem;

	//textures
	aie::Texture m_gridTexture;
	aie::Texture m_spearTexture;
	aie::Texture m_gunTexture;

	//shader data
	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normalMapShader;

	//soulspear data
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	//gun data
	aie::OBJMesh m_gunMesh;
	glm::mat4 m_gunTransform;

	//generic quad data
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	//standford bunny data
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	//box data
	Mesh m_boxMesh;
	glm::mat4 m_boxTransform;

	//pyromid data
	Mesh m_pyromidMesh;
	glm::mat4 m_pyromidTransform;

	float m_zoom = 10.0f;
	float m_speed = 10.0f;
	float m_rotation = 0.0f;
	float m_rotSpeed = 45.f;
	glm::vec3 m_position = glm::vec3(0);

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 colour;
	};

	Light m_light;
	glm::vec3 m_ambientLight;
};
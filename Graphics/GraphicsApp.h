#pragma once

#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"

#include "Camera.h"
#include "FlyCamera.h"
#include "OrbitalCamera.h"

#include "RenderTarget.h"
#include "ParticleEmitter.h"

#include "SolarSystem.h"

#include <glm/mat4x4.hpp>

#include <vector>


class Scene;
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

	bool havePlanets = false;
	
	float m_zoom = 10.0f;
	float m_speed = 10.0f;
	float m_rotation = 0.0f;
	float m_rotSpeed = 45.f;
	glm::vec3 m_position = glm::vec3(0);

	glm::mat4 m_modelTransform;

	// camera transforms
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	//todo: make vectors of m_camera
	//pushback new flyCamera etc.... make an index value 
	//look in imgui for each camera etc...
	std::vector <Camera*> m_camera;
	int m_cameraIndex = 1;

	SolarSystem* m_solarSystem;

	//textures
	aie::Texture m_gridTexture;
	aie::Texture m_spearTexture;
	aie::Texture m_gunTexture;
	aie::Texture m_marbleTexture;
	aie::Texture m_hatchingTexture;
	aie::Texture m_rampTexture;

	//shader data
	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_postShader;
	aie::ShaderProgram m_particleShader;

	//render data
	aie::RenderTarget m_rendarTarget;

	int m_postProcessEffect = 1;

	int m_pixelAmount = 100;

	//for post processing
	Mesh m_screenQuad;

	//soulspear data
	aie::OBJMesh	m_spearMesh;
	glm::mat4		m_spearTransform;

	//gun data
	aie::OBJMesh	m_gunMesh;
	glm::mat4		m_gunTransform;

	//generic quad data
	Mesh			m_quadMesh;
	glm::mat4		m_quadTransform;

	//standford bunny data
	aie::OBJMesh	m_bunnyMesh;
	glm::mat4		m_bunnyTransform;

	//box data
	Mesh m_boxMesh;
	glm::mat4		m_boxTransform;

	//pyromid data
	Mesh			m_pyromidMesh;
	glm::mat4		m_pyromidTransform;

	//light data
	glm::vec3		m_ambientLight;

	//scene data
	Scene*			m_scene;

	//particle data
	ParticleEmitter* m_emitter;
	glm::mat4		 m_particleTransform;

	void InitialiseOurParticles();
	void DrawOurParticles(glm::mat4 a_pvm);
	
};
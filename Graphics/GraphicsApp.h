#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "Mesh.h"

#include "SolarSystem.h"

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

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	SolarSystem* m_solarSystem;

	aie::ShaderProgram m_shader;

	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

};
#include <iostream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "OBJMesh.h"
#include "Mesh.h"
#include "Camera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp()
{

}

GraphicsApp::~GraphicsApp()
{

}

bool GraphicsApp::startup()
{

	havePlanets = false;

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//start the scene with a default lighting.
	m_light.colour = { 1, 1, 1 };
	m_light.direction = { 1, -1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	//create a new solarSystem
	if (havePlanets)
		m_solarSystem = new SolarSystem();

	//create camaera
	m_camera = new Camera();

	return LaunchShaders();
}

void GraphicsApp::shutdown()
{
	Gizmos::destroy();
	if (havePlanets)
		m_solarSystem->~SolarSystem();
}

void GraphicsApp::update(float deltaTime)
{
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Grab the time since the application started
	//float time = getTime();

	// Rotate the light
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0)); */


	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Global Light Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Global Light colour", &m_light.colour[0], 0.1, 0.0f, 2.0f);
	ImGui::End();

	//update solarSystem
	if (havePlanets) m_solarSystem->Update(deltaTime);

	//update camera
	m_camera->Update(deltaTime);
}

void GraphicsApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	//draw the solar system
	if (havePlanets)
		m_solarSystem->Draw();

	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(),
		(float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

    #pragma region Object Shaders
	//bind the shader
	m_phongShader.bind();
	m_modelTransform = m_bunnyTransform;

	m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	m_phongShader.bindUniform("LightColour", m_light.colour);
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("CameraPosition", glm::vec3(glm::inverse(m_viewMatrix)[3]));

	//bind the transform
	auto pvm = projectionMatrix * viewMatrix * m_modelTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("ModelMatrix", m_modelTransform);

	//draw the bunny
	m_bunnyMesh.draw();

	//====================================================
	// Soul Spear 
	m_texturedShader.bind();
	pvm = projectionMatrix * viewMatrix * m_spearTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	m_spearMesh.draw();
	//=====================================================

	//bind the phong shader
	m_texturedShader.bind();
	m_modelTransform = m_quadTransform;
	pvm = projectionMatrix * viewMatrix * m_modelTransform;
	//simple binding for lighting data based on model used
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	//bind the texture at the location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	//bind the texture to the specific location
	m_gridTexture.bind(0);
	//draw quad
	m_quadMesh.Draw();
#pragma endregion

	Gizmos::draw(projectionMatrix * viewMatrix);
}

bool GraphicsApp::LaunchShaders()
{
 #pragma region Shaders
	//simple shader
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");
	if (m_shader.link() == false)
		printf("simple Shader Error: %s\n", m_shader.getLastError());

	//phong shader
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");
	if (m_phongShader.link() == false)
		printf("phong shader Error: %s\n", m_phongShader.getLastError());

	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Textured shader Error: s\n", m_texturedShader.getLastError());
		return false;
	}
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("failed to load the texture, please check file path\n");
		return false;
	}

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.2,  0.5, 1 };
	vertices[1].position = { 0.5f, 0.2,  0.5, 1 };
	vertices[2].position = { -0.5f, 0.2, -0.5, 1 };
	vertices[3].position = { 0.5f, 0.2, -0.5, 1 };

	unsigned int indices[6] = { 0, 1 ,2  ,2 ,1 ,3 };

#pragma endregion

	//quad
	m_quadMesh.InitialiseQuad();
	m_quadTransform = { 10, 0, 0, 0,
						0, 10, 0, 0,
						0, 0, 10, 0,
						0, 0, 0, 1 }; //this is 10 units large

	//bunny
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny mesh Error!\n");
		return false;
	}
	m_bunnyTransform = { 0.5, 0, 0, 0,
						 0, 0.5, 0, 0,
						 0, 0, 0.5, 0,
						 0, 0, 0, 1 };

	//spear
	if (m_spearMesh.load("./soulspear/soulspear.obj",
		true, true) == false)
	{
		printf("SoulSpear mesh error!\n");
		return false;
	}
	m_spearTransform = { 1, 0, 0, 0,
						 0, 1, 0, 0,
					     0, 0, 1, 0,
					 	 0, 0, 0, 1 };

	return true;
}

void GraphicsApp::CamControls(float deltaTime)
{
	//temp controls
	aie::Input* input = aie::Input::getInstance();

	//Input UP/DOWN - ZOOM
	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_zoom -= 0.5f;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_zoom += 0.5f;

	//Input LEFT/RIGHT - ROTATION
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_rotation += deltaTime * glm::radians(m_rotSpeed);

	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_rotation -= deltaTime * glm::radians(m_rotSpeed);

	vec3 forward = -vec3(cos(m_rotation), 0, sin(m_rotation));
	vec3 right = vec3(forward.z, 0, -forward.x);

	//Input W/S - Z MOVE
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime * m_speed;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * deltaTime * m_speed;

	//Input A/D - X MOVE
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position += right * deltaTime * m_speed;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position -= right * deltaTime * m_speed;

	m_viewMatrix = glm::lookAt(vec3(cos(m_rotation), 1, sin(m_rotation)) * m_zoom + m_position, m_position, vec3(0, 1, 0));



}


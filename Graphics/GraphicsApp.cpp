#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "OBJMesh.h"


#include <glm/glm.hpp>
#include <glm/ext.hpp>




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

	//create a new solarSystem
	if (havePlanets) m_solarSystem = new SolarSystem();

	return LaunchShaders();
}

void GraphicsApp::shutdown()
{

	Gizmos::destroy();
	if (havePlanets) m_solarSystem->~SolarSystem();
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

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	//update solarsystem
	if (havePlanets) m_solarSystem->Update(deltaTime);


	CamControls(deltaTime);
}

void GraphicsApp::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	//draw the solar system
	if (havePlanets) m_solarSystem->Draw();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	//bind the shader
	m_shader.bind();

	//bind the transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_pyromidTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	//draw the quad
	//m_quadMesh.Draw();
	//m_boxMesh.Draw();
	m_pyromidMesh.Draw();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool GraphicsApp::LaunchShaders()
{
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_shader.link() == false)
		printf("simple Shader Error: %s\n", m_shader.getLastError());

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.2,  0.5, 1 };
	vertices[1].position = { 0.5f, 0.2,  0.5, 1 };
	vertices[2].position = { -0.5f, 0.2, -0.5, 1 };
	vertices[3].position = { 0.5f, 0.2, -0.5, 1 };

	unsigned int indices[6] = { 0, 1 ,2
								,2 ,1 ,3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);
	m_quadTransform = { 10, 0, 0, 0,
						0, 10, 0, 0,
						0, 0, 10, 0,
						0, 0, 0, 1 }; //this is 10 units large

	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny mesh Error!\n");
		return false;
	}

	m_bunnyTransform = { 0.5, 0, 0, 0,
						 0, 0.5, 0, 0,
						 0, 0, 0.5, 0,
						 0, 0, 0, 1 };


	m_boxMesh.InitialiseBox();
	m_boxTransform = {   0.5, 0, 0, 0,
						 0, 0.5, 0, 0,
						 0, 0, 0.5, 0,
						 0, 0, 0, 1 };

	m_pyromidMesh.InitialisePryomid();
	m_pyromidTransform = { 1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };


	return true;
}

void GraphicsApp::CamControls(float deltaTime)
{
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


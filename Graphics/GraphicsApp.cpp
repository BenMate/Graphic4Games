
#include <imgui.h>

#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Instance.h"
#include "Scene.h"


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
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	//create camaera
	m_camera.push_back(new Camera()); // stationary camera
	m_camera.push_back(new FlyCamera()); // player controlled camera

	m_camera[1]->SetPosition({ -10, 5, 0 });

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//start the scene with a default lighting.
	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, -1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_scene = new Scene(m_camera[m_cameraIndex], glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);

	//add a point light into the scene
	m_scene->AddPointLight(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLight(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	//m_scene->AddInstance(new Instance(m_spearTransform, &m_spearMesh, &m_normalMapShader));

	return LaunchShaders();
}

void GraphicsApp::shutdown()
{
	Gizmos::destroy();

	delete m_scene;
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

#pragma region IMGUI
	//GUI
	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Global Light Direction", &m_scene->GetGlobalLight().direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Global Light colour", &m_scene->GetGlobalLight().colour[0], 0.1, 0.0f, 2.0f);
	ImGui::End();

	//Point light gui
	ImGui::Begin("PointLight Settings");
	for (int i = 0; i < m_scene->GetNumLights(); i++)
	{
		ImGui::DragFloat3("Point Light Direction", &m_scene->GetPointLight()[i].direction[0], 0.1f, -1.0f, 1.0f);
		ImGui::DragFloat3("Point Light colour", &m_scene->GetPointLight()[i].colour[0], 0.1, 0.0f, 2.0f);
	}
	ImGui::End();

	//Camera Control gui
	FlyCamera* m_flyCam = dynamic_cast<FlyCamera*>(m_camera[m_cameraIndex]);
	ImGui::Begin("Camera Settings");
	ImGui::DragInt("Camera", &m_cameraIndex, 1, 0, m_camera.size() - 1);
	ImGui::Checkbox("Cam Debug Mode", &m_camera[m_cameraIndex]->m_debugMode);
	//get and set the new speed
	if (m_flyCam != nullptr)
	{
		float speed = m_flyCam->GetSpeed();
		ImGui::DragFloat("Fly Cam Speed", &speed, 0.1f, 1, 5);
		m_flyCam->SetSpeed(speed);
	}
	ImGui::End();

	//shader effects editor
	//GraphicsApp* shaderEffect = dynamic_cast<GraphicsApp*>();
	//int shaderIndex = shaderEffect->GetProccessEffectsIndex();

	//ImGui::Begin("Shader Index");
	//ImGui::DragInt("postProcessEffect", &shaderEffect, 0.1f, 1, 5);
	//
	//shaderEffect->SetProcessEffect(shaderIndex);
	//ImGui::End();
	
#pragma endregion

	m_scene->SetCamera(m_camera[m_cameraIndex]);

	//update camera
	m_camera[m_cameraIndex]->Update(deltaTime);
}

void GraphicsApp::draw()
{
	// we need to bind our render target first
	m_rendarTarget.bind();
	// wipe the screen to the background colour
	clearScreen();

	for (int i = 0; i < m_camera.size(); i++)
	{
		m_camera[i]->Draw();
	}

	// update perspective based on screen size
	glm::mat4 projectionMatrix = m_camera[m_cameraIndex]->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	glm::mat4 viewMatrix = m_camera[m_cameraIndex]->GetViewMatrix();
	auto pvm = projectionMatrix * viewMatrix * mat4(1);

	m_scene->Draw();
	m_rendarTarget.unbind();
	clearScreen();

	#pragma region Quad
	// Texture Quad
	m_texturedShader.bind();
	m_modelTransform = m_quadTransform;
	pvm = projectionMatrix * viewMatrix * m_modelTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Draw the quad
	m_gridTexture.bind(0);
	//m_rendarTarget.getTarget(0).bind(0);
	m_quadMesh.Draw();
#pragma endregion

	Gizmos::draw(projectionMatrix * viewMatrix);
	// Unbind the target to return it to the back of the buffer
	//m_rendarTarget.unbind();
	//clearScreen();
	
	// Bind the post processing shader and texture
	m_postShader.bind();
	m_postShader.bindUniform("colourTarget", 0);
	m_postShader.bindUniform("postProcessTarget", m_postProcessEffect);
	m_rendarTarget.getTarget(0).bind(0);
	m_screenQuad.Draw();
	
}

bool GraphicsApp::LaunchShaders()
{
	if (m_rendarTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
				printf("Render Target: error \n");
				return false;
	}

#pragma region Simple Shaders
	//simple shader
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");
	if (m_shader.link() == false)
		printf("simple Shader Error: %s\n", m_shader.getLastError());
#pragma endregion

#pragma region Phongshader
	//phong shader
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");
	if (m_phongShader.link() == false)
		printf("phong shader Error: %s\n", m_phongShader.getLastError());
#pragma endregion

#pragma region TexturedShader
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Textured shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("failed to load the texture, please check file path\n");
		return false;
	}
#pragma endregion

#pragma region normalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("normal map has a  shader error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region PostShader
	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/advancedPost.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/advancedPost.frag");

	if (m_postShader.link() == false)
	{
		printf("Post prossesing shader has had an error: %s\n", m_postShader.getLastError());
		return false;
	}

#pragma endregion

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.2,  0.5, 1 };
	vertices[1].position = { 0.5f, 0.2,  0.5, 1 };
	vertices[2].position = { -0.5f, 0.2, -0.5, 1 };
	vertices[3].position = { 0.5f, 0.2, -0.5, 1 };

	unsigned int indices[6] = { 0, 1 ,2  ,2 ,1 ,3 };

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
	m_bunnyTransform = { 0.1, 0, 0, 0,
						 0, 0.1, 0, 0,
						 0, 0, 0.1, 0,
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

	//gun
	if (m_gunMesh.load("./gun/gun.obj", true, true) == false) 
	{
		printf("gun mesh error!\n");
		return false;
	}
	m_gunTransform = { 0.3, 0, 0, 0,
					   0, 0.3, 0, 0,
					   0, 0, 0.3, 0,
					   0, 0, 0, 1 };

	//create the fullscreen quad for post precessing effects
	m_screenQuad.InitialiseFullScreenQuad();

	//for x amount, add instances.
	for (int i = 0; i < 1; i++)
	{
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0),glm::vec3(0, 0, 0),
			glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
		m_scene->AddInstance(new Instance(glm::vec3(i * 4, 0, i * 5), glm::vec3(i * 20, 0, 0),
			glm::vec3(1, 1, 1), &m_gunMesh, &m_normalMapShader));
	}
	
	return true;
}

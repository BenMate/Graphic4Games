
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

	m_camera.push_back(new Camera()); // stationary camera
	m_camera.push_back(new FlyCamera()); // player controlled camera
	m_camera.push_back(new OrbitalCamera(1.0f, 1, 10.0f, RotationalDirection::CW));

	m_camera[1]->SetPosition({ -10, 5, 0 });

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	//start the scene with a default lighting.
	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = { 1, -1, 1 };
	m_ambientLight = { 0.5f, 0.5f, 0.5f };

	m_scene = new Scene(m_camera[m_cameraIndex], glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);

	//add a point light into the scene
	m_scene->AddPointLight(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLight(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	InitialiseOurParticles();

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
	ImGui::ColorEdit3("Global Light colour", &m_scene->GetGlobalLight().colour[0]);
	ImGui::End();

	//Point light gui
	ImGui::Begin("PointLight Settings");
	for (int i = 0; i < m_scene->GetNumLights(); i++)
	{
		ImGui::DragFloat3(std::string("Point Light Position##").append(std::to_string(i)).c_str(), &m_scene->GetPointLight()[i].direction[0], 0.1f);
		ImGui::DragFloat3(std::string("Point Light colour##").append(std::to_string(i)).c_str(), &m_scene->GetPointLight()[i].colour[0], 0.1, 0.0f, 2.0f);
		ImGui::Spacing();
	}
	ImGui::End();

	//Camera Control gui
	FlyCamera* m_flyCam = dynamic_cast<FlyCamera*>(m_camera[m_cameraIndex]);
	ImGui::Begin("Camera Settings");
	ImGui::DragInt("Camera", &m_cameraIndex, 0.01f, 0, m_camera.size() - 1);
	ImGui::Checkbox("Cam Debug Mode", &m_camera[m_cameraIndex]->m_debugMode);

	//get and set the new speed
	if (m_flyCam != nullptr)
	{
		float speed = m_flyCam->GetSpeed();
		ImGui::DragFloat("Fly Cam Speed", &speed, 0.1f, 1.0f, 50.0f);
		m_flyCam->SetSpeed(speed);
	}
	ImGui::End();

	//shader effects editor
	ImGui::Begin("Shader Index");
	ImGui::DragInt("postProcessEffect", &m_postProcessEffect, 0.05f, 0, 11);
	if (m_postProcessEffect == 1)
		ImGui::DragFloat("BlurAmount", &m_blurStrength, 0.05f, 1, 11);
	
	if (m_postProcessEffect == 8) //pixel effect
		ImGui::DragInt("pixelAmount", &m_pixelAmount, 0.5f, 10, 250);
	ImGui::End();
	
	//emittor
	ImGui::Begin("Emitter Settings");

	if (ImGui::Button("Press to Toggle Emitter"))
		m_emitter->ToggleEmit();
	
	glm::vec3 pos = m_emitter->GetPosition();
	ImGui::DragFloat3("Emitter Position", &pos[0], 0.1f);
	m_emitter->SetPosition(pos);

	ImGui::End();

#pragma endregion

	//set and update current camera
	m_scene->SetCamera(m_camera[m_cameraIndex]);
	m_camera[m_cameraIndex]->Update(deltaTime);

	//update emitter 
	m_emitter->Update(deltaTime, m_camera[m_cameraIndex]->GetTransform(m_camera[m_cameraIndex]->GetPosition(), glm::vec3(0), glm::vec3(1)));
	
	m_scene->Update(deltaTime);
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

#pragma region BunnyTests
	m_phongShader.bind();
	m_phongShader.bindUniform("LightDirection", m_scene->GetGlobalLight().direction);
	m_phongShader.bindUniform("AmbientColour", m_scene->GetAmbientLight());
	m_phongShader.bindUniform("LightColour", m_scene->GetGlobalLight().colour);
	m_phongShader.bindUniform("CameraPosition", m_camera[m_cameraIndex]->GetPosition());

	pvm = projectionMatrix * viewMatrix * m_bunnyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);

	m_marbleTexture.bind(0);
	m_phongShader.bindUniform("seamlessTexture", 0);

	m_hatchingTexture.bind(1);
	m_phongShader.bindUniform("hatchingTexture", 1);

	m_rampTexture.bind(2);
	m_phongShader.bindUniform("rampTexture", 2);

	m_bunnyMesh.draw();
#pragma endregion

	m_scene->Draw();
	Gizmos::draw(projectionMatrix * viewMatrix);

	//particles
	pvm = projectionMatrix * viewMatrix * m_particleTransform;
	DrawOurParticles(pvm);

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

	m_rendarTarget.getTarget(0).bind(0);
	m_quadMesh.Draw();
#pragma endregion

	
	
	// Bind the post processing shader and texture
	m_postShader.bind();
	m_postShader.bindUniform("colourTarget", 0);
	m_postShader.bindUniform("postProcessTarget", m_postProcessEffect);
	m_postShader.bindUniform("pixelAmount", m_pixelAmount);
	m_postShader.bindUniform("blurStrength", m_blurStrength);

	m_rendarTarget.getTarget(0).bind(0);
	m_screenQuad.Draw();
	
}

bool GraphicsApp::LaunchShaders()
{
	//initialise render target
	if (m_rendarTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
				printf("Render Target: error \n");
				return false;
	}

	//load vertex
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phongExt.vert");
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/advancedPost.vert");
	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/particle.vert");

	//load fragment
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongExt.frag");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/advancedPost.frag");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/particle.frag");

	//link shaders
	if (m_shader.link() == false) printf("simple Shader Error: %s\n", m_shader.getLastError());
	if (m_phongShader.link() == false) printf("phong shader Error: %s\n", m_phongShader.getLastError());
	if (m_texturedShader.link() == false)
	{
		printf("Textured shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	if (m_normalMapShader.link() == false)
	{
		printf("normal map has a  shader error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
	if (m_postShader.link() == false)
	{
		printf("Post prossesing shader has had an error: %s\n", m_postShader.getLastError());
		return false;
	}
	if (m_particleShader.link() == false)
	{
		printf("Particle shader has an error: %s \n", m_particleShader.getLastError());
		return false;
	}

	//load textures
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("failed to load the texture, please check file path\n");
		return false;
	}
	m_marbleTexture.load("./textures/marble2.jpg");
	m_hatchingTexture.load("./textures/Ramp02.png");
	m_rampTexture.load("./textures/ramps.png", true);

	//load mesh
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny mesh Error!\n");
		return false;
	}
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("SoulSpear mesh error!\n");
		return false;
	}
	if (m_gunMesh.load("./gun/gun.obj", true, true) == false)
	{
		printf("gun mesh error!\n");
		return false;
	}

	//mesh indices
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.2,  0.5, 1 };
	vertices[1].position = { 0.5f, 0.2,  0.5, 1 };
	vertices[2].position = { -0.5f, 0.2, -0.5, 1 };
	vertices[3].position = { 0.5f, 0.2, -0.5, 1 };
	unsigned int indices[6] = { 0, 1 ,2  ,2 ,1 ,3 };

	//transforms
	m_quadMesh.InitialiseQuad();
	m_quadTransform = { 10, 0, 0, 0,
						0, 10, 0, 0,
						0, 0, 10, 0,
						0, 0, 0, 1 }; 
	//this is 10 units large
	m_bunnyTransform = { 2, 0, 0, 0,
						 0, 2, 0, 0,
						 0, 0, 2, 0,
						 0, 0, 0, 2 };
	m_spearTransform = { 1, 0, 0, 0,
						 0, 1, 0, 0,
					     0, 0, 1, 0,
					 	 0, 0, 0, 1 };
	m_gunTransform = { 0.3, 0, 0, 0,
					   0, 0.3, 0, 0,
					   0, 0, 0.3, 0,
					   0, 0, 0, 1 };
	m_particleTransform = { 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1 };

	//create the fullscreen quad for post precessing effects
	m_screenQuad.InitialiseFullScreenQuad();

#pragma region AddingInstancing

	m_spearPosition = glm::vec3(20, 0, 0);
	m_scene->AddInstance(new Instance(m_spearPosition, glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));

	for (int i = 0; i < 2; i++)
	{
		m_scene->AddInstance(new Instance(glm::vec3(i * 4, 0, i * 5), glm::vec3(i * 20, 0, 0),
			glm::vec3(1, 1, 1), &m_gunMesh, &m_normalMapShader));
	}

#pragma endregion

	return true;
}

void GraphicsApp::InitialiseOurParticles()
{
	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, 0.1, 1.0f, 1.0f, 10.0f, 2.0f, 0.1f, glm::vec4(0, 1, 1, 1), glm::vec4(1, 1, 0, 1));

}

void GraphicsApp::DrawOurParticles(glm::mat4 a_pvm)
{
	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", a_pvm);
	m_emitter->Draw();
}

#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{


public:
	
	Camera();
	~Camera();

	virtual void Update(float deltaTime);
	virtual void Draw();

	//getters
	glm::mat4 GetWorldTransform() const { return m_worldTransform; }
	glm::mat4 GetProjectionView() const { return m_projectionViewTransform; }
	glm::vec3 GetPosition() const { return m_position; }
	glm::mat4 GetView() const { return m_viewTransform; }
	
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	glm::mat4 GetTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	//setters
	void SetPosition(glm::vec3 newPos) { m_position = newPos; }
	void SetPerspective(float fov, float aspectRatio, float near, float far);
	void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	
	bool m_debugMode = true;

protected:

	glm::vec3 m_position = glm::vec3(-20.0f, 2.0f, 0.0f);

	float m_theta = 0;
	float m_phi = 0;

	float m_lastMouseX;
	float m_lastMouseY;

	const float m_turnspeed = 0.05f;

private:
	void UpdateProjectionViewTransform();

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
};


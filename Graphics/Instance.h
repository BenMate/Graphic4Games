#pragma once

#include <glm/glm.hpp>

class Camera;
struct Light;
class Scene;

namespace aie 
{
	class OBJMesh;
	class ShaderProgram;

}


class Instance
{
public:

	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh,
		aie::ShaderProgram* a_shader);

	Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles,
		glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);

	void Draw(Scene* a_scene);

	static glm::mat4 MakeTransform(glm::vec3 a_position, 
		glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	glm::mat4 GetTransform();
	glm::vec3 GetScale() { return m_scale; }
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetRotation() { return m_rotation; }

	void SetPosition(glm::vec3 a_position) { m_position = a_position; }
	void SetRotation(glm::vec3 a_rotation) { m_rotation = a_rotation; }
	void Setscale(glm::vec3 a_scale) { m_scale = a_scale; }


protected:

	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;

	glm::vec3 m_rotation;
	glm::vec3 m_position;
	glm::vec3 m_scale;
};


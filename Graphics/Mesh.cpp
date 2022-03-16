#include <gl_core_4_4.h>
#include "Mesh.h"

Mesh::~Mesh()
{
	// if the array does not equal zero, then delte the data
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::InitialiseQuad()
{
	//check if the mesh is not initialised already 
	assert(m_vao == 0);

	//generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	//bind the vertex array. this is our mesh wrapper
	glBindVertexArray(m_vao);

	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//define 6 vertices for our two triangles
	Vertex vertices[6];

	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = { 0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f,  0,  -0.5f, 1 };
	vertices[4].position = { 0.5f,  0,   0.5f, 1 };
	vertices[5].position = { 0.5f,  0,  -0.5f, 1 };


	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

		//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//enable the second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//now unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//this quad is made of 2 triangles
	m_triCount = 2;
}

void Mesh::InitialiseBox()
{
	Mesh::Vertex vertices[8];

	//top vert
	vertices[0].position = { -0.5f, 0.5,  0.5, 1 };
	vertices[1].position = { 0.5f,  0.5,  0.5, 1 };
	vertices[2].position = { -0.5f, 0.5, -0.5, 1 };
	vertices[3].position = { 0.5f,  0.5, -0.5, 1 };

	//bottom vert
	vertices[4].position = { -0.5f, -0.5,  0.5, 1 };
	vertices[5].position = { 0.5f,  -0.5,  0.5, 1 };
	vertices[6].position = { -0.5f, -0.5, -0.5, 1 };
	vertices[7].position = { 0.5f,  -0.5, -0.5, 1 };


	unsigned int indices[36] = {
		0, 1 ,2,  2 ,1 ,3,
		5, 7, 3,  3, 1, 5,
		0, 4, 1,  4, 5, 1,
		0, 6, 4,  0, 2, 6,
		6, 2, 3,  3, 7, 6,
		7, 5, 4,  4, 6, 7,
	};

	Initialise(8, vertices, 36, indices);
}

void Mesh::InitialisePryomid()
{
	Mesh::Vertex vertices[5];

	//top vert
	vertices[0].position = { 0, 0.5,  0, 1 };

	//bottom vert
	vertices[1].position = { -0.5f, -0.5,  0.5, 1 };
	vertices[2].position = { 0.5f,  -0.5,  0.5, 1 };
	vertices[3].position = { -0.5f, -0.5, -0.5, 1 };
	vertices[4].position = { 0.5f,  -0.5, -0.5, 1 };

	unsigned int indices[18] = {
		1, 3, 4,  1, 4, 2,
		2, 0, 1,  0, 4, 3,
		0, 3, 1,  0, 2, 4,
	};

	Initialise(5, vertices, 18, indices);
}

void Mesh::initialiseGrid()
{
	//make a quad

	//add rows and collums to make more quads

	//todo: everything


}

void Mesh::Initialise(unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_indexCount, unsigned int* a_indices)
{
	//check if the mesh is not initialised already
	assert(m_vao == 0);

	//generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	//bind the vertex array, this is our mesh wrapper
	glBindVertexArray(m_vao);

	//bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, a_vertexCount * sizeof(Vertex),
		a_vertices, GL_STATIC_DRAW);

	//enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	if (a_indexCount != 0)
	{
		glGenBuffers(1, &m_ibo);
		//bind the vertex buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		//fill the vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_indexCount * sizeof(unsigned int),
			a_indices, GL_STATIC_DRAW);

		m_triCount = a_indexCount / 3;
	}
	else
		m_triCount = a_vertexCount / 3;

	//now unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	//are we actually using the indices or vertices?
	if (m_ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);

}



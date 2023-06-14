#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>

struct vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 tex_uv;
};

class VBO
{
	GLuint id_ = 0;
	reference_counter* count_;
public:
	VBO()
	{
		count_ = new reference_counter(1);
	}

	explicit VBO(const std::vector<vertex>& vertices)
	{
		count_ = new reference_counter(1);
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
	}

	explicit VBO(const int size)
	{
		count_ = new reference_counter(1);
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VBO(const VBO& obj): id_(obj.id_), count_(obj.count_)
	{
		count_->add_ref();
	}

	void bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_);
	}

	void unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void dispose() const
	{
		glDeleteBuffers(1, &id_);
	}

	~VBO()
	{
		if(count_->release() == 0)
		{
			delete count_;
			dispose();
		}
	}
};
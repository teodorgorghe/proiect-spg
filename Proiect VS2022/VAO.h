#pragma once

#include <GL/glew.h>

#include "reference_counter.h"
#include "VBO.h"

class VAO
{
	reference_counter* count_;
	GLuint id_ = 0;
public:
	VAO()
	{
		count_ = new reference_counter(1);
		glGenVertexArrays(1, &id_);
	}

	VAO(const VAO& obj): count_(obj.count_), id_(obj.id_)
	{
		count_->add_ref();
	}

	void link_attribute(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset) const
	{
		vbo.bind();
		glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		vbo.unbind();
	}
	
	void bind() const
	{
		glBindVertexArray(id_);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	void dispose() const
	{
		glDeleteVertexArrays(1, &id_);
	}

	~VAO()
	{
		if (count_->release() == 0)
		{
			delete count_;
			this->dispose();
		}
	}
};

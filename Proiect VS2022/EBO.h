#pragma once

#include <vector>
#include <GL/glew.h>
#include "reference_counter.h"

class EBO
{
	reference_counter* count_;
	GLuint id_ = 0;
public:
	EBO(const std::vector<GLuint>& indices)
	{
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
		count_ = new reference_counter(1);
	}

	EBO(const EBO& obj): count_(obj.count_), id_(obj.id_)
	{
		count_->add_ref();
	}

	void bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
	}
	
	void unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void dispose()
	{
		glDeleteBuffers(1, &id_);
	}

	~EBO()
	{
		if(count_->release() == 0)
		{
			delete count_;
			dispose();
		}
	}
};

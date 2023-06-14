#pragma once

#include <GL/glew.h>
#include "Shader.h"

class texture
{
public:
	GLuint id = 0;
	const char* type;
	GLuint unit;

	texture(const char* image, const char* tex_type, GLuint slot, GLenum format, GLenum pixel_type, GLint internal_format = GL_RGBA);

	texture(const unsigned char* bytes, const int width, const int height, const char* tex_type, GLuint slot, GLenum format, GLenum pixel_type, GLint internal_format = GL_RGBA);

	void tex_unit(const shader& shader, const char* uniform, const GLuint unit);

	void bind() const;

	void unbind();

	void dispose() const;
};

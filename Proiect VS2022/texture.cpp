#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture::texture(const char* image, const char* tex_type, GLuint slot, GLenum format, GLenum pixel_type,
	GLint internal_format)
{
	type = tex_type;

	int width_img, height_img, num_col_ch;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &width_img, &height_img, &num_col_ch, 0);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_img, height_img, 0, format, pixel_type, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);
}

texture::texture(const unsigned char* bytes, const int width, const int height, const char* tex_type, GLuint slot,
	GLenum format, GLenum pixel_type, GLint internal_format)
{
	type = tex_type;

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, pixel_type, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::tex_unit(const shader& shader, const char* uniform, const GLuint unit)
{
	const GLuint tex_uni = glGetUniformLocation(shader.id, uniform);
	shader.activate();
	glUniform1i(tex_uni, unit);
}

void texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::dispose() const
{
	glDeleteTextures(1, &id);
}

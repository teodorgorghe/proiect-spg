#pragma once
#include <map>
#include <string>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

#include "Camera.h"
#include "drawable.h"
#include "texture.h"
#include "VAO.h"

struct character
{
	texture		 texture;
	glm::ivec2   size;
	glm::ivec2   bearing;
	unsigned int advance;
};

class text: public drawable
{
	std::map<unsigned char, character> characters_;
	VAO vao_;
	VBO vbo_;
	shader shader_;
	glm::mat4 projection_;

public:
	text(const char* font_face, const std::string& text, int x, int y, float scale, glm::vec3 color = glm::vec3(0, 0, 0));

	void draw(const camera&) override;
	~text() override;

	std::string displayed_text;
	glm::vec3 color;
	int x;
	int y;
	float scale;
};

#pragma once
#include "Camera.h"
#include "drawable.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"

class dashed_line : public drawable
{
	float dash_size_;
	float gap_size_;
	VAO vao_;
	VBO vbo_;
	shader shader_;
	glm::vec3 color_;

public:
	dashed_line(glm::vec3 start_pos, glm::vec3 end_pos, glm::vec3 color, float dash_size, float gap_size);

	void draw(const camera&) override;
	void change_line(glm::vec3, glm::vec3) const;
	~dashed_line() override = default;
};

#pragma once
#include "Camera.h"
#include "drawable.h"
#include "Shader.h"
#include "VAO.h"

class circle: public drawable
{
	shader shader_;
	float cx_;
	float line_;
	float cy_;
	float radius_;
	glm::vec3 fill_color_;
	glm::vec3 line_color_;
	std::vector<vertex> points_;
	int num_segments_;
	VBO vbo_;
	VAO vao_;

public:
	circle(float cx, float cy, float radius, float line = 0.0f, glm::vec3 fill_color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 line_color = glm::vec3(0.0f, 0.0f, 0.0f), int num_segments = 64);

	void draw(const camera&) override;
	void update_points();
	void set_center(float cx, float cy);
	~circle() override = default;
};

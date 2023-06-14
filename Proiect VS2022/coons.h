#pragma once

#include <vector>
#include <glm/detail/type_vec.hpp>

#include "Camera.h"
#include "circle.h"
#include "dashed_line.h"
#include "drawable.h"
#include "shader.h"
#include "VAO.h"
#include "VBO.h"

class coons: public drawable
{
	std::vector<vertex> points_;
	std::vector<circle> circles_;
	std::vector<dashed_line> lines_;
	size_t size_;

	glm::vec3 p1_;
	glm::vec3 p2_;
	glm::vec3 t1_;
	glm::vec3 t2_;
	glm::vec3 color_;

	VBO vbo_;
	VAO vao_;
	shader shader_;
	
public:
	coons(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, size_t size, glm::vec3 color = glm::vec3(0, 0, 0));

	void draw(const camera&) override;
	void update_points();

	void set_p1(const glm::vec3&);
	void set_p2(const glm::vec3&);
	void set_t1(const glm::vec3&);
	void set_t2(const glm::vec3&);

	glm::vec3 get_p1() const;
	glm::vec3 get_p2() const;
	glm::vec3 get_t1() const;
	glm::vec3 get_t2() const;
};

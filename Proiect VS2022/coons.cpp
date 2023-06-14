﻿#include "coons.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

coons::coons(glm::vec3 p1, glm::vec3 p2, glm::vec3 t1, glm::vec3 t2, const size_t size, const glm::vec3 color):
	points_(size), size_(size), p1_(p1), p2_(p2), t1_(t1), t2_(t2), color_(color), vbo_(points_), shader_("vertex.vert", "fragment.vert")
{
	vao_.bind();
	vao_.link_attribute(vbo_, 0, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, position)));
	vao_.link_attribute(vbo_, 1, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));

	update_points();
	vao_.unbind();
	vbo_.unbind();
}

void coons::draw(const camera& camera)
{
	shader_.activate();
	vao_.bind();

	glUniformMatrix4fv(glGetUniformLocation(shader_.id, "modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(camera.camera_matrix));

	glDrawArrays(GL_LINE_STRIP, 0, points_.size());
	vao_.unbind();
}

void coons::set_p1(const glm::vec3& p1)
{
	p1_ = p1;
	update_points();
}

void coons::set_p2(const glm::vec3& p2)
{
	p2_ = p2;
	update_points();
}

void coons::set_t1(const glm::vec3& t1)
{
	t1_ = t1;
	update_points();
}

void coons::set_t2(const glm::vec3& t2)
{
	t2_ = t2;
	update_points();
}

glm::vec3 coons::get_p1() const
{
	return p1_;
}

glm::vec3 coons::get_p2() const
{
	return p2_;
}

glm::vec3 coons::get_t1() const
{
	return t1_;
}

glm::vec3 coons::get_t2() const
{
	return t2_;
}

void coons::update_points()
{
	for(size_t i = 0; i < size_; i++)
	{
		const float u = static_cast<float>(i) / (size_ - 1);
		const float u2 = u * u;
		const float u3 = u2 * u;
		const float f0 = 2 * u3 - 3 * u2 + 1;
		const float f1 = -2 * u3 + 3 * u2;
		const float f2 = u3 - 2 * u2 + u;
		const float f3 = u3 - u2;

		const float x = f0 * p1_[0] + f1 * p2_[0] + f2 * t1_[0] + f3 * t2_[0];
		const float y = f0 * p1_[1] + f1 * p2_[1] + f2 * t1_[1] + f3 * t2_[1];
		const float z = f0 * p1_[2] + f1 * p2_[2] + f2 * t1_[2] + f3 * t2_[2];

		points_[i].position = glm::vec3(x, y, z);
		points_[i].color = color_;
	}
	vbo_.bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_ * sizeof(vertex), points_.data());
	vbo_.unbind();
}

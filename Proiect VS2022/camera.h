#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
	glm::mat4 inverse_matrix_;
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -20.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 camera_matrix = glm::mat4(1.0f);

	bool first_click = true;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	camera(const int width, const int height, const glm::vec3 position): position(position), width(width), height(height) {}

	void update_matrix(const float fo_vdeg, const float near_plane, const float far_plane)
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(position, position + orientation, up);
		projection = glm::perspective(glm::radians(fo_vdeg), static_cast<float>(width) / height, near_plane, far_plane);

		camera_matrix = projection * view;
		inverse_matrix_ = inverse(camera_matrix);
	}

	void update_matrix_ortho()
	{
		camera_matrix = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
		inverse_matrix_ = inverse(camera_matrix);
	}

	glm::vec3 transform_2d_to_3d(const glm::vec2& coords) const
	{
		const float normalized_x = (2.0f * coords.x / width) - 1.0f;
		const float normalized_y = 1.0f - (2.0f * coords.y / height);

		const glm::vec4 clip_point = inverse_matrix_ * glm::vec4(normalized_x, normalized_y, 0.0f, 1.0f);
		return glm::vec3(clip_point) / clip_point.w;
	}
};

#include "dashed_line.h"

dashed_line::dashed_line(const glm::vec3 start_pos, const glm::vec3 end_pos, const glm::vec3 color, const float dash_size, const float gap_size):
	dash_size_(dash_size), gap_size_(gap_size), vbo_(std::vector<vertex>
	{
		vertex{ start_pos, color, glm::vec3(0, 0, 0), glm::vec2(0, 0) },
		vertex{ end_pos, color, glm::vec3(0, 0, 0), glm::vec2(0, 0) }
	}), shader_("vertex-line.vert", "fragment-line.vert"), color_(color)
{
	vao_.bind();
	vao_.link_attribute(vbo_, 0, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, position)));
	vao_.link_attribute(vbo_, 1, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));

	vao_.unbind();
}

void dashed_line::draw(const camera& camera)
{
	shader_.activate();
	vao_.bind();

	glUniformMatrix4fv(glGetUniformLocation(shader_.id, "modelViewProjectionMatrix"), 1, GL_FALSE, value_ptr(camera.camera_matrix));
	glUniform1f(glGetUniformLocation(shader_.id, "u_dashSize"), dash_size_);
	glUniform1f(glGetUniformLocation(shader_.id, "u_gapSize"), gap_size_);
	glUniform2f(glGetUniformLocation(shader_.id, "u_resolution"), static_cast<float>(camera.width), static_cast<float>(camera.height));

	glDrawArrays(GL_LINES, 0, 2);

	vao_.unbind();
}

void dashed_line::change_line(const glm::vec3 start_pos, const glm::vec3 end_pos) const
{
	vbo_.bind();
	const std::vector<vertex> updated_buffer =
	{
		vertex{start_pos, color_, glm::vec3(0, 0, 0), glm::vec2(0, 0)},
		vertex{end_pos, color_, glm::vec3(0, 0, 0), glm::vec2(0, 0)}
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(vertex), updated_buffer.data());
}

#include "circle.h"

circle::circle(const float cx, const float cy, const float radius, const float line, const glm::vec3 fill_color, const glm::vec3 line_color, const int num_segments):
	shader_("vertex-circle.vert", "fragment-circle.vert"), cx_(cx), line_(line), cy_(-cy), radius_(radius),
	fill_color_(fill_color), line_color_(line_color), points_(num_segments + 2), num_segments_(num_segments), vbo_(points_)
{
    update_points();

    vao_.bind();
    vao_.link_attribute(vbo_, 0, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, position)));
    vao_.link_attribute(vbo_, 1, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));
    //vao_.link_attribute(vbo_, 2, 3, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, normal)));
    //vao_.link_attribute(vbo_, 3, 2, GL_FLOAT, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, tex_uv)));

    vao_.unbind();
}

void circle::draw(const camera& camera)
{
    shader_.activate();
    vao_.bind();

    glUniformMatrix4fv(glGetUniformLocation(shader_.id, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.camera_matrix));
    glUniform1f(glGetUniformLocation(shader_.id, "radius"), radius_);
    glUniform1f(glGetUniformLocation(shader_.id, "line"), line_);
    glUniform2fv(glGetUniformLocation(shader_.id, "center"), 1, glm::value_ptr(glm::vec2(cx_, cy_)));
    glUniform3fv(glGetUniformLocation(shader_.id, "fill_color"), 1, glm::value_ptr(fill_color_));

    glDrawArrays(GL_TRIANGLE_FAN, 0, points_.size());

    vao_.unbind();
}

void circle::update_points()
{
    const float theta = 3.1415926f * 2.0f / static_cast<float>(num_segments_);
    const float tangential_factor = tanf(theta);

    const float radial_factor = cosf(theta);

    float x = radius_;
    float y = 0;
    points_[0] = vertex{ glm::vec3(cx_, cy_, 0.0f), glm::vec3(fill_color_), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
    for (int i = 0; i < num_segments_; i++)
    {
        points_[i + 1] = vertex{ glm::vec3(x + cx_, y + cy_, -0.5f), line_color_, glm::vec3(x, y, 0.0f), glm::vec2(0.0f, 0.0f) };

        const float tx = -y;
        const float ty = x;

        x += tx * tangential_factor;
        y += ty * tangential_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    points_.back() = points_[1];

    vbo_.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, points_.size() * sizeof(vertex), points_.data());
    vbo_.unbind();
}

void circle::set_center(const float cx, const float cy)
{
    cx_ = cx;
    cy_ = -cy;
    update_points();
}
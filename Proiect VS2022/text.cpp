#include "text.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>

text::text(const char* font_face, const std::string& text, const int x, const int y, const float scale,
           const glm::vec3 color): vbo_(sizeof(float) * 6 * 4), shader_("vertex-text.vert", "fragment-text.vert"),
									displayed_text(text), color(color), x(x), y(y), scale(scale)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        throw std::exception("FONT subsystem init failed!");
    }

    FT_Face face;
    if (FT_New_Face(ft, font_face, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        throw std::exception("FONT init failed!");
    }

    FT_Set_Pixel_Sizes(face, 0, 32);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        const texture texture(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, "CHAR", 0, GL_RED, GL_UNSIGNED_BYTE, GL_RED);

        character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters_.insert_or_assign(c, character);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    vao_.bind();
    vao_.link_attribute(vbo_, 0, 4, GL_FLOAT, 4 * sizeof(float), nullptr);
    vao_.unbind();
}

void text::draw(const camera& camera)
{
	auto x_tmp = static_cast<float>(x);
    shader_.activate();
    glUniform3f(glGetUniformLocation(shader_.id, "textColor"), color.x, color.y, color.z);
    projection_ = glm::ortho(0.0f, static_cast<float>(camera.width), static_cast<float>(camera.height), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection_));

    vao_.bind();

    for(const char& chr: displayed_text)
    {
        character ch = characters_.find(chr)->second;

        const float x_pos = x_tmp + ch.bearing.x * scale;
        const float y_pos = static_cast<float>(y) + (32.0f + ch.bearing.y - ch.size.y) * scale;

        const float w = ch.size.x * scale;
        const float h = ch.size.y * scale;

        const float vertices[6][4] = {
		    { x_pos,     y_pos - h,     0.0f, 0.0f },
		    { x_pos,     y_pos,         0.0f, 1.0f },
		    { x_pos + w, y_pos,         1.0f, 1.0f },

		    { x_pos,     y_pos - h,     0.0f, 0.0f },
		    { x_pos + w, y_pos,         1.0f, 1.0f },
		    { x_pos + w, y_pos - h,     1.0f, 0.0f }
        };
        ch.texture.bind();
        vbo_.bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        vbo_.unbind();

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x_tmp += (ch.advance >> 6) * scale;
    }

    vao_.unbind();
}

text::~text() = default;

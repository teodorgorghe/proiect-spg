#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>

static std::string text_file_read(const char* fn)
{
	std::ifstream file(fn);
	std::string file_text;
	while (file.good()) {
		std::string line;
		std::getline(file, line);
		file_text.append(line + "\n");
	}
	return file_text;
}

class shader
{
public:
	GLuint id = 0;
	
	shader(const char* vertex_file, const char* fragment_file)
	{
		const std::string vertex_code = text_file_read(vertex_file);
		const std::string fragment_code = text_file_read(fragment_file);

		const char* vertex = vertex_code.c_str();
		const char* fragment = fragment_code.c_str();

		const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex, nullptr);
		glCompileShader(vertex_shader);
		compile_errors(vertex_shader, "VERTEX");

		const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment, nullptr);
		glCompileShader(fragment_shader);
		compile_errors(fragment_shader, "FRAGMENT");

		id = glCreateProgram();
		glAttachShader(id, vertex_shader);
		glAttachShader(id, fragment_shader);
		glLinkProgram(id);
		compile_errors(id, "PROGRAM");

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void activate() const
	{
		glUseProgram(id);
	}

	void dispose() const
	{
		glDeleteProgram(id);
	}

private:
	static void compile_errors(const unsigned int shader, const char* type)
	{
		GLint has_compiled;

		char info_log[1024];
		if (strcmp(type, "PROGRAM") != 0)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);
			if (has_compiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, info_log);
				std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << info_log << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &has_compiled);
			if (has_compiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, info_log);
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << info_log << std::endl;
			}
		}
	}
};
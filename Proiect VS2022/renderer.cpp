#include "Renderer.h"

#include <cstdio>
#include <GL/glew.h>
#include <GL/freeglut_std.h>

renderer::renderer(const int width, const int height): camera(width, height, glm::vec3(0, 0, 20))
{
}

void renderer::init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glClearColor(1, 1, 1, 0);
	glewInit();

	camera.update_matrix_ortho();
}

void renderer::reshape(const int width, const int height)
{
	camera.height = height;
	camera.width = width;
	camera.update_matrix_ortho();
}

void renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& obj : coons_vector)
	{
		obj.first.draw(camera);
	}

	for (auto& obj : bezier_vector)
	{
		obj.first.draw(camera);
	}

	for (auto& obj : coons_vector)
	{
		for(auto& circle: obj.second.first)
		{
			circle.draw(camera);
		}

		for (auto& lines : obj.second.second)
		{
			lines.draw(camera);
		}
	}

	for (auto& obj : bezier_vector)
	{
		for (auto& circle : obj.second.first)
		{
			circle.draw(camera);
		}

		for (auto& lines : obj.second.second)
		{
			lines.draw(camera);
		}
	}

	for (text& obj : text_vector)
	{
		obj.draw(camera);
	}

	glutSwapBuffers();
}
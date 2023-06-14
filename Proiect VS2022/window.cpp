#include "Window.h"

#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include "coons.h"
#include "lambda.h"

#define PI glm::pi<float>()

/**
 * \brief Renderer init and rendering the text into the screen
 */
void window::init()
{
	renderer_.init();
	renderer_.text_vector.emplace_back("fonts/arial.ttf", "Coons", 10, 10, 1.0f);
}

/**
 * \brief Window initialization and glut subsystem
 * \param width Window width size
 * \param height Window height size
 * \param x_pos Window x position
 * \param y_pos Window y position
 * \param title Window title
 * \param argc Glut subsystem argument counter
 * \param argv Glut subsystem argument list
 */
window::window(const int width, const int height, const int x_pos, const int y_pos, const char* title, int argc, char** argv): renderer_(width, height)
{
	glutInit(&argc, argv);

	width_ = width;
	height_ = height;
	x_pos_ = x_pos;
	y_pos_ = y_pos;

	if (title == nullptr)
	{
		printf("Null title has found! Using default title!");
	}
	else
	{
		const size_t length = strlen(title) + 1;
		title_ = new char[length];
		memcpy(title_, title, length);
	}
}

window::~window()
{
	delete[] title_;
}

/**
 * \brief Window init with the given position and title, callback init and main loop.
 */
void window::run()
{
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(x_pos_, y_pos_);
	glutInitWindowSize(width_, height_);
	glutCreateWindow(title_);

	init();

	glutDisplayFunc(partial([=] { renderer_.draw(); }));
	glutReshapeFunc(to_func([=](const int x, const int y) { on_reshape(x, y); }));
	glutKeyboardFunc(to_func2([=](unsigned char chr, int i1, int i2) { on_key_event(chr, i1, i2); }));
	glutMouseFunc(to_func3([=](int i1, int i2, int i3, int i4) { on_mouse_event(i1, i2, i3, i4); }));
	glutMotionFunc(to_func([=](int i1, int i2) { on_mouse_move(i1, i2); }));
	glutMainLoop();
}

/**
 * \brief Keyboard callback
 * \param chr Key pressed in ASCII encoding
 * \param i1 x position with values between 0 and width
 * \param i2 y position with values between 0 and height
 */
void window::on_key_event(unsigned char chr, int i1, int i2)
{
	if(chr == ' ')
	{
		if(state_curbe != 0)
		{
			switch (curbe_type)
			{
			case 0:
				renderer_.coons_vector.pop_back();
				break;
			case 1:
				renderer_.bezier_vector.pop_back();
				break;
			default:
				break;
			}
			state_curbe = 0;
		}
		if(curbe_type == 0)
		{
			curbe_type = 1;
			renderer_.text_vector[0].displayed_text = "Bezier";
		}
		else
		{
			curbe_type = 0;
			renderer_.text_vector[0].displayed_text = "Coons";
		}
		glutPostRedisplay();
	}
}

/**
 * \brief Mouse click callback
 * \param button GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
 * \param state GLUT_DOWN or GLUT_UP
 * \param x position on x-axis, [0-width]
 * \param y position on y-axis, [0-height]
 */
void window::on_mouse_event(int button, int state, int x, int y)
{
	const glm::vec3 coords = renderer_.camera.transform_2d_to_3d(glm::vec2(x, y));
	if(button == GLUT_LEFT_BUTTON)
	{
		mouse_button_ = button;

		// Verificare dacă punctul introdus este pe o altă curbe
		switch(curbe_type)
		{
		case 0:
			// modifică curba existentă
			if (state == GLUT_DOWN)
			{
				const std::pair<std::pair<coons, guidelines>*, int> selection = select_coons(coords.x, coords.y);
				if (selection.first == nullptr)
				{
					// adaugă o curbă nouă
					put_coons(coords.x, coords.y);
				}
				else
				{
					selected_coons = selection;
				}
			}
			else
			{
				if(selected_coons.first != nullptr)
				{
					move_coons(coords);
				}
			}
			break;
		case 1:
			if (state == GLUT_DOWN)
			{
				const std::pair<std::pair<bezier, guidelines>*, int> selection = select_bezier(coords.x, coords.y);
				if (selection.first == nullptr)
				{
					// adaugă o curbă nouă
					put_bezier(coords.x, coords.y);
				}
				else
				{
					selected_bezier = selection;
				}
			}
			else
			{
				if (selected_bezier.first != nullptr)
				{
					move_bezier(coords);
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		mouse_button_ = -1;
	}
	glutPostRedisplay();
}

/**
 * \brief Add a new coons curve into the screen
 * \param x position on x-axis, [-1,1]
 * \param y position on y-axis, [-1,1]
 */
void window::put_coons(float x, float y)
{
	renderer_.coons_vector.push_back(std::pair<coons, guidelines>(
		coons(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 20),
		{ std::vector<circle> {
			circle(x, y, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f))
		}, std::vector<dashed_line>{
			dashed_line(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 50.0f, 50.0f),
			dashed_line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 50.0f, 50.0f)
		} })
	);
	selected_coons = { &renderer_.coons_vector.back(), 0};
}

/**
 * \brief Add a new bezier curve into the screen
 * \param x position on x-axis, [-1,1]
 * \param y position on y-axis, [-1,1]
 */
void window::put_bezier(float x, float y)
{
	renderer_.bezier_vector.push_back(std::pair<bezier, guidelines>(
		bezier(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 20),
		{ std::vector<circle> {
			circle(x, y, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f)),
			circle(0.0f, 0.0f, 0.03f, 0.01f, glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f))
		}, std::vector<dashed_line>{
			dashed_line(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 50.0f, 50.0f),
			dashed_line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 50.0f, 50.0f)
		} })
	);
	selected_bezier = { &renderer_.bezier_vector.back(), 0};
}

/**
 * \brief Mouse move callback
 * \param x position on x-axis, [0-width]
 * \param y position on y-axis, [0-height]
 */
void window::on_mouse_move(int x, int y)
{
	if(mouse_button_ == GLUT_LEFT_BUTTON)
	{
		const glm::vec3 coords = renderer_.camera.transform_2d_to_3d(glm::vec2(x, y));
		switch(curbe_type)
		{
		case 0:
			if(selected_coons.first != nullptr)
			{
				move_coons(coords);
			}
			break;
		case 1:
			if (selected_bezier.first != nullptr)
			{
				move_bezier(coords);
			}
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}
}

void window::on_reshape(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	renderer_.reshape(w, h);
}

/**
 * \brief Gets the clicked circle
 * \param x coordinate on x-asis
 * \param y coordinate on y-axis
 * \return A pair of the curve and index selected
 */
std::pair<std::pair<coons, guidelines>*, int> window::select_coons(const float x, const float y)
{
	std::pair<coons, guidelines>* selected = nullptr;
	int index = -1;
	float distance = FLT_MAX;
	for(auto& curbe: renderer_.coons_vector)
	{
		glm::vec3 points[] = { curbe.first.get_p1(), curbe.first.get_p2(), curbe.first.get_t1(), curbe.first.get_t2() };
		for(int i = 0; i < sizeof(points)/sizeof(glm::vec3); i++)
		{
			const float dx = x - points[i].x;
			const float dy = y - points[i].y;
			const float d = sqrt(dx * dx + dy * dy);
			if(d <= 0.03f && d < distance)
			{
				distance = d;
				index = i;
				selected = &curbe;
			}
		}
	}
	return { selected, index };
}

/**
 * \brief Gets the clicked circle
 * \param x coordinate on x-asis
 * \param y coordinate on y-axis
 * \return A pair of the curve and index selected
 */
std::pair<std::pair<bezier, guidelines>*, int> window::select_bezier(const float x, const float y)
{
	std::pair<bezier, guidelines>* selected = nullptr;
	int index = -1;
	float distance = FLT_MAX;
	for (auto& curbe : renderer_.bezier_vector)
	{
		glm::vec3 points[] = { curbe.first.get_p1(), curbe.first.get_p2(), curbe.first.get_t1(), curbe.first.get_t2() };
		for (int i = 0; i < sizeof(points) / sizeof(glm::vec3); i++)
		{
			const float dx = x - points[i].x;
			const float dy = y - points[i].y;
			const float d = sqrt(dx * dx + dy * dy);
			if (d <= 0.03f && d < distance)
			{
				distance = d;
				index = i;
				selected = &curbe;
			}
		}
	}
	return { selected, index };
}

void window::move_coons(glm::vec3 coords) const
{
	selected_coons.first->second.first[selected_coons.second].set_center(coords.x, coords.y);
	switch (selected_coons.second)
	{
	case 0:
		selected_coons.first->first.set_p1(coords);
		selected_coons.first->second.second[0].change_line(selected_coons.first->first.get_p1(), selected_coons.first->first.get_t1());
		break;
	case 1:
		selected_coons.first->first.set_p2(coords);
		selected_coons.first->second.second[1].change_line(selected_coons.first->first.get_p2(), selected_coons.first->first.get_t2());
		break;
	case 2:
		selected_coons.first->first.set_t1(coords);
		selected_coons.first->second.second[0].change_line(selected_coons.first->first.get_p1(), selected_coons.first->first.get_t1());
		break;
	case 3:
		selected_coons.first->first.set_t2(coords);
		selected_coons.first->second.second[1].change_line(selected_coons.first->first.get_p2(), selected_coons.first->first.get_t2());
		break;
	default:
		break;
	}
}

void window::move_bezier(glm::vec3 coords)
{
	selected_bezier.first->second.first[selected_bezier.second].set_center(coords.x, coords.y);
	switch (selected_bezier.second)
	{
	case 0:
		selected_bezier.first->first.set_p1(coords);
		selected_bezier.first->second.second[0].change_line(selected_bezier.first->first.get_p1(), selected_bezier.first->first.get_t1());
		break;
	case 1:
		selected_bezier.first->first.set_p2(coords);
		selected_bezier.first->second.second[1].change_line(selected_bezier.first->first.get_p2(), selected_bezier.first->first.get_t2());
		break;
	case 2:
		selected_bezier.first->first.set_t1(coords);
		selected_bezier.first->second.second[0].change_line(selected_bezier.first->first.get_p1(), selected_bezier.first->first.get_t1());
		break;
	case 3:
		selected_bezier.first->first.set_t2(coords);
		selected_bezier.first->second.second[1].change_line(selected_bezier.first->first.get_p2(), selected_bezier.first->first.get_t2());
		break;
	default:
		break;
	}
}
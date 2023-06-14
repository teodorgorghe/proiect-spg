#pragma once

#include "Renderer.h"
using guidelines = std::pair<std::vector<circle>, std::vector<dashed_line>>;

class window
{
	int width_;
	int height_;
	int x_pos_;
	int y_pos_;
	char* title_ = const_cast<char*>("SPG Proiect");

	glm::mat4 projection_matrix_;
	glm::mat4 view_matrix_;

	renderer renderer_;

	int state_curbe = 0;
	int curbe_type = 0;
	int mouse_button_ = -1;

	void on_mouse_event(int button, int state, int x, int y);
	void put_coons(float x, float y);
	void put_bezier(float x, float y);
	void on_mouse_move(int x, int y);
	void on_reshape(int w, int h);
	void move_bezier(glm::vec3 coords);
	void move_coons(glm::vec3 coords) const;

	std::pair<std::pair<coons, guidelines>*, int> select_coons(float, float);
	std::pair<std::pair<bezier, guidelines>*, int> select_bezier(float x, float y);

	std::pair<std::pair<coons, guidelines>*, int> selected_coons { nullptr, -1 };
	std::pair<std::pair<bezier, guidelines>*, int> selected_bezier { nullptr, -1 };

	void init();
public:
	window(int width, int height, int x_pos, int y_pos, const char* title, int argc = 0, char** argv = nullptr);
	~window();

	void run();
	void on_key_event(unsigned char chr, int i1, int i2);
};

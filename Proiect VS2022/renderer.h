#pragma once

#include <vector>

#include "bezier.h"
#include "Camera.h"
#include "coons.h"
#include "text.h"

class renderer
{
public:
	using guidelines = std::pair<std::vector<circle>, std::vector<dashed_line>>;
	camera camera;
	std::vector<std::pair<coons, guidelines>> coons_vector;
	std::vector<std::pair<bezier, guidelines>> bezier_vector;
	std::vector<text> text_vector;

	renderer(int width, int height);
	void init();
	void reshape(int width, int height);
	void draw();
};

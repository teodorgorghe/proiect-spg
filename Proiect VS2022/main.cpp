#include "Window.h"

int main(int argc, char** argv)
{
	window wdn(800, 600, 0, 0, "SPG", argc, argv);
	wdn.run();
	return 0;
}
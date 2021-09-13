#pragma once

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>		// does not contain #pragma once
#endif

extern double scroll_callback_x_offset = 0;
extern double scroll_callback_y_offset = 0;

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	scroll_callback_x_offset = x_offset;
	scroll_callback_y_offset = y_offset;
}
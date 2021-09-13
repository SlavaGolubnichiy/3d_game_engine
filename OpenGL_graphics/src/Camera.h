#pragma once

#include "util/aux_.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include "vendor/glm/gtx/rotate_vector.hpp"
#include "vendor/glm/gtx/vector_angle.hpp"

#include <iomanip>

#include "MouseScrollCallbackInput.h"

#define MOUSE_CONTROL_DEBUG 0


class Camera
{
	public:		// public is required, because ImGui needs values' references (correct it?)

	float window_width;
	float window_height;
	float near_render_limit = 0.001f;
	float far_render_limit = 100.0f;
	float fov_degrees = 60.0f;

	glm::vec3 position		= glm::vec3(0.0f, 0.0f,  0.0f);
	glm::vec3 orientation	= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up			= glm::vec3(0.0f, 1.0f,  0.0f);
	float speed = 0.02f;
	float sensitivity = 40.0f;

	glm::mat4 view_mat = glm::mat4(1.0f);	// change the way, Camera::view_mat passed into object1->SetUniformMatrix4f()
											// (make matrix non-private)
	glm::mat4 proj_mat = glm::mat4(1.0f);	// change the way, Camera::proj_mat passed into object1->SetUniformMatrix4f() 
											// (make matrix non-private)
	bool do_mouse_control_camera = false;

	Camera
	(
		unsigned int window_width,
		unsigned int window_height,
		glm::vec3 position		= glm::vec3(0.0f, 0.0f,  0.0f),
		glm::vec3 orientation	= glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up			= glm::vec3(0.0f, 1.0f,  0.0f)
	)
		:
		window_width(window_width),
		window_height(window_height),
		position(position),
		orientation(orientation),
		up(up)
	{
		// this before getting constructor params values is not neccesary - 
		// they're equal anyway
		this->view_mat = glm::lookAt(position, position + orientation, up);
		this->proj_mat = glm::perspective
		(
			glm::radians(fov_degrees),
			(float)(window_width / window_height),
			near_render_limit,
			far_render_limit
		);
	}

	void SetNearRenderLimit(float value)
	{
		if (0.00099f < value)
		{
			this->near_render_limit = value;
		}
		else
		{
			throw std::exception
			(
				std::string("Camera [" + std::to_string((unsigned int)this) + "].near_render_limit value has to be in range (0.00099f; float_max_value].").c_str()
			);
		}
	}
	void SetFarRenderLimit(float value)
	{
		if (0.00099f < value)
		{
			this->far_render_limit = value;
		}
		else
		{
			throw std::exception
			(
				std::string("Camera [" + std::to_string((unsigned int)this) + "].far_render_limit value has to be in range (0.00099f; float_max_value].").c_str()
			);
		}
	}
	void SetFovDegrees(float value)
	{
		if (0.00000099f < value && value < 360.0f)
		{
			this->fov_degrees = value;
		}
		else
		{
			throw std::exception
			(
				std::string("Camera [" + std::to_string((unsigned int)this) + "].fov_degrees value has to be in range (0.00000099f; 360.0f).").c_str()
			);
		}
	}
	void SetSpeed(float value)
	{
		if (0.00099f < value)
		{
			this->speed = value;
		}
		else
		{
			throw std::exception
			(
				std::string("Camera [" + std::to_string((unsigned int)this) + "].fov_degrees value has to be in range (0.00099f; float_max_value).").c_str()
			);
		}
	}

	void Update()	// updates position (x,y,z) and configuration (fov, limits etc.)
	{
		this->view_mat = glm::mat4(1.0f);
		this->proj_mat = glm::mat4(1.0f);
		this->view_mat = glm::lookAt(this->position, this->position + this->orientation, this->up);
		this->proj_mat = glm::perspective
		(
			glm::radians(this->fov_degrees),
			(float)(this->window_width / this->window_height),
			this->near_render_limit,
			this->far_render_limit
		);
	}

	void ProcessKeyboardInputs(GLFWwindow* window)
	{
		// movement is relative to current camera orientation
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += speed * orientation;					// move forwards
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += speed * (-glm::normalize(glm::cross(orientation, up)));		// move left
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position += speed * (-orientation);					// move backwards
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += speed * glm::normalize(glm::cross(orientation, up));	// move right
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			//position += speed * up;							// move upwards
			position += speed * glm::normalize(glm::cross(/*right vector -> */glm::normalize(glm::cross(orientation, up)), orientation));
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			//position += speed * (-up);						// move downwards
			position += speed * -(glm::normalize(glm::cross(/*right vector -> */glm::normalize(glm::cross(orientation, up)), orientation)));
		}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			speed *= 1.05f;		// +5 %
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		{
			speed /= 1.05f;		// -5 %
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			this->do_mouse_control_camera = !this->do_mouse_control_camera;
			for(unsigned int i = 0; i < 0x01FFFFFF; i++);	// delay for C key (better way to do that ?)
		}
	}

	void ProcessMouseInputs(GLFWwindow* window, unsigned int window_height, unsigned int window_width)
	{
		if (do_mouse_control_camera)
		{
			/// mouse_cursor-camera_orientation control

			// movement forwards/backwards/left/right/up/down 
			// is relative to current camera orientation
			
			// Hide cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			
			// calculate camera rotation depending on cursor movements
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			// cursor movement calculation and normalization from (0; w) and (0; h) to (-1; 1) and (-1; 1)
			float vert_cam_rotation = sensitivity * (float)(mouseY - ((float)window_height / 2)) / window_height;
			float horiz_cam_rotation = sensitivity * (float)(mouseX - ((float)window_width / 2)) / window_width;
			// camera rotationX
			this->orientation = glm::rotate(this->orientation, glm::radians(-horiz_cam_rotation), up);
			// camera rotationY check
			glm::vec3 new_orientation =
				glm::rotate(this->orientation, glm::radians(-vert_cam_rotation), glm::normalize(glm::cross(orientation, up)));
			// check if camera angle is in accepted degrees range
			float angle_between_new_orient_and_up = glm::angle(new_orientation, up);
			bool does_cam_look_in_accepted_angle = 
				glm::radians(2.0f) <= angle_between_new_orient_and_up &&
				angle_between_new_orient_and_up <= glm::radians(178.0f);
			if (does_cam_look_in_accepted_angle)
			{
				// camera rotation Y
				this->orientation = new_orientation;
			}
			#if MOUSE_CONTROL_DEBUG
			//std::cout << std::setprecision(3) <<
			//	"angle(new_orient, up) = [" << glm::radians(1.0f) << "; " << glm::angle(new_orientation, up) << "; " <<
			/	glm::radians(179.0f) << "]." << std::endl;
			#endif

			// reset cursor pos
			glfwSetCursorPos(window, (window_width / 2), (window_height / 2));

			/// mouse_wheel-fov control
			float next_fov = this->fov_degrees + 10.0f * (-scroll_callback_y_offset);
			if (10.0f <= next_fov && next_fov <= 120.0f)
			{
				this->fov_degrees = next_fov;
				this->sensitivity;
			}
			scroll_callback_y_offset = 0;

			#if MOUSE_CONTROL_DEBUG
			//std::cout << "fov_delta = " << (double)(0.5f * -scroll_callback_y_offset) << std::endl;
			//std::cout << "sensitivity = " << this->sensitivity << std::endl;
			#endif
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
};



#pragma once

#include <string>

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>
#endif


// Singleton pattern used
class GLFWManagerSingleton
{
	private:
	int opengl_context_version = 0;
	int opengl_context_profile = 0;
	bool is_vsync_enabled = true;
	GLFWwindow* main_window;
	//std::vector<GLFWwindow*> other_windows_array;
	
	// Constructor is private
	GLFWManagerSingleton
	(
		int opengl_context_version,
		int opengl_context_profile,
		bool is_vsync_enabled = true
	)
		:
		opengl_context_version(opengl_context_version),
		opengl_context_profile(opengl_context_profile),
		is_vsync_enabled(is_vsync_enabled)
	{
		// Initizlize GLFW
		glfwInit();
		// Tell GLFW what OpenGL version we are using (OpenGL 3.3 in this case)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_context_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_context_version);
		// tell GLFW to use CORE_PROFILE
		glfwWindowHint(GLFW_OPENGL_PROFILE, opengl_context_profile);
		if (is_vsync_enabled)
		{
			this->EnableVsync();
		}
		
	}

	// Singletons have to be non-clonable.
	GLFWManagerSingleton(const GLFWManagerSingleton& other) = delete;
	// Singleton have to be non-assignable.
	void operator=(const GLFWManagerSingleton& other) = delete;



	public:

	static GLFWManagerSingleton* GetInstance
	(
		int opengl_context_version,
		int opengl_context_profile,
		bool is_vsync_enabled = true
	)
	{
		static GLFWManagerSingleton* single_glfw_manager = new GLFWManagerSingleton
		(
			opengl_context_version,
			opengl_context_profile,
			is_vsync_enabled
		);
		return single_glfw_manager;
	}

	~GLFWManagerSingleton()
	{
		// destroy main window
		if (this->main_window != NULL)
		{
			glfwDestroyWindow(this->main_window);
			this->main_window = nullptr;
		}
		// destroy other windows
		/*
		unsigned int windows_number = this->windows_array.size();
		if (windows_number != 0)
		{
			for (unsigned int i = 0; i < windows_number; i++)
			{
				if (this->windows_array[i] != NULL)
				{
					glfwDestroyWindow(this->windows_array[i]);
				}
			}
		}
		*/
		glfwTerminate();
	}

	bool tryIsWidthOrHeightValid(int width_or_height_value)
	{
		if (width_or_height_value > 0)
		{
			return true;
		}
		else
		{
			throw std::exception(std::string("Neither width, not height value should not be less than +1.").c_str());
		}
	}

	void CreateMainWindow
	(
		int width,
		int height,
		const char* name
	)
	{
		if (tryIsWidthOrHeightValid(width) && tryIsWidthOrHeightValid(height))
		{
			// create window with specified parameters
			this->main_window = glfwCreateWindow
			(
				width,		// window width
				height,		// window height
				name,		// window name
				NULL,		// ???
				NULL
			);
		}
	}

	bool IsMainWindowValid()
	{
		// check if window created incorrectly
		return (this->main_window != NULL);
	}

	void MakeMainWindowCurrent()
	{
		// introduce the window into the current context
		glfwMakeContextCurrent(this->main_window);
		// enable or disable v-sync
		if (is_vsync_enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

	}

	void UpdateMainWindow()
	{
		// Swap write-buffer (back-buffer) with the show-buffer (front-buffer) 
		// to see the clear-color at the window
		glfwSwapBuffers(this->main_window);
	}

	bool ShouldMainWindowClose()
	{
		return glfwWindowShouldClose(this->main_window);
	}

	void PollMainWindowEvents()
	{
		// process GLFW window events (resizing, closing etc.)
		glfwPollEvents();
	}

	bool GetIsVsyncEnabled()
	{
		return this->is_vsync_enabled;
	}

	void EnableVsync()
	{
		this->is_vsync_enabled = true;
		glfwSwapInterval(1);
	}

	void DisableVsync()
	{
		this->is_vsync_enabled = false;
		glfwSwapInterval(0);
	}

	GLFWwindow* GetMainWindow()
	{
		return this->main_window;
	}


	/*
	void CreateNewWindow
	(
		unsigned int width,
		unsigned int height,
		const char* name
	)
	{
		// create window with specified parameters
		this->main_window = glfwCreateWindow
		(
			width,		// window width
			height,		// window height
			name,		// window name
			NULL,		// ???
			NULL
		);
	}
	*/

};
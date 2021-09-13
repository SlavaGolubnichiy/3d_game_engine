/// OpenGL learning project (learning started from 06.07.2021)
/// v1.005 from 29.08.2021

// 12.07.2021 -	started v1.005, ambient light implemented
// 14.07.2021 -	diffuse light implemented
// 25.07.2021 -	bugs fixes
// 05.07.2021 -	specular light implemented
// 10.07.2021 -	specular light improoved
// 21.07.2021 -	different types of light (directionalLight, pointLight and spotLight) implemented in an *_illuminated.frag
// 28.08.2021 -	bugs fixes
// 29.08.2021 - trying to implement Mesh class
// 30.08.2021 - ~
// 31.08.2021 - mesh class successfully implemented & used
// 01.08.2021 - 

/// TODO:

/// Check Texture2D.h::GenerateTexture()::glTexImage2D(internal_format) parameter on docs.gl

// 1.	Camera::ProcessMouseInputs():		+- OK (make better)
		// 2.1 Correct the camera behaviour when looking alongside Y axis (up and down).
		// 2.2 Make camera movement smooth
		// 2.3 Make camera fov changing smoothly
// 2. Prevent multiple switching to/from mouse-camera from/to mouse-input mode.	+- OK (make better)
//		Implement controls calculation in a separate thread (?)
// 4. implement OpenGLTextureContainer::
//		PROBLEM:
//			when several textures are created, slots for them have to be specified by user-programmer.
//			This is, at least, uncomfortable, and sometimes hard to do (for example when textures 
//			have to be created automatically or by program-user).
//		SOLUTION:
//			Implement OpenGLTextureContainer class, which contains 32 texture in 32 slots.
//			Access has to be allowed by a slot number. Usage of 32 slots only is allowed (opengl machine restrictions).
//			Create a TextureContainer, using singleton pattern. It will manage texture slots correctly.
// 6. Camera::
//		Change the way, Camera::view_mat and Camera::proj_mat are passed into object1->SetUniformMatrix4f()
//		(make matrices non-public)
// 7. ShaderProgram.cpp::
//		Rewrite SAFE_SET_METHOD as static class method. Use function pointer to pass
//		function into the new SAFE_SET_METHOD definition body.
// 8. ShaderProgram class::
//		Implement fetching all the uniforms names from the shader source.
//		Optional: Then all uniforms values must be passed to glUseProgram method.
// 9. OpenGLobj::SetScale()
//		Correct rotation transformation of scaled model. 
//		(when using both SetRotation() and SetScale() in main app.)

/*
//		---- shader.vert ----
//		uniform float uni1;
//		uniform vec2 uni2;
//		---------------------
//
//		---- Shader.h ----
//		class ShaderProgram
//		{
//			private:
//			...
//			std::unordered_map<std::string, GLint> uniforms_name_id_map;
//
//			public:
//			ShaderProgram
//			(
//				unsigned int vertex_shader_id,
//				unsigned int fragment_shader_id,
//				GLint& is_success,
//				float uni1,
//				glm::vec2 uni2
//			)
//			{
//				// same as previous constructor
//
//				for (unsigned int i = 0; i < uniforms_name_id_map.size; i++)
//				{
//
//				}
//
//			}
//		}
*/

// 10. Whole project::VertexAttribute class
//		Define VertexAttributes-related class or something.
//		Check the structure of OpenGLobj and make it clearer, especially in
//		the scope of VertexAttributes specifying.
// 11. GLFWmanager::
//		Implement creating more than one main window
//		(means implement working with		std::vector<GLFWwindow*> other_windows_array)
// 12. FileReader::
//		Make fileReader class singleton or a namespace (which way is better ?)
// 13. VertexBuffer:: 
//		UpdateVerticesData(...):: add data resize opengl updating
// 14. Application.cpp:: 
//		Separate developed classes code int *.h and *.cpp files. Debug the app after doing it.



#include "App.h"

// to log errors use SPDLog (search on GitHub)


namespace camera
{
	glm::vec3 position		= glm::vec3(-3.4f, 2.3f,-0.7f);
	glm::vec3 orientation	= glm::vec3( 0.7f,-0.2f, 0.8f);
	glm::vec3 up			= glm::vec3( 0.0f, 1.0f, 0.0f);
}

namespace world
{
	glm::vec3 ambient_light_color = glm::vec3(0.3f, 0.3f, 0.3f);
}

int main()
{
	double loading_start = glfwGetTime();
	#pragma region GLFW_GLEW_init

	GLFWManagerSingleton* glfw_manager =
		GLFWManagerSingleton::GetInstance
		(
			3,
			GLFW_OPENGL_CORE_PROFILE,
			true
		);
	glfw_manager->CreateMainWindow(ThisApp::main_window_width, ThisApp::main_window_height, ThisApp::main_window_name.c_str());
	// check if window created correctly
	if (!glfw_manager->IsMainWindowValid())
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfw_manager->~GLFWManagerSingleton();
		return -1;
	}
	glfw_manager->MakeMainWindowCurrent();

	// GLEW init
	glewInit();
	if (glewInit())
	{
		std::cout << "GLEW initialization failed." << std::endl;
		glfwTerminate();
		return -1;
	}
	ThisApp::PrintOpenGLInfo();

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	#pragma endregion

	#pragma region Global_rendering_settings
	
	// Specify the OpenGL viewport
	// In this case, viewport goes from x = 0, y = 0 to x = WINDOW_WIDTH, y = WINDOW_HEIGHT
	glViewport(0, 0, ThisApp::main_window_width, ThisApp::main_window_height);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	#pragma endregion

	#pragma region imgui_init
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(glfw_manager->GetMainWindow(), true);
	const char* glsl_version = "#version 330";	// check if it's true (130 was by default)
	ImGui_ImplOpenGL3_Init(glsl_version);
	#pragma endregion

	glfwSetScrollCallback(glfw_manager->GetMainWindow(), scroll_callback);
	Camera main_cam
	(
		ThisApp::main_window_width, 
		ThisApp::main_window_height,
		camera::position,
		camera::orientation,
		camera::up
	);
	OpenglObj3DDataGenerator object_generator;

	std::string shaders_dir = "res/shaders/";
	std::string basic2d_vert_filename	= "basic2d.vert";
	std::string basic2d_frag_filename	= "basic2d.frag";
	std::string basic3d_vert_filename	= "basic3d.vert";
	std::string basic3d_frag_filename	= "basic3d.frag";
	std::string basic3d_illuminated_vert_filename = "basic3d_illuminated.vert";
	std::string basic3d_illuminated_frag_filename = "basic3d_illuminated.frag";
	std::string light_vert_filename		= "light.vert";
	std::string light_frag_filename		= "light.frag";
	std::string tex_vert_filename		= "tex.vert";
	std::string tex_frag_filename		= "tex.frag";
	std::string tex_illuminated_vert_filename = "tex_illuminated_vert.glsl";
	std::string tex_illuminated_frag_filename = "tex_illuminated_frag.glsl";

	/// square (textured)
	#pragma region square_textured
	aux::color::RGBAColor square_tex_backgr_color = { 0.5f, 0.5f, 0.5f, 1.0f };

	Texture2DData* square_tex = new Texture2DData
	(
		"res/textures/my/test_512x512.png", GL_RGBA, GL_TEXTURE0,
		GL_NEAREST, GL_NEAREST,
		GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER,
		square_tex_backgr_color
	);
	glm::vec2 square_sizes(1.0f, 1.0f);
	glm::vec3 square_pos(2.0f, 0.0f, 0.0f);
	OpenglObjTextured* square = new OpenglObjTextured
	(
		&object_generator.GetRect3d(square_sizes),
		shaders_dir + tex_vert_filename,
		shaders_dir + tex_frag_filename,
		square_tex,
		"tex0",
		square_pos
	);
	#pragma endregion

	/// pyramid (textured)
	#pragma region pyramid_textured
	aux::color::RGBAColor pyramid_tex_backgr_color = { 0.5f, 0.5f, 0.5f, 1.0f };
	Texture2DData pyramid_tex_data
	(
		//"res/textures/demo/specular_light_demo_material_Paint_Rusted_1K_albedo.png",
		"res/textures/textures.com/YellowMedievalBricks_New_1K_albedo.png",
		GL_RGBA, 
		GL_TEXTURE1
	);
	Texture2DData pyramid_tex_specular_map_data
	(
		//"res/textures/demo/specular_light_demo_material_Paint_Rusted_1K_roughness_v2.png",
		"res/textures/textures.com/YellowMedievalBricks_New_1K_roughness.png",		// roughness map better fits the purpose of specular light map
		//"res/textures/textures.com/YellowMedievalBricks_New_1K_ambient_occlusion.png",
		GL_RGBA,
		GL_TEXTURE2
	);
	glm::vec3	pyramid_pos(-3.0f, 1.0f, 1.0f);
	float		pyramid_rotation_angle = 0.0f;
	glm::vec3	pyramid_rotation_normal(0.0f, 1.0f, 0.0f);
	glm::vec3	pyramid_scale(1.0f, 1.0f, 1.0f);
	float		pyramid_linear_scale = 1.0f;
	OpenglObj3DData pyramid_obj_data = object_generator.GetPyramid3dWithNormals(glm::vec2(1.0f, 1.0f), 1.0f, glm::vec3(0.4f, 0.64f, 0.1f));
	OpenglObjTexturedSpecmapped* pyramid = new OpenglObjTexturedSpecmapped
	(
		&pyramid_obj_data,
		shaders_dir + tex_illuminated_vert_filename,
		shaders_dir + tex_illuminated_frag_filename,
		&pyramid_tex_data,
		"tex0",
		&pyramid_tex_specular_map_data,
		"tex_specular_map",
		pyramid_pos
	);
	float pyramid_material_property1 = 32.0f;
	#pragma endregion

	// variables to rotate the pyramid
	float rotation_increment = 0.5;
	bool do_auto_rotate = false;
	double previous_time = glfwGetTime();
	double current_time = 0;
	
	/// Colored cube
	#pragma region cube_textured
	glm::vec3 cube_color = glm::vec3(0.8f, 0.3f, 0.2f);
	glm::vec3 cube_sizes = glm::vec3(1.0f, 1.0f, 1.0);
	glm::vec3 cube_pos = glm::vec3(1.0f, 1.0f, 1.0f);
	OpenglObjColored* cube = new OpenglObjColored
	(
		&object_generator.GetCuboid3dWithNormals(cube_sizes, cube_color),
		shaders_dir + basic3d_illuminated_vert_filename,
		shaders_dir + basic3d_illuminated_frag_filename,
		cube_pos
	);
	float cube_material_property1 = 32.0f;
	#pragma endregion

	/// Light
	#pragma region light
	glm::vec3	light_color = glm::vec3 (1.0f, 1.0f, 1.0f);	// glm::vec3(1.0f, 0.64f, 0.1f);
	glm::vec3	light_sizes = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3	light_pos = glm::vec3(-0.7f, 2.3f, -0.2f);
	float		light_rotation_angle = 0.0f;
	glm::vec3	light_scale(1.0f, 1.0f, 1.0f);
	float		light_linear_scale = 1.0f;
	OpenglObjColored* light = new OpenglObjColored
	(
		&object_generator.GetCuboid3d(light_sizes),
		shaders_dir + light_vert_filename,
		shaders_dir + light_frag_filename,
		light_pos
	);
	light->SetColor(light_color);
	#pragma endregion
	/**/
	






	// 1. Insert this code into ShaderProgram constructor, using Mesh class
	// 

	/// DRAW USING MESH - BEGIN
	#if 0
	std::string vertex_shader_file_path = shaders_dir + basic3d_illuminated_vert_filename;
	std::string fragment_shader_file_path = shaders_dir + basic3d_illuminated_frag_filename;

	#pragma region TEST DRAW USING MESH - BEGIN
	// retrieve shaders' source codes
	std::string mesh_vertex_shader_source = FileReader::Read(vertex_shader_file_path);
	std::string mesh_fragment_shader_source = FileReader::Read(fragment_shader_file_path);

	// load shaders into opengl machine
	Shader mesh_vertex_shader(mesh_vertex_shader_source.c_str(), GL_VERTEX_SHADER, vertex_shader_file_path);
	Shader mesh_fragment_shader(mesh_fragment_shader_source.c_str(), GL_FRAGMENT_SHADER, fragment_shader_file_path);
	ShaderProgram* mesh_shader_program = new ShaderProgram(mesh_vertex_shader.GetId(), mesh_fragment_shader.GetId());

	glm::vec3 mesh_sizes(1.0f ,1.0f, 1.0f);
	OpenglObj3DData data = object_generator.GetCuboid3dWithNormals(mesh_sizes, glm::vec3(0.2f, 0.5f, 1.0f));


	Mesh first_mesh(object_generator.GetCuboid3dWithNormals(mesh_sizes, glm::vec3(0.2f, 0.5f, 1.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
	//MeshColored first_mesh(&object_generator.GetPyramid3dWithNormals(glm::vec2(1.0f, 1.0f), 1.0f, glm::vec3(0.2f, 0.5f, 1.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
	#endif
	#pragma endregion
	/// DRAW USING MESH - END










	double loading_end = glfwGetTime();
	std::cout << "Loading time: " << loading_end - loading_start << " s.";

	// frame loop
	while (!glfw_manager->ShouldMainWindowClose())
	{
		#pragma region ok_code
		// Set the background color value
		glClearColor(world::ambient_light_color.r, world::ambient_light_color.g, world::ambient_light_color.b, 1.0f);
		// Clean the write-buffer (back-buffer) and fill it with the current CLEAR-COLOR value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render imgui at the top layer
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Camera info");
		ImGui::Text("position: %.3f; %.3f; %.3f", main_cam.position.x, main_cam.position.y, main_cam.position.z);
		ImGui::Text("orientation: %.3f; %.3f; %.3f", main_cam.orientation.x, main_cam.orientation.y, main_cam.orientation.z);
		ImGui::Text("speed (1/frame): %.9f", main_cam.speed);
		ImGui::Text("fov: %.1f deg", main_cam.fov_degrees);
		ImGui::Text("Info: %.3f ms/frame, %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Begin("Controls");
		ImGui::Text("C - mouse-camera/mouse-input mode switching");
		ImGui::Text("WSADRF - forwards/backwards/left/right/up/down");
		ImGui::Text("Num+/Num- - increase/decrease camera speed");
		ImGui::Text("Mouse wheel up/down - increase/decrease FOV");
		ImGui::End();

		ImGui::Begin("Objects control");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.85f);	// sets the slider width as 90% of window_width
		ImGui::SliderFloat3("light.pos.xyz", glm::value_ptr(light_pos), -10.0f, 10.0f);
		ImGui::SliderFloat("light rotation angle", &light_rotation_angle, 0.0f, 360.0f);
		ImGui::SliderFloat("light linear scale", &light_linear_scale, 1.0f, 5.0f);
		ImGui::Text("---");
		ImGui::SliderFloat3("pyramid.pos.xyz", glm::value_ptr(pyramid_pos), -10.0f, 10.0f);
		ImGui::SliderFloat("pyramid rotation angle", &pyramid_rotation_angle, 0.0f, 360.0f);
		ImGui::SliderFloat("pyramid linear scale", &pyramid_linear_scale, 1.0f, 5.0f);
		ImGui::Checkbox("auto rotate", &do_auto_rotate);
		//ImGui::Text("pyramid->GetRotatedAngle(): %.3f", pyramid->GetRotatedAngle());
		ImGui::End();


		// ----------------------------- opengl render begin

		// rotation calculation
		current_time = glfwGetTime();
		if (current_time - previous_time >= 1 / 60)
		{
			if (do_auto_rotate)
			{
				pyramid_rotation_angle += rotation_increment;
				if (pyramid_rotation_angle >= 360.0f)
				{
					pyramid_rotation_angle = 0.0f;
				}
			}
			previous_time = current_time;
		}
		square->UseShaderProgram();
		// set the values of matrices uniforms in the shader
		square->SetUniformVec3("ambient_light", glm::value_ptr(world::ambient_light_color));
		square->SetUniformMatrix4f("model_mat", glm::value_ptr(square->GetModelMatrix()));
		square->SetUniformMatrix4f("view_mat", glm::value_ptr(main_cam.view_mat));
		square->SetUniformMatrix4f("proj_mat", glm::value_ptr(main_cam.proj_mat));
		square->Draw();

		pyramid->UseShaderProgram();
		// light calc - begin
		// diffused light uniforms
		pyramid->SetUniformVec3("ambient_light_color", glm::value_ptr(world::ambient_light_color));
		pyramid->SetUniformVec3("light_color", glm::value_ptr(light->GetColor()));
		pyramid->SetUniformVec3("light_pos", glm::value_ptr(light->GetPosition()));
		// specular light uniforms
		pyramid->SetUniformVec3("view_pos", glm::value_ptr(main_cam.position));
		pyramid->SetUniform1f("specular_strength", 0.5);
		pyramid->SetUniform1f("material_property1", pyramid_material_property1);
		// light calc - end
		pyramid->SetPosition(pyramid_pos);
		pyramid->SetRotation(pyramid_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		pyramid->SetScale(pyramid_linear_scale);
		//pyramid->SetUniformVec3("ambient_light", glm::value_ptr(world::ambient_light_color));
		pyramid->SetUniformMatrix4f("model_mat", glm::value_ptr(pyramid->GetModelMatrix()));
		pyramid->SetUniformMatrix4f("view_mat", glm::value_ptr(main_cam.view_mat));
		pyramid->SetUniformMatrix4f("proj_mat", glm::value_ptr(main_cam.proj_mat));
		pyramid->Draw();

		cube->UseShaderProgram();
		// light calc - begin
		// diffused light uniforms
		cube->SetUniformVec3("ambient_light_color", glm::value_ptr(world::ambient_light_color));
		cube->SetUniformVec3("light_color", glm::value_ptr(light->GetColor()));
		cube->SetUniformVec3("light_pos", glm::value_ptr(light->GetPosition()));
		// specular light uniforms
		cube->SetUniformVec3("view_pos", glm::value_ptr(main_cam.position));
		cube->SetUniform1f("specular_strength", 0.5);
		cube->SetUniform1f("material_property1", cube_material_property1);
		// light calc - end
		cube->SetUniformMatrix4f("model_mat", glm::value_ptr(cube->GetModelMatrix()));
		cube->SetUniformMatrix4f("view_mat", glm::value_ptr(main_cam.view_mat));
		cube->SetUniformMatrix4f("proj_mat", glm::value_ptr(main_cam.proj_mat));
		cube->SetPosition(glm::vec3(2.0f));
		cube->Draw();

		light->UseShaderProgram();
		light->SetPosition(light_pos);
		light->SetRotation(light_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		light->SetScale(light_linear_scale);
		light->SetUniformVec3("light_color", glm::value_ptr(light->GetColor()));
		light->SetUniformMatrix4f("model_mat", glm::value_ptr(light->GetModelMatrix()));
		light->SetUniformMatrix4f("view_mat", glm::value_ptr(main_cam.view_mat));
		light->SetUniformMatrix4f("proj_mat", glm::value_ptr(main_cam.proj_mat));
		light->Draw();
		#pragma endregion

		/// drawing using mesh - begin
		#if 0
		mesh_shader_program->UseProgram();
		// light calc - begin
		// diffused light uniforms
		mesh_shader_program->SetUniformVec3("ambient_light_color", glm::value_ptr(world::ambient_light_color));
		mesh_shader_program->SetUniformVec3("light_color", glm::value_ptr(light->GetColor()));
		mesh_shader_program->SetUniformVec3("light_pos", glm::value_ptr(light->GetPosition()));
		// specular light uniforms
		mesh_shader_program->SetUniformVec3("view_pos", glm::value_ptr(main_cam.position));
		mesh_shader_program->SetUniform1f("specular_strength", 0.5);
		mesh_shader_program->SetUniform1f("material_property1", cube_material_property1);
		// light calc - end
		mesh_shader_program->SetUniformMatrix4f("model_mat", glm::value_ptr(first_mesh.GetModelMatrix()));
		mesh_shader_program->SetUniformMatrix4f("view_mat", glm::value_ptr(main_cam.view_mat));
		mesh_shader_program->SetUniformMatrix4f("proj_mat", glm::value_ptr(main_cam.proj_mat));
		first_mesh.Draw(mesh_shader_program, main_cam);
		#endif
		/// drawing using mesh - end











		main_cam.ProcessKeyboardInputs(glfw_manager->GetMainWindow());
		main_cam.ProcessMouseInputs(glfw_manager->GetMainWindow(), ThisApp::main_window_height, ThisApp::main_window_width);
		main_cam.Update();

		// ----------------------------- opengl render end

		ImGui::Render();
		// render opengl draw data at the bottom layer (under the imgui)
		//ImGui::ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfw_manager->UpdateMainWindow();
		// process GLFW window events (resizing, closing etc.)
		glfw_manager->PollMainWindowEvents();
	}

	// destroy (delete) window and stop (de-init) glfw
	glfw_manager->~GLFWManagerSingleton();
	//end program
	return 0;
}

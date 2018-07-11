#pragma once

#include "Shader.h"
#include "Mesh2.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "rhostring.hpp"
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"



class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();
	void loadSceneConfig();
	string loadFile(string path);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	
	void do_movement();
	void render();
	void run();
	void finish();
	//void SceneManager::mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//void SceneManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	// scene
	void setupScene();

private:
	// Window dimensions
	GLuint WIDTH, HEIGHT;


	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;
	Shader *sunShader;

	//scene attributes
	//GLuint VAO;

	//GLuint VAO;
	//GLuint VBO;
	//GLuint EBO;

	//glm::mat4 projection;
	//glm::mat4 view;



};



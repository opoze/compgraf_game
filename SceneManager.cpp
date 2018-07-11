#include "SceneManager.h"
#include "objeto.h"

//static controllers for mouse and keyboard
static bool keys[1024];

//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
bool firstMouse = true;
GLfloat lastX;
GLfloat lastY;
Camera *camera = new Camera(glm::vec3(0.0f, 2.0f, 4.0f));

objeto *mundo1;
objeto *mundo2;
objeto *sol;
glm::mat4 transform;

vector <objeto> objs;


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint width, GLuint height)
{
	this->WIDTH = width;
	this->HEIGHT = height;

	lastX = this->WIDTH / 2.0;
	lastY = this->HEIGHT / 2.0;
	

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	//setup the scene
	setupScene();

}

void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_R])
		transform = glm::rotate(transform, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (keys[GLFW_KEY_J])
		transform = glm::translate(transform, glm::vec3(-0.05f, 0.0f, 0.0f));
	if (keys[GLFW_KEY_L])
		transform = glm::translate(transform, glm::vec3(0.05f, 0.0f, 0.0f));
	if (keys[GLFW_KEY_K])
		transform = glm::translate(transform, glm::vec3(0.0f, -0.05f, 0.0f));
	if (keys[GLFW_KEY_I])
		transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, 0.0f));
}

void SceneManager::render()
{

	// Clear the colorbuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//mundo1->render(camera, WIDTH, HEIGHT);
	//mundo2->render(camera, WIDTH, HEIGHT);
	//sol->render(camera, WIDTH, HEIGHT);

	for (unsigned int j = 0; j < objs.size(); j++) {
		objs[j].render(camera, WIDTH, HEIGHT);
	}

}

void SceneManager::run()
{
	//GAME LOOP


	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void SceneManager::setupScene()
{

	loadSceneConfig();

	for (unsigned int j = 0; j < objs.size(); j++) {
		objs[j].setupScene();
	}


}


void SceneManager::loadSceneConfig() {
	// Load the file
	string code = loadFile("scene.md");
	objeto *obj;

	if (code.size() > 0) {
		vector <string> tokens;
		tokens = tokenize(code);
		string token;
		string token_value;
		string obj_path = "";
		string obj_name = "";
		float raio = 1.0f;
		float spinSpeed = 1.0f;
		bool inCurve = true;
		bool drawCurve = true;
		float size = 1.0f;
		bool move = true;
		bool spin = true;
		bool colorOnly = false;
		int temp = 0;
		int plusMoveSpeed = 1;

		for (unsigned int j = 0; j < tokens.size(); j++) {
			if (j == tokens.size()-1) {
				break;
			}

			token = tokens[j];
			token_value = tokens[j + 1];
			
			if (token == "{") {
				obj_path = "";
				obj_name = "";
				raio = 1.0f;
				spinSpeed = 1.0f;
				inCurve = true;
				drawCurve = true;
				size = 1.0f;
				move = true;
				spin = true;
				colorOnly = false;
				plusMoveSpeed = 1;
			}

			if (token == "obj_path") {
				obj_path = token_value;
			}
			if (token == "obj_name") {
				obj_name= token_value;
			}
			if (token == "raio") {
				raio = ::atof(token_value.c_str());
			}
			if (token == "spinSpeed") {
				spinSpeed = ::atof(token_value.c_str());
			}
			if (token == "inCurve") {
				temp = ::atoi(token_value.c_str());
				if (temp == 1) {
					inCurve = true;
				}
				else {
					inCurve = false;
				}
			}
			if (token == "drawCurve") {
				temp = ::atoi(token_value.c_str());
				if (temp == 1) {
					drawCurve = true;
				}
				else {
					drawCurve = false;
				}
			}
			if (token == "size") {
				size = ::atof(token_value.c_str());
			}
			if (token == "move") {
				temp = ::atoi(token_value.c_str());
				if (temp == 1) {
					move = true;
				}
				else {
					move= false;
				}
			}
			if (token == "spin") {
				temp = ::atoi(token_value.c_str());
				if (temp == 1) {
					spin = true;
				}
				else {
					spin = false;
				}
			}
			if (token == "colorOnly") {
				temp = ::atoi(token_value.c_str());
				if (temp == 1) {
					colorOnly = true;
				}
				else {
					colorOnly = false;
				}
			}
			if (token == "plusMoveSpeed") {
				plusMoveSpeed = ::atoi(token_value.c_str());
			}


			if (token == "}") {
				obj = new objeto(obj_path, obj_name);

				obj->raio = raio;
				obj->spinSpeed = spinSpeed;
				obj->inCurve = inCurve;
				obj->drawCurve = drawCurve;
				obj->size = size;
				obj->move = move;
				obj->spin= spin;
				obj->spinSpeed = spinSpeed;
				obj->colorOnly = colorOnly;
				obj->plusMoveSpeed = plusMoveSpeed;
				objs.push_back(*obj);



			}
		}
	}
}

string SceneManager::loadFile(string path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);
	file.open(path);
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	return fileStream.str();
}



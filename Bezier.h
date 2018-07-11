#pragma once

// GLEW e GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "shader.h"
#include "Camera.h"



using namespace std;

class Bezier
{
public:
	Bezier();
	~Bezier();
	Shader* shader;
	void genCurve(int resolution);
	void draw(Camera* camera, float w, float h);
	void addPoint(glm::vec3 p);
	vector <glm::vec3> curve; //pontos da curva gerada
protected:
	vector <glm::vec3> points; //pontos de controle
	glm::mat4 M;
	GLuint VAO, VBO, cVBO;
};


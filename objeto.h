#pragma once

#include <glm/glm.hpp>
#include "loader.h"
#include <vector>
#include "Shader.h"
#include "Mesh2.h"
#include "GL\glew.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "bezier.h"
#include "Camera.h"


class objeto{
public:
	
	glm::mat4 modelMatrix;
	std::vector<Mesh2> meshs;
	
	Bezier bezier;
	bool move = true;
	bool spin = true;
	bool drawCurve = true;
	float raio = 1.0;
	bool inCurve = true;
	bool colorOnly = false;
	float size = 1.0f;

	glm::mat4 rotateM;
	glm::mat4 scaleM;
	glm::mat4 transformM;

	Shader * shader;
	Camera *camera;

	objeto(string objPath, string objFileName);
	void setupScene();
	void render(Camera* camera, float w, float h);
	void setBezier(unsigned int resolution);



	//Timing
	double elapsed_seconds;
	double previous_seconds;
	double current_seconds;
	double spinSpeed = 0.5f;
	int plusMoveSpeed = 1;

	int curveIndex = 0;

};
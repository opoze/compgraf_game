#include "objeto.h"
#include "bezier.h"

objeto::objeto(
	string objPath,
	string objFileName
) {
	loader *l = new loader();
	meshs = l->proccessObjFile(objPath, objFileName);
}



void objeto::setupScene() {

	if (colorOnly) {
		shader = new Shader("./shaders/transformations-color.vs", "./shaders/transformations-color.frag");
	}
	else {
		shader = new Shader("./shaders/transformations.vs", "./shaders/transformations.frag");
	}
	
	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - previous_seconds;
	previous_seconds = current_seconds;

	setBezier(400 * elapsed_seconds);

	for (unsigned int i = 0; i < meshs.size(); i++) {
		meshs[i].setupScene();
		//meshs[i].print();
	}
}

void objeto::setBezier(unsigned int resolution) {
	float controlPoints[] = {
		1.0f * raio, 0.0f * raio,
		0.0f * raio, 1.0f * raio,
		-1.0f * raio, 0.0f * raio,
		
		-1.0f * raio, 0.0f * raio,
		-1.0f * raio, 0.0f * raio,
		-1.0f * raio, -1.0f * raio,
		1.0f * raio, 0.0f *raio
	};

	//float controlPoints[] = {
	//	1.0f * raio, 0.0f * raio,
	//	0.0f * raio, 1.0f * raio,
	//	-1.0f * raio, 0.0f * raio
	//};

	for (int i = 0; i < 14; i += 2)
	{
		//bezier.addPoint(glm::vec3(controlPoints[i], controlPoints[i + 1], 0.0f));
		bezier.addPoint(glm::vec3(controlPoints[i], 0.0f, controlPoints[i + 1]));
	}

	bezier.genCurve(resolution);
}

void objeto::render(Camera* camera, float w, float h) {

	shader->Use();

	//GLint objectColorLoc = glGetUniformLocation(shader->Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(shader->Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(shader->Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");

	//glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
	glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

	GLint viewLoc = glGetUniformLocation(shader->Program, "view");
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Create transformations


	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(shader->Program, "transform");

	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - previous_seconds;
	previous_seconds = current_seconds;
	
	if(spin){
		rotateM = glm::rotate(rotateM, (GLfloat)(spinSpeed * elapsed_seconds), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	scaleM = glm::scale(glm::mat4(), glm::vec3(size * 0.1f, size * 0.1f, size * 0.1f));

	if (move) {
		transformM = glm::translate(glm::mat4(), bezier.curve[curveIndex]);
	}
	else {
		if (inCurve) {
			transformM = glm::translate(glm::mat4(), bezier.curve[0]);
		}
	}

	modelMatrix = transformM * scaleM * rotateM;


	for (unsigned int i = 0; i < meshs.size(); i++) {
		meshs[i].render(transformLoc, modelMatrix, shader, colorOnly);
	}


	if (drawCurve) {
		//bezier.draw(transformLoc, glm::mat4(), shader);
		bezier.draw(camera, w, h);
	}

	if (curveIndex + plusMoveSpeed >= bezier.curve.size() -1) {
		curveIndex = 0;
	}
	else {
		curveIndex += plusMoveSpeed;
	}
}

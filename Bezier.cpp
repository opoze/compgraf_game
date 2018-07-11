#include "Bezier.h"

Bezier::Bezier()
{
	shader = new Shader("./shaders/transformations-color.vs", "./shaders/transformations-color.frag");

	M = glm::mat4x4(
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	);

}


Bezier::~Bezier()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &cVBO);
}

void Bezier::genCurve(int resolution)
{
	float step = 1.0 / (float)resolution;

	float t = 0;

	int nControlPoints = points.size();
	// 0 , 1
	for (int i = 0; i < points.size()-3; i+=3)
	{
		//int k = 0;
		for (float t = 0.0; t<=1.0; t += step)
		{
			glm::vec3 p;
			
			glm::vec4 T(t*t*t, t*t, t, 1);

			glm::mat4x3 G(
				points[i + 0].x, points[i + 0].y, points[i + 0].z,
				points[i + 1].x, points[i + 1].y, points[i + 1].z,
				points[i + 2].x, points[i + 2].y, points[i + 2].z,
				points[i + 3].x, points[i + 3].y, points[i + 3].z
			);

			p = G * M * T;  //---------

			curve.push_back(p);
		}
	}

	vector <glm::vec3> colors;
	for (int i = 0; i < curve.size(); i++)
	{
		colors.push_back(glm::vec3(0.5, 0.5, 0.5));
	}

	shader->Use();

	//Gera os buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &cVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, curve.size() * sizeof(GLfloat) * 3, &curve[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat) * 3, &colors[0], GL_STATIC_DRAW);

	// Position attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}

//void Bezier::draw(GLint transformLoc, glm::mat4 transform)
void Bezier::draw(Camera* camera, float w, float h)
{
	shader->Use();


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

	glLineWidth(0.2);
	glPointSize(0.1);
	glBindVertexArray(VAO);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	glDrawArrays(GL_LINE_STRIP, 0, curve.size());
	glBindVertexArray(0);
}

void Bezier::addPoint(glm::vec3 p)
{
	points.push_back(p);
}

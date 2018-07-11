#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
};

class Mesh {
public:
	/*  Mesh Data  */
	int vertexFloats = 11;
	int indexFloats = 3;
	GLfloat *vtcs;
	unsigned int *idcs;
	unsigned int idcsSize;
	unsigned int vtcsSize;
	unsigned int VAO, VBO, EBO;
	int vertexSize;
	int normalSize;
	int textureSize;
	int indexSize;

	/*  Functions  */
	Mesh(vector <glm::vec3> vertex, vector <glm::vec3> normals, vector <glm::vec2> textures, vector <glm::vec3> vertexIndex, vector <glm::vec3> normalIndex, vector <glm::vec3> textureIndex)
	{

		vertexSize = vertex.size();
		normalSize = normals.size();
		textureSize = textures.size();
		indexSize = vertexIndex.size();
		
		vtcsSize = vertexFloats * vertexSize;
		vtcs = new GLfloat[vtcsSize];
		idcsSize = indexFloats * indexSize;
		idcs = new unsigned int[idcsSize];

		unsigned int indice;

		for (unsigned int i = 0; i < vertexSize; i++) {

			// Coordenadas de vertices 3 floats
			vtcs[(i * 11) + 0] = glm::value_ptr(vertex[i])[0];
			vtcs[(i * 11) + 1] = glm::value_ptr(vertex[i])[1];
			vtcs[(i * 11) + 2] = glm::value_ptr(vertex[i])[2];

			// Cores 3 floats
			vtcs[(i * 11) + 3] = 0.0f;
			vtcs[(i * 11) + 4] = 1.0f;
			vtcs[(i * 11) + 5] = 0.0f;

			// Coordenadas de normais, 3 floats
			if (normalSize == vertexSize && normalIndex.size() > i) {

				//int normalIndex0 = normalIndex[i][0]-1;
				//int normalIndex1 = normalIndex[i][1]-1;
				//int normalIndex2 = normalIndex[i][2]-1;
				//int i0 = normalIndex0 / 3;
				//int ia0 = normalIndex0 % 3;
				//int i1 = normalIndex1 / 3;
				//int ia1 = normalIndex1 % 3;
				//int i2 = normalIndex2 / 3;
				//int ia2 = normalIndex2 % 3;
				//GLfloat teste = glm::value_ptr(normals[i0])[ia0];
				//GLfloat teste1 = glm::value_ptr(normals[i1])[ia1];
				//GLfloat teste2 = glm::value_ptr(normals[i2])[ia2];
				//vtcs[(i * 11) + 6] = teste;
				//vtcs[(i * 11) + 7] = teste1;
				//vtcs[(i * 11) + 8] = teste2;


				vtcs[(i * 11) + 6] = glm::value_ptr(normals[i])[0];
				vtcs[(i * 11) + 7] = glm::value_ptr(normals[i])[1];
				vtcs[(i * 11) + 8] = glm::value_ptr(normals[i])[2];
			}
			else {
				vtcs[(i * 11) + 6] = 0.0f;
				vtcs[(i * 11) + 7] = 0.0f;
				vtcs[(i * 11) + 8] = 0.0f;
			}

			// Coordenadas de textura, 2 floats
			if (textureSize == vertexSize && textureIndex.size() > i) {
				indice = textureIndex[i][0];
				vtcs[(indice * 11) + 9] = glm::value_ptr(textures[indice])[0];
				vtcs[(indice * 11) + 10] = glm::value_ptr(textures[indice])[1];
			}
			else {
				vtcs[(indice * 11) + 9] = 0.0f;
				vtcs[(indice * 11) + 10] = 0.0f;
			}
		}

		for (unsigned int i = 0; i < indexSize; i++) {
			idcs[i * 3 + 0] = glm::value_ptr(vertexIndex[i])[0]-1;
			idcs[i * 3 + 1] = glm::value_ptr(vertexIndex[i])[1]-1;
			idcs[i * 3 + 2] = glm::value_ptr(vertexIndex[i])[2]-1;
		}
	}

	void print()
	{
		cout << "Vertices" << endl;
		for (unsigned int a = 0; a < vtcsSize; a++) {
			if (a % 11 == 0) {
				cout << endl;
			}
			cout << vtcs[a] << "\t";
		}

		cout << endl << "Indices" << endl;
		for (unsigned int a = 0; a < idcsSize; a++) {
			if (a % 3 == 0 ){
				cout << endl;
			}
			cout << idcs[a] << "\t";
		}
	}

	void setupScene()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vtcsSize * sizeof(GLfloat), vtcs, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idcsSize * sizeof(GLint), idcs, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	}

	void render(GLint transformLoc, glm::mat4 transform) {
		glBindVertexArray(VAO);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glDrawElements(GL_TRIANGLES, idcsSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	/*  Render data  */
	/*  Functions    */
};

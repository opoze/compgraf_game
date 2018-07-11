#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "rhostring.hpp"
#include "stb_image.h"
#include "shader.h"


using namespace std;

class Mesh2 {
public:
	GLfloat *vtcs;
	GLint *idcs;
	unsigned int vtcsSize;
	unsigned int idcsSize;
	unsigned int VAO, VBO, EBO;
	string materialFileName;
	string materialName;

	// Material vars
	float Ns = 0;
	float Ni = 0;
	float d = 0;
	float Tf[3] = { 0, 0, 0 };
	float illum = 0;
	float Ka[3] = { 0, 0, 0 };
	float Kd[3] = { 0, 0, 0 };
	float Ks[3] = { 0, 0, 0 };
	string map_Kd = "";
	
	bool useMaterial = false;
	bool useTexture = false;

	GLuint texture;

	/*  Functions  */
	Mesh2(
		vector <float> vertex,
		vector <float> normals,
		vector <float> textures,
		vector <int> vertexIndex,
		vector <int> normalIndex,
		vector <int> textureIndex,
		string mtlFileName,
		string mtlName
	)
	{
		// sizes
		unsigned int vertexIndexSize = vertexIndex.size();
		unsigned int normalIndexSize = normalIndex.size();
		unsigned int textureIndexSize = textureIndex.size();

		// quantidade de floats por descrição de vertice ao shader
		// 3 para vertices, 3 para cores, 3 para vetor normal e 2 para texturas
		unsigned int qtdFloatsPerVertex = 11;

		// vtcs
		vtcsSize = qtdFloatsPerVertex * vertexIndex.size();
		vtcs = new GLfloat[vtcsSize];

		// idcs
		idcsSize = vertexIndexSize;
		idcs = new GLint[idcsSize];

		unsigned int vertexIndice = 0;
		unsigned int normalIndice = 0;
		unsigned int textureIndice = 0;

		bool hasVertexIndice = false;
		bool hasNormalIndice = false;
		bool hasTextureIndice = false;

		for (unsigned int i = 0; i < vertexIndex.size(); i++) {

			// Inicializa variaveis que indicam se ha indices
			hasVertexIndice = hasNormalIndice = hasTextureIndice = false;

			// inicializa vetor vtc com 0
			vtcs[i * 11 + 0] = 0.0f;
			vtcs[i * 11 + 1] = 0.0f;
			vtcs[i * 11 + 2] = 0.0f;
			vtcs[i * 11 + 3] = 0.0f;
			vtcs[i * 11 + 4] = 0.0f;
			vtcs[i * 11 + 5] = 0.0f;
			vtcs[i * 11 + 6] = 0.0f;
			vtcs[i * 11 + 7] = 0.0f;
			vtcs[i * 11 + 8] = 0.0f;
			vtcs[i * 11 + 9] = 0.0f;
			vtcs[i * 11 + 10] = 0.0f;

			// Obtem indice (indice do vetor que conten vertices)
			if (vertexIndex.size() >= i + 1) {
				vertexIndice = vertexIndex[i] - 1;
				hasVertexIndice = true;
			}
			if (normalIndex.size() >= i + 1) {
				normalIndice = normalIndex[i] - 1;
				hasNormalIndice = true;
			}
			if (textureIndex.size() >= i + 1) {
				textureIndice = textureIndex[i] - 1;
				hasTextureIndice = true;
			}


			if (hasVertexIndice) {
				vtcs[i * 11 + 0] = vertex[3 * vertexIndice + 0];
				vtcs[i * 11 + 1] = vertex[3 * vertexIndice + 1];
				vtcs[i * 11 + 2] = vertex[3 * vertexIndice + 2];
			}

			vtcs[i * 11 + 3] = 0.9f;
			vtcs[i * 11 + 4] = 0.6f;
			vtcs[i * 11 + 5] = 0.0f;

			if (hasNormalIndice) {
				//if (normalIndex.size() >= (3 * normalIndice + 2)) {
				// tem 1446 normais
				// o indice aponta para um trio de normais
				//1446 - 1629
					float teste = normals[3 * normalIndice + 0];
					vtcs[i * 11 + 6] = teste;
					teste = normals[3 * normalIndice + 1];
					vtcs[i * 11 + 7] = teste;
					teste = normals[3 * normalIndice + 2];
					vtcs[i * 11 + 8] = teste;
				//}
			}

			if (hasTextureIndice) {
				//if (textureIndex.size() >= (2 * textureIndice + 1)) {
					vtcs[i * 11 + 9]  = textures[2 * textureIndice + 0];
					vtcs[i * 11 + 10] = textures[2 * textureIndice + 1];
				//}
			}

			idcs[i] = i;
		}

		// se conseguir seta useMaterial
		loadMaterial(mtlFileName, mtlName);
		setupTexture();
	}

	void print()
	{
		
		unsigned int count = 0;
		for (unsigned int a = 0; a < vtcsSize; a++) {

			if (count == 0) {
				cout << endl << "Vertice: " << endl;
			}

			cout << vtcs[a] << endl;

			if (count == 10) {
				cout << endl;
				count = 0;
			}
			else {
				count++;
			}
		}

		cout << endl << "Indices" << endl;
		for (unsigned int a = 0; a < idcsSize; a++) {
			if (a % 3 == 0) {
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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Normal attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// Texture attribute
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	}

	void render(GLint transformLoc, glm::mat4 transform, Shader* shader, bool colorOnly) {

		if (!colorOnly) {
			// Passar Uniforme useMaterial e useTexture
			GLint useMaterialLoc = glGetUniformLocation(shader->Program, "useMaterial");
			GLint useTextureLoc = glGetUniformLocation(shader->Program, "useTexture");
			if (useMaterial) {
				glUniform1i(useMaterialLoc, 1);
			}
			else {
				glUniform1i(useMaterialLoc, -1);
			}

			if (useTexture) {
				glUniform1i(useTextureLoc, 1);
			}
			else {
				glUniform1i(useTextureLoc, -1);
			}

			if (useTexture) {
				// Passar textura para o shader
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
			}

			// Passar materias para shader para usar com iluminação
			if (useMaterial) {
				// Ka, Coeficiente de luz ambiente
				GLint kaLoc = glGetUniformLocation(shader->Program, "ka");
				glUniform3f(kaLoc, Ka[0], Ka[1], Ka[2]);

				// Ks, Coeficiente de luz specular
				GLint ksLoc = glGetUniformLocation(shader->Program, "ks");
				glUniform3f(ksLoc, Ks[0], Ks[1], Ks[2]);

				// Kd, Coeficiente de luz difusa
				GLint kdLoc = glGetUniformLocation(shader->Program, "kd");
				glUniform3f(kdLoc, Kd[0], Kd[1], Kd[2]);
			}
		}

		// Model Matrix
		glBindVertexArray(VAO);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glDrawElements(GL_TRIANGLES, idcsSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	string loadFile(string path)
	{
		std::ifstream file;
		file.exceptions(std::ifstream::badbit);
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		return fileStream.str();
	}

	void loadMaterial(string mtlFileName, string mtlName) {

		// Load the file
		string code = loadFile(mtlFileName);

		if (code.size() > 0) {
			useMaterial = true;

			vector <string> tokens;
			tokens = tokenize(code);
			string token;

			if (tokens.size() == 0) {
				useMaterial = false;
			}
			else {
				useMaterial = true;
			}

			// Aux Vars
			bool newMtl = false;
			bool gettingMaterial = false;
			GLfloat temp;

			for (unsigned int j = 0; j < tokens.size(); j++) {

				token = tokens[j];
			
				if (token == "newmtl") { newMtl = true; }
				if (newMtl && token == mtlName) { gettingMaterial = true; }

				if (Ns == -1) {
					temp = ::atof(token.c_str());
					Ns = temp; 
				}

				if (Ni == -1) {
					temp = ::atof(token.c_str());
					Ni = temp;
				}

				if (d == -1) {
					temp = ::atof(token.c_str());
					d = temp;
				}

				if (Tf[0] == -1) {
					temp = ::atof(token.c_str());
					Tf[0] = temp;
					Tf[1] = -1;
				}

				if (Tf[1] == -1) {
					temp = ::atof(token.c_str());
					Tf[1] = temp;
					Tf[2] = -1;
				}

				if (Tf[2] == -1) {
					temp = ::atof(token.c_str());
					Tf[2] = temp;
				}

				if (illum == -1) {
					temp = ::atof(token.c_str());
					illum = temp;
				}

				if (Ka[0] == -1) {
					temp = ::atof(token.c_str());
					Ka[0] = temp;
					Ka[1] = -1;
				}

				if (Ka[1] == -1) {
					temp = ::atof(token.c_str());
					Ka[1] = temp;
					Ka[2] = -1;
				}

				if (Ka[2] == -1) {
					temp = ::atof(token.c_str());
					Ka[2] = temp;
				}

				if (Kd[0] == -1) {
					temp = ::atof(token.c_str());
					Kd[0] = temp;
					Kd[1] = -1;
				}

				if (Kd[1] == -1) {
					temp = ::atof(token.c_str());
					Kd[1] = temp;
					Kd[2] = -1;
				}

				if (Kd[2] == -1) {
					temp = ::atof(token.c_str());
					Kd[2] = temp;
				}

				if (Ks[0] == -1) {
					temp = ::atof(token.c_str());
					Ks[0] = temp;
					Ks[1] = -1;
				}

				if (Ks[1] == -1) {
					temp = ::atof(token.c_str());
					Ks[1] = temp;
					Ks[2] = -1;
				}

				if (Ks[2] == -1) {
					temp = ::atof(token.c_str());
					Ks[2] = temp;
				}

				if (map_Kd == "-1") {
					map_Kd = token;
					//when finish getting the meterial break the loop
					break;
				}

				if (gettingMaterial) {
					if (token == "Ns") { Ns = -1; }
					if (token == "Ni") { Ni = -1; }
					if (token == "d") { d = -1; }
					if (token == "Tf") { Tf[0] = -1; }
					if (token == "illum") { illum = -1; }
					if (token == "Ka") { Ka[0] = -1; }
					if (token == "Kd") { Kd[0] = -1; }
					if (token == "Ks") { Ks[0] = -1; }
					if (token == "map_Kd") { map_Kd = "-1"; }
				}


			}
	
		}
	}

	// Pode-se usar perlin nouse para gerar uma textura aleatória
	void setupTexture() {

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);

		unsigned char *data = stbi_load(map_Kd.c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			useTexture = true;
		}
		else {
			useTexture = false;
			cout << "Nao foi possivel carregar a textura" << map_Kd.c_str() <<  endl;
			// aqui é possível gerar textura aleatória e carregar em data
		}


		if (useTexture) {
			// load and create a texture 
			// -------------------------
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

			glActiveTexture(GL_TEXTURE0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		stbi_image_free(data);
	}
};

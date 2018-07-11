#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "rhostring.hpp"
#include "Mesh2.h"
#include <GL/glew.h>

class loader
{
public:

	std::string code;
	vector<Mesh2> meshs;

	void loadFile(string path)
	{
		std::ifstream file;
		file.exceptions(std::ifstream::badbit);
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		code = fileStream.str();
	}

	vector<Mesh2> proccessObjFile(string path, string fileName)
	{
		code = "";
		meshs.clear();

		string a(path);
		string b(fileName);
		loadFile((a+b).c_str());

		vector <string> tokens;
		tokens = tokenize(code);

		vector <float> vertex;
		vector <float> normals;
		vector <float> textures;

		vector <int> vertexIndex;
		vector <int> normalIndex;
		vector <int> textureIndex;

		bool gettingVertex = false;
		bool gettingNormal = false;
		bool gettingTexture = false;
		bool gettingIndex = false;
		bool gettingMtllib = false;
		bool gettingUseMtl = false;

		string mtlFile;
		string mtlName;

		GLfloat temp;
		string token;
		vector <string> auxIndex;
		int i = 0;
		bool newGroup = false;
		bool createMesh = false;

		for (unsigned int j = 0; j<tokens.size(); j++) {
			token = tokens[j];

			if (gettingVertex) {
				temp = ::atof(token.c_str());
				vertex.push_back(temp);
				i++;
			}

			if (gettingNormal) {
				temp = ::atof(token.c_str());
				normals.push_back(temp);
				i++;
			}

			if (gettingTexture) {
				temp = ::atof(token.c_str());
				textures.push_back(temp);
				i++;
			}

			if (gettingIndex) {
				auxIndex = explode(token, '/');
				if (auxIndex.size() >= 1) {
					temp = ::atoi(auxIndex[0].c_str());
					vertexIndex.push_back(temp);
				}
				if (auxIndex.size() >= 2) {
					temp = ::atoi(auxIndex[1].c_str());
					textureIndex.push_back(temp);
				}
				if (auxIndex.size() >= 3) {
					temp = ::atoi(auxIndex[2].c_str());
					normalIndex.push_back(temp);
				}
				i++;
			}

			if (gettingMtllib) {
				mtlFile = path + token;
				gettingMtllib = false;
			}

			if (gettingUseMtl) {
				mtlName = token;
				gettingUseMtl = false;
			}

			if (i == 3 || (i == 2 && gettingTexture)) {
				gettingVertex = gettingIndex = gettingNormal = gettingTexture = gettingMtllib = gettingUseMtl = false;
				i = 0;
			}

			if (token == "v") {
				gettingVertex = true;
				gettingIndex = gettingNormal = gettingTexture = gettingMtllib = gettingUseMtl = false;
				if (newGroup) { createMesh = true; }
			}
			else if (token == "vn") {
				gettingNormal = true;
				gettingIndex = gettingTexture = gettingVertex = gettingMtllib = gettingUseMtl = false;
				if (newGroup) { createMesh = true; }
			}
			else if (token == "vt") {
				gettingTexture = true;
				gettingIndex = gettingNormal = gettingVertex = gettingMtllib = gettingUseMtl = false;
				if (newGroup) { createMesh = true; }
			}
			else if (token == "f") {
				gettingIndex = true;
				gettingVertex = gettingNormal = gettingVertex = gettingMtllib = gettingUseMtl = false;
			}
			else if (token == "g") {
				newGroup = true;
				gettingVertex = gettingIndex = gettingNormal = gettingTexture = gettingMtllib =  gettingUseMtl = false;
			}
			else if (token == "mtllib") {
				gettingMtllib = true;
				gettingVertex = gettingIndex = gettingNormal = gettingTexture = gettingUseMtl = false;
			}
			else if (token == "usemtl") {
				gettingUseMtl = true;
				gettingVertex = gettingIndex = gettingNormal = gettingTexture = gettingMtllib = false;
			}

			if (createMesh) {
				//cout << "anus" << endl;
				Mesh2 *mesh = new Mesh2(vertex, normals, textures, vertexIndex, normalIndex, textureIndex, mtlFile, mtlName);
				meshs.push_back(*mesh);
				newGroup = false;
				createMesh = false;
			}
		}

		//if (newGroup) {
			//cout << "anus1" << endl;
			Mesh2 *mesh = new Mesh2(vertex, normals, textures, vertexIndex, normalIndex, textureIndex, mtlFile, mtlName);
			meshs.push_back(*mesh);
		//}
		return meshs;


	}

	const vector<string> explode(const string& s, const char& c)
	{
		string buff{ "" };
		vector<string> v;

		for (auto n : s)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if (buff != "") v.push_back(buff);

		return v;
	}

};


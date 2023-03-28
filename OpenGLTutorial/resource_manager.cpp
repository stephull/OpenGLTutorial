#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

map<string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char* vsfile, const char* fsfile, const char* gshader, string name) {
	Shaders[name] = loadShaderFromFile(vsfile, fsfile, gshader);
	return Shaders[name];
}

Shader ResourceManager::GetShader(string name) {
	return Shaders[name];
}

void ResourceManager::Clear() {
	for (auto iter : Shaders) {
		glDeleteProgram(iter.second.id);
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vsfile, const char* fsfile, const char* gshader) {
	string vertexCode, fragmentCode, geometryCode;

	try {
		ifstream vertexShaderFile(vsfile);
		ifstream fragmentShaderFile(fsfile);
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (gshader != nullptr) {
			ifstream geometryShaderFile(gshader);
			stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (exception e) {
		cout << "Shader error: Failed to read shader files" << endl;
	}

	const char* vscode = vertexCode.c_str();
	const char* fscode = fragmentCode.c_str();
	const char* gcode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vscode, fscode, (gshader != nullptr) ? gcode : nullptr);
	return shader;
}
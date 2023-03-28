#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "shader.h"

using namespace std;

class ResourceManager {
public:
	static map<string, Shader> Shaders;

	static Shader LoadShader(const char* vsfile, const char* fsfile, const char* gshader, string name);
	static Shader GetShader(string name);

	static void Clear();

private:
	ResourceManager() {};
	
	static Shader loadShaderFromFile(const char* vsfile, const char* fsfile, const char* gshader = nullptr);
};

#endif
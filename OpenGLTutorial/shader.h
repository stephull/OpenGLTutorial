#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
	unsigned int id;

	Shader() {}

	Shader& Use();

	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	
	void SetFloat(const char* name, float val, bool useShader = false);
	void SetInteger(const char* name, int val, bool useShader = false);
	
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, const glm::vec2& val, bool useShader = false);

	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3& val, bool useShader = false);

	void SetVector4f(const char* name, float x, float y, float z, float a, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4& val, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4& mat, bool useShader = false);

private:
	void checkCompileErrors(unsigned int object, string type);
};

#endif
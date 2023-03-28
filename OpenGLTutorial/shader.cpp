#include "shader.h"

#include <iostream>

Shader& Shader::Use() {
	glUseProgram(this->id);
	return *this;
}

void Shader::Compile(
	const char* vertexSource, const char* fragmentSource, const char* geometrySource
) {
	unsigned int sVertex, sFragment, gShader;

	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, nullptr);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");

	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, nullptr);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");

	if (geometrySource != nullptr) {
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, nullptr);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}

	this->id = glCreateProgram();
	glAttachShader(this->id, sVertex);
	glAttachShader(this->id, sFragment);
	if (geometrySource != nullptr) {
		glAttachShader(this->id, gShader);
	}

	glLinkProgram(this->id);
	checkCompileErrors(this->id, "PROGRAM");

	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr) {
		glDeleteShader(gShader);
	}
}

void Shader::SetFloat(const char* name, float val, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint floatLoc = glGetUniformLocation(this->id, name);
	glUniform1f(floatLoc, val);
}

void Shader::SetInteger(const char* name, int val, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint intLoc = glGetUniformLocation(this->id, name);
	glUniform1i(intLoc, val);
}

void Shader::SetVector2f(const char* name, float x, float y, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector2Loc = glGetUniformLocation(this->id, name);
	glUniform2f(vector2Loc, x, y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& val, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector2Loc = glGetUniformLocation(this->id, name);
	glUniform2f(vector2Loc, val.x, val.y);
}

void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector3Loc = glGetUniformLocation(this->id, name);
	glUniform3f(vector3Loc, x, y, z);
}

void Shader::SetVector3f(const char* name, const glm::vec3 &val, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector3Loc = glGetUniformLocation(this->id, name);
	glUniform3f(vector3Loc, val.x, val.y, val.z);
}

void Shader::SetVector4f(const char* name, float x, float y, float z, float a, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector4Loc = glGetUniformLocation(this->id, name);
	glUniform4f(vector4Loc, x, y, z, a);
}

void Shader::SetVector4f(const char* name, const glm::vec4 &val, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint vector4Loc = glGetUniformLocation(this->id, name);
	glUniform4f(vector4Loc, val.x, val.y, val.z, val.w);
	// notice the val.w compared to the variable a
}

void Shader::SetMatrix4(const char* name, const glm::mat4& mat, bool useShader) {
	if (useShader) {
		this->Use();
	}

	GLint matLoc = glGetUniformLocation(this->id, name);
	const GLfloat* matValue = glm::value_ptr(mat);

	glUniformMatrix4fv(matLoc, 1, false, matValue);
}

void Shader::checkCompileErrors(unsigned int object, string type) {
	int success;
	char infoLog[1024];

	if (type == "PROGRAM") {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, nullptr, infoLog);
			cout << "Program error: link-time error with type " << type << "\n" << infoLog << '\n' << endl;
		}
	}
	else {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, nullptr, infoLog);
			cout << "Shader error: compile-time error with type " << type << "\n" << infoLog << "\n" << endl;
		}
	}
}
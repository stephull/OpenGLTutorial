#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

using Stream = std::ifstream;
using StringStream = std::stringstream;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertPath, const char* fragPath) {
		string vertCode, fragCode;
		Stream vsFile, fsFile;

		static constexpr int NG = Stream::failbit | Stream::badbit;
		vsFile.exceptions(NG);
		fsFile.exceptions(NG);

		try {
			vsFile.open(vertPath);
			fsFile.open(fragPath);

			StringStream vsStream, fsStream;
			vsStream << vsFile.rdbuf();
			fsStream << fsFile.rdbuf();

			vsFile.close();
			fsFile.close();

			vertCode = vsStream.str();
			fragCode = fsStream.str();
		}
		catch (Stream::failure& e) {
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
		}

		const char* vsCode = vertCode.c_str();
		const char* fsCode = fragCode.c_str();
		
		unsigned int vertex, fragment;
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vsCode, nullptr);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fsCode, nullptr);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const string& name, bool val) const {
		uint32_t uniform = glGetUniformLocation(ID, name.c_str());
		glUniform1i(uniform, (int)val);
	}

	void setInt(const string& name, int val) const {
		uint32_t uniform = glGetUniformLocation(ID, name.c_str());
		glUniform1i(uniform, val);
	}

	void setFloat(const string& name, float val) const {
		uint32_t uniform = glGetUniformLocation(ID, name.c_str());
		glUniform1f(uniform, val);
	}

private:
	void checkCompileErrors(unsigned int shader, string type) {
		int success;
		char infoLog[1024];

		if (type == "PROGRAM") {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				cout << "ERROR::PROGRAM_LINKED_ERROR of type: " << type << '\n' << infoLog << endl;
			}
		}
		else {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				cout << "ERROR::SHADER_COMPILE_ERROR of type: " << type << '\n' << infoLog << endl;
			}
		}
	}
};

#endif
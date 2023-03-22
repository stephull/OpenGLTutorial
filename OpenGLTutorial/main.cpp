// Triangle Test Program using OpenGL (C++)

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <map>

using namespace std;

void print_shader_error(const char*, char*);
const char* print_shader_src(const char*);
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
vector<float> randomFloats(void);

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

map<const char*, const char*> shaderTypes;
vector<float> rgb;

unsigned int vertexShader;

int success;
char infoLog[512];

int main(void) {
	cout << "Hello, world!\n" << endl;

	GLFWwindow* window;
	const unsigned int winW = 1280;
	const unsigned int winH = 720;
	const char* title = "OpenGLTest";

	bool init = glfwInit();
	if (!init) {
		const char* err_msg = "Failed to open program.";
		cout << err_msg << endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(winW, winH, title, nullptr, nullptr);
	if (!window) {
		const char* err_msg = "Failed to create a new GLFW window.";
		cout << err_msg << endl;

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	GLADloadproc addr = (GLADloadproc)glfwGetProcAddress;
	if (!gladLoadGLLoader(addr)) {
		const char* err_msg = "Failed to initialize GLAD";
		cout << err_msg << endl;
		exit(EXIT_FAILURE);
	}

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		print_shader_error("VERTEX::COMPILATION_FAILED", infoLog);
	}

	// shader source texts
	const char* fragmentSource1 = print_shader_src("RED");
	const char* fragmentSource2 = print_shader_src("BLUE");
	const char* fragmentSource3 = print_shader_src("GREEN");
	const char* fragmentSource4 = print_shader_src("BLACK");
	const char* fragmentSource5 = print_shader_src("WHITE");

	// fragment shader 1
	unsigned int fShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource1 = (const GLchar*)fragmentSource1;

	glShaderSource(fShader1, 1, &fSource1, nullptr);
	glCompileShader(fShader1);
	glGetShaderiv(fShader1, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader1, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	// fragment shader 2
	unsigned int fShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource2 = (const GLchar*)fragmentSource2;

	glShaderSource(fShader2, 1, &fSource2, nullptr);
	glCompileShader(fShader2);
	glGetShaderiv(fShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader2, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	// fragment shader 3
	unsigned int fShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource3 = (const GLchar*)fragmentSource3;

	glShaderSource(fShader3, 1, &fSource3, nullptr);
	glCompileShader(fShader3);
	glGetShaderiv(fShader3, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader3, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	// fragment shader 4
	unsigned int fShader4 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource4 = (const GLchar*)fragmentSource4;

	glShaderSource(fShader4, 1, &fSource4, nullptr);
	glCompileShader(fShader4);
	glGetShaderiv(fShader4, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader4, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	// fragment shader 5
	unsigned int fShader5 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource5 = (const GLchar*)fragmentSource1;

	glShaderSource(fShader5, 1, &fSource5, nullptr);
	glCompileShader(fShader5);
	glGetShaderiv(fShader5, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader5, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	// shader program 1
	unsigned int prog1 = glCreateProgram();
	glAttachShader(prog1, vertexShader);
	glAttachShader(prog1, fShader1);
	glLinkProgram(prog1);
	glGetProgramiv(prog1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog1, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(fShader1);

	// shader program 2
	unsigned int prog2 = glCreateProgram();
	glAttachShader(prog2, vertexShader);
	glAttachShader(prog2, fShader2);
	glLinkProgram(prog2);
	glGetProgramiv(prog2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog2, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(fShader2);

	// shader program 3
	unsigned int prog3 = glCreateProgram();
	glAttachShader(prog3, vertexShader);
	glAttachShader(prog3, fShader3);
	glLinkProgram(prog3);
	glGetProgramiv(prog3, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog3, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(fShader3);

	// shader program 4
	unsigned int prog4 = glCreateProgram();
	glAttachShader(prog4, vertexShader);
	glAttachShader(prog4, fShader4);
	glLinkProgram(prog4);
	glGetProgramiv(prog4, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog4, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(fShader4);

	// shader program 5
	unsigned int prog5 = glCreateProgram();
	glAttachShader(prog5, vertexShader);
	glAttachShader(prog5, fShader5);
	glLinkProgram(prog5);
	glGetProgramiv(prog5, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog5, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(fShader5);

	float t1[] = {
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	float t2[] = {
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	float t3[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};
	float t4[] = {
		0.0f, -0.5f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		-0.0f, -1.0f, 0.0f
	};
	float t5[] = {
		0.0f, -0.5f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	float i1[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i2[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i3[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i4[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i5[] = {
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f
	};

	rgb = randomFloats();

	unsigned int VBO[5], VAO[5], EBO[5];
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(5, EBO);

	// for program 1
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t1), t1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i1), i1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for program 2
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t2), t2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i2), i2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for program 3
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t3), t3, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i3), i3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for program 4
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t4), t4, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i4), i4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// for program 5
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t5), t5, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i5), i5, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(rgb.at(0), rgb.at(1), rgb.at(2), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// for program 1
		glUseProgram(prog1);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 2
		glUseProgram(prog2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// for program 3
		glUseProgram(prog3);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 4
		glUseProgram(prog4);
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 5
		glUseProgram(prog5);
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	glDeleteProgram(prog1);
	glDeleteProgram(prog2);
	glDeleteProgram(prog3);
	glDeleteProgram(prog4);
	glDeleteProgram(prog5);

	cout << "\nThank you for using this program. Goodbye!" << endl;

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void print_shader_error(const char* s, char* log) {
	cout << "ERROR::SHADER::" << s << '\n' << log << endl;
}

const char* print_shader_src(const char* color) {
	shaderTypes["WHITE"] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	shaderTypes["RED"] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	shaderTypes["GREEN"] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	shaderTypes["BLUE"] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
		"}\n\0";
	shaderTypes["BLACK"] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n\0";

	size_t numColor = shaderTypes.count(color);
	if (numColor <= 0) {
		const char* err_msg = "Failed to render color scheme for fragments.";
		cout << err_msg << endl;
		exit(EXIT_FAILURE);
	}

	return shaderTypes[color];
}

void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;

	const char* key_msg;

	switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(w, true);
			key_msg = "Pressed escape key, program will now terminate.";
			break;
		case GLFW_KEY_SPACE:
			key_msg = "Pressed space key, color of background has now changed.";
			rgb = randomFloats();
			break;
		default:
			key_msg = "Unknown key event detected.";
			break;
	}

	cout << key_msg << endl;
}

vector<float> randomFloats(void) {
	const int rgbLimit = 3;

	using Rand = std::default_random_engine;
	using FloatDistribute = std::uniform_real_distribution<float>;
	static Rand e;
	static FloatDistribute dist(0.0f, 1.0f);

	vector<float> ret;
	for (int i = 0; i < rgbLimit; i++) {
		float rand = dist(e);
		ret.emplace_back(rand);
	}

	for (auto& i : ret) {
		cout << i << " ";
	}
	cout << "(rgb colors)" << endl;

	return ret;
}
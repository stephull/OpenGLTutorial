// Triangle Test Program using OpenGL (C++)

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <map>

using namespace std;

void print_shader_error(const char*, char*);
const char* print_shader_src(const char*);

unsigned int create_shader(const char*);
unsigned int create_program(unsigned int);
void bind_all(unsigned int, unsigned int, unsigned int, float*, float*);
void use_and_draw_arrays(unsigned int, unsigned int);

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

	const char* fragmentSource1 = print_shader_src("RED");
	const char* fragmentSource2 = print_shader_src("BLUE");
	const char* fragmentSource3 = print_shader_src("GREEN");
	const char* fragmentSource4 = print_shader_src("BLACK");
	const char* fragmentSource5 = print_shader_src("WHITE");

	unsigned int fragmentShader1 = create_shader(fragmentSource1);
	unsigned int fragmentShader2 = create_shader(fragmentSource2);
	unsigned int fragmentShader3 = create_shader(fragmentSource3);
	unsigned int fragmentShader4 = create_shader(fragmentSource4);
	unsigned int fragmentShader5 = create_shader(fragmentSource5);

	unsigned int shaderProgram1 = create_program(fragmentShader1);
	unsigned int shaderProgram2 = create_program(fragmentShader2);
	unsigned int shaderProgram3 = create_program(fragmentShader3);
	unsigned int shaderProgram4 = create_program(fragmentShader4);
	unsigned int shaderProgram5 = create_program(fragmentShader5);

	float t1[9] = {
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	float t2[9] = {
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	float t3[9] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};
	float t4[9] = {
		0.0f, -0.5f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		-0.0f, -1.0f, 0.0f
	};
	float t5[9] = {
		0.0f, -0.5f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	float i1[6] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i2[6] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i3[6] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i4[6] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float i5[6] = {
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f
	};

	rgb = randomFloats();

	unsigned int VBO[5], VAO[5], EBO[5];
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(5, EBO);

	bind_all(VAO[0], VBO[0], EBO[0], t1, i1);
	bind_all(VAO[1], VBO[1], EBO[1], t2, i2);
	bind_all(VAO[2], VBO[2], EBO[2], t3, i3);
	bind_all(VAO[3], VBO[3], EBO[3], t4, i4);
	bind_all(VAO[4], VBO[4], EBO[4], t5, i5);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(rgb.at(0), rgb.at(1), rgb.at(2), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		use_and_draw_arrays(shaderProgram1, VAO[0]);
		use_and_draw_arrays(shaderProgram2, VAO[1]);
		use_and_draw_arrays(shaderProgram3, VAO[2]);
		use_and_draw_arrays(shaderProgram4, VAO[3]);
		use_and_draw_arrays(shaderProgram5, VAO[4]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
	glDeleteProgram(shaderProgram3);
	glDeleteProgram(shaderProgram4);
	glDeleteProgram(shaderProgram5);

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

unsigned int create_shader(const char* source) {
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fSource = (const GLchar*)source;

	glShaderSource(fShader, 1, &fSource, nullptr);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	return fShader;
}

unsigned int create_program(unsigned int shader) {
	unsigned int prog = glCreateProgram();
	GLuint fShader = (GLuint)shader;

	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fShader);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}

	glDeleteShader(fShader);
	return prog;
}

void bind_all(unsigned int vao, unsigned int vbo, unsigned int ebo, float* tri, float* ind) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(&tri), &tri, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(&ind), &ind, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void use_and_draw_arrays(unsigned int prog, unsigned int vertex) {
	glUseProgram(prog);
	glBindVertexArray(vertex);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
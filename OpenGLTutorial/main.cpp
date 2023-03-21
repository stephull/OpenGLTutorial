#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

void print_shader_error(const char*, char*);
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
std::vector<float> randomFloats(void);

const char* vertexShaderSource;
const char* fragmentShaderSource;

std::vector<float> rgb;
float redValueBg;
float greenValueBg;
float blueValueBg;

int main(void) {
	std::cout << "Hello, world!" << std::endl;

	GLFWwindow* window;
	const unsigned int winW = 800;
	const unsigned int winH = 450;
	const char* title = "OpenGLTest";

	bool init = glfwInit();
	if (!init) {
		const char* err_msg = "Failed to open program.";
		std::cout << err_msg << std::endl;

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
		std::cout << err_msg << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	GLADloadproc addr = (GLADloadproc)glfwGetProcAddress;
	if (!gladLoadGLLoader(addr)) {
		const char* err_msg = "Failed to initialize GLAD";
		std::cout << err_msg << std::endl;

		exit(EXIT_FAILURE);
	}

	vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\n\0";
	fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	int success;
	char infoLog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		print_shader_error("VERTEX::COMPILATION_FAILED", infoLog);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	rgb = randomFloats();
	redValueBg = rgb[0];
	greenValueBg = rgb[1];
	blueValueBg = rgb[2];

	float triangle1[] = {
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};
	float triangle2[] = {
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	float triangle3[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};

	float indices1[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float indices2[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float indices3[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	unsigned int VBO[3], VAO[3], EBO[3];
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle3), triangle3, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(redValueBg, greenValueBg, blueValueBg, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	glDeleteProgram(shaderProgram);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void print_shader_error(const char* s, char* log) {
	std::cout << "ERROR::SHADER::" << s << '\n' << log << std::endl;
}

void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) {
		return;
	}

	const char* key_msg;

	switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(w, true);
			key_msg = "Pressed escape key, program will now terminate.";
			break;
		case GLFW_KEY_SPACE:
			key_msg = "Pressed space key, color of background has now changed.";
			rgb = randomFloats();
			redValueBg = rgb[0];
			greenValueBg = rgb[1];
			blueValueBg = rgb[2];
			break;
		default:
			key_msg = "Unknown key event detected.";
			break;
	}

	std::cout << key_msg << std::endl;
}

std::vector<float> randomFloats(void) {
	const int rgbLimit = 3;
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	std::vector<float> ret;
	for (int i = 0; i < rgbLimit; i++) {
		float rand = dist(e);
		ret.emplace_back(rand);
	}

	std::cout << "RGB COLORS: ";
	for (auto& i : ret) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	return ret;
}
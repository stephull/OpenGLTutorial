#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <map>
#include <deque>

using namespace std;

const char* print_shader_src(const char*);
void print_shader_error(const char*, char*);
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
vector<float> randomFloats(void);

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

vector<const char*> fragmentSources;
deque<unsigned int> fragmentShaders;
vector<unsigned int> shaderPrograms;

vector<float*> triangles;
vector<float*> indices;

vector<float> rgb;
float redValueBg, greenValueBg, blueValueBg;

map<const char*, const char*> shaderTypes;

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

	const unsigned int numOfColors = 5;
	const char* colors[numOfColors] = { "RED", "BLUE", "GREEN", "BLACK", "WHITE" };
	for (int i = 0; i < numOfColors; i++) {
		const char* color = colors[i];
		const char* temp = print_shader_src(color);
		if (!temp) {
			const char* err_msg = "Failed to render color scheme for fragments.";
			cout << err_msg << endl;
			exit(EXIT_FAILURE);
		}
		fragmentSources.push_back(temp);
		//cout << "1:" << temp << endl;
	}

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

	for (int i = 0; i < fragmentSources.size(); i++) {
		unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fSource = (const GLchar*)fragmentSources.at(i);
		
		glShaderSource(fShader, 1, &fSource, nullptr);
		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
			print_shader_error("FRAGMENT::COMPILATION_FAILED", infoLog);
		}

		fragmentShaders.push_back(fShader);
		//cout << "2:" << fShader << endl;
	}

	for (int j = 0; j < fragmentShaders.size(); j++) {
		unsigned int prog = glCreateProgram();
		GLuint fShader = fragmentShaders.at(j);
		
		glAttachShader(prog, vertexShader);
		glAttachShader(prog, fShader);
		glLinkProgram(prog);
		glGetProgramiv(prog, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(prog, 512, nullptr, infoLog);
			print_shader_error("PROGRAM::LINKING_FAILED", infoLog);
		}

		shaderPrograms.push_back(prog);
		//cout << "3:" << prog << endl;
		glDeleteShader(fShader);
		fragmentShaders.pop_front();
	}

	rgb = randomFloats();
	redValueBg = rgb.at(0);
	greenValueBg = rgb.at(1);
	blueValueBg = rgb.at(2);

	float t1[9] = {
		0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	}, t2[9] = {
		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	}, t3[9] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	}, t4[9] = {
		0.0f, -0.5f, 0.0f,
		-0.5f, -1.0f, 0.0f,
		-0.0f, -1.0f, 0.0f
	}, t5[9] = {
		0.0f, -0.5f, 0.0f,
		0.5f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};
	triangles = {t1, t2, t3, t4, t5};

	float i1[6] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	}, i2[6] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	}, i3[6] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	}, i4[6] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	}, i5[6] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	indices = { i1, i2, i3, i4, i5 };

	unsigned int VBO[5], VAO[5], EBO[5];
	glGenVertexArrays(5, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(5, EBO);

	for (int a = 0; a < sizeof(VAO) / sizeof(VAO[0]); a++) {
		glBindVertexArray(VAO[a]);

		float* t = triangles.at(a);
		//cout << "4:" << t << endl;
		glBindBuffer(GL_ARRAY_BUFFER, VBO[a]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(t), t, GL_STATIC_DRAW);
		
		float* i = indices.at(a);
		//cout << "5:" << i << endl;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[a]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i), i, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(redValueBg, greenValueBg, blueValueBg, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (
			int i = 0, j = 0; 
			i < shaderPrograms.size() && j < sizeof(VAO)/sizeof(VAO[0]);
			i++, j++
		) {
			glUseProgram(shaderPrograms.at(i));
			glBindVertexArray(VAO[j]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, EBO);

	for (int i = shaderPrograms.size()-1; i >= 0; i--) {
		glDeleteProgram(shaderPrograms.at(i));
		shaderPrograms.erase(shaderPrograms.begin() + i);
	}

	cout << "\nThank you for using this program. Goodbye!" << endl;

	glfwTerminate();
	exit(EXIT_SUCCESS);
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

	return shaderTypes.count(color) 
		? shaderTypes[color] 
		: nullptr;
}

void print_shader_error(const char* s, char* log) {
	cout << "ERROR::SHADER::" << s << '\n' << log << endl;
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
			redValueBg = rgb.at(0);
			greenValueBg = rgb.at(1);
			blueValueBg = rgb.at(2);
			break;
		default:
			key_msg = "Unknown key event detected.";
			break;
	}

	cout << key_msg << endl;
}

vector<float> randomFloats(void) {
	const int rgbLimit = 3;
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	vector<float> ret;
	for (int i = 0; i < rgbLimit; i++) {
		float rand = dist(e);
		ret.emplace_back(rand);
	}

	cout << "RGB COLORS: ";
	for (auto& i : ret) {
		cout << i << " ";
	}
	cout << endl;

	return ret;
}
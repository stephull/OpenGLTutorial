// Triangle Test Program using OpenGL (C++)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <stddef.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void cursor_pos_callback(GLFWwindow*, double, double);
vector<float> random_floats(void);

vector<float> rgb;

int main(void) {
	cout << "Hello, world!\n" << endl;

	GLFWwindow* window;
	const unsigned int winW = 800; //16x50
	const unsigned int winH = 450; //09x50
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
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	GLADloadproc addr = (GLADloadproc)glfwGetProcAddress;
	if (!gladLoadGLLoader(addr)) {
		const char* err_msg = "Failed to initialize GLAD";
		cout << err_msg << endl;
		exit(EXIT_FAILURE);
	}

	Shader shader1("shader.vs", "shader.fs");
	Shader shader2("shader.vs", "shader.fs");
	Shader shader3("upside_down.shader.vs", "shader.fs");
	Shader shader4("upside_down.shader.vs", "shader.fs");
	Shader shader5("upside_down.shader.vs", "shader.fs");

	float t1[18]{
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	}, t2[18]{
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	}, t3[18]{
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	}, t4[18]{
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	}, t5[18]{
		0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};

	float i1[6]{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	}, i2[6] {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	}, i3[6] {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	}, i4[6] {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	}, i5[6] {
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// for program 2
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t2), t2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i2), i2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// for program 3
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t3), t3, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i3), i3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// for program 4
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t4), t4, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i4), i4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// for program 5
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t5), t5, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i5), i5, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	rgb = random_floats();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(rgb.at(0), rgb.at(1), rgb.at(2), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// for program 1
		shader1.use();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 2
		float xoffset = 0.25f, yoffset = -0.5f;
		shader2.use();
		shader2.setFloat("xoffset", xoffset);
		shader2.setFloat("yoffset", yoffset);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// for program 3
		shader3.use();
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 4
		shader4.use();
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// for program 5
		shader5.use();
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(5, EBO);

	cout << "\nThank you for using this program. Goodbye!" << endl;

	glfwTerminate();
	exit(EXIT_SUCCESS);
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
			rgb = random_floats();
			break;
		default:
			key_msg = "Unknown key event detected.";
			break;
	}

	cout << key_msg << endl;
}

void cursor_pos_callback(GLFWwindow* w, double x, double y) {
	std::cout << "Cursor position: (" << x << ", " << y << ")" << endl;
}

vector<float> random_floats(void) {
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
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* w, int width, int height);
void processInput(GLFWwindow* w);

int main(void) {
	std::cout << "Hello, world!" << std::endl;

	GLFWwindow* window;
	const unsigned int winW = 800;
	const unsigned int winH = 450;
	const char* title = "OpenGLTest";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(winW, winH, title, nullptr, nullptr);
	if (window == NULL) {
		const char* err_msg = "Failed to create a new GLFW window.";
		std::cout << err_msg << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	GLADloadproc addr = (GLADloadproc)glfwGetProcAddress;
	if (!gladLoadGLLoader(addr)) {
		const char* err_msg = "Failed to initialize GLAD";
		std::cout << err_msg << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow* w) { 
	int temp = glfwGetKey(w, GLFW_KEY_ESCAPE);
	if (temp == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, true);
	}
}

void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}
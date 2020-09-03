#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// GLFW = Graphics Library Framework
// Pre-Steps
// 1. Add includes: GLFW, KHR (khronos?), glad (retrieves and stores location of functions in function pointers for later use)
// 2. Add libraries: glfw3.lib, opengl32.lib (64-bit opengl is also called opengl32.lib)
// 3. Add glad.c file to project and add it as existing item

// Process input (duh comment) - used at Line 49 in the render loop.
// No declaration so its up here
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	glfwInit();

	// glfwWindowHint is used to configure GLFW
	// Set major and minor to fail running GLFW is no proper OpenGL version detected
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Tries to create a window of width = 800, height = 600, with name LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize glad
	// GLADloadproc is the glad function to load the address of the OpenGL function pointers, os-specific
	// GLFW returns glfwGetProcAddress which defines the correct function based on which OS we're compiling for
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Define viewport (lower left corner of the window, width, height)
	// Not to be confused with GLFWwindow, this belongs to OpenGL
	// Sets normalized coordinates (-1,1) to be between (800, 600)
	glViewport(0, 0, 800, 600);

	// Call back function that GLFW calls when screen resizes to reset glViewport(0,0,width,height), declaration
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	// Registering the callback function with GLFW
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Simple render loop
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Rendering commands
		// Clears the screen on specified buffer
		// glClearColor = state setting function
		// glClear = state using function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swaps the front and back buffer (i think, like dx swap chain?)
		glfwSwapBuffers(window);
		// Check for events, keyboard, mouse, etc. (i.e like WinAPI in dx?)
		glfwPollEvents();
	}

	// Terminate window as soon as out of render loop
	glfwTerminate();
	return 0;
}

// Definition of screen resize callback function - used at Line 42
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
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

void BuildCompileShaders(int& shaderProgram) {
	const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

	// Vertex Shader - create, bind source, compile, check if success
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders to shader program - link, check for errors
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete shaders after binding
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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

	// Build, Compile, Bind to shader program
	int shaderProgram = glCreateProgram();
	BuildCompileShaders(shaderProgram);

	float vertices[] = {
		 0.5f, 0.5f, 0.0f, // top right
		 0.5f,-0.5f, 0.0f, // bottom right
		-0.5f,-0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f  // top left
	};

	unsigned int indices[] = {
		0,1,3, // first triangle
		1,2,3  // second triangle
	};

	// VBO = Vertex Buffer Object (stores the vertex data), VBA = Vertex Array Object (records further bindings) 
	unsigned int VBO, VAO, EBO;
	// Create
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 1. Bind VAO which will start storing attrib pointers and EBOs
	glBindVertexArray(VAO);

	// 2. Bind vertex buffer which AttribPointer will get their data from
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. Bind EBO which will be stored in VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Set Attrib Pointers which will be stored in VAO
	// (vertex attrib location, size 3 for Vec3, type, normalized coords false - already normalized, stride, offset)
	// Gets data from currently bound vertex buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 5. Unbind VBO and VAO - as bindings have been stored in VAO
	// Do not unbind EBO as EBO is stored directly in the VAO
	// Unbinding VBO is allowed as it is bound to attribute pointer once glVertexAttribPointer is called
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// For drawing in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		// Draw Triangle - bind shader program, bind VAO, draw, unbind VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 vertices, indices type GL_UNSIGNED_INT
		//glBindVertexArray(0);

		// Swaps the front and back buffer (i think, like dx swap chain?)
		glfwSwapBuffers(window);
		// Check for events, keyboard, mouse, etc. (i.e like WinAPI in dx?)
		glfwPollEvents();
	}

	// De-allocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Terminate window as soon as out of render loop
	glfwTerminate();
	return 0;
}

// Definition of screen resize callback function - used at Line 42
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
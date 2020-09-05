#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	using namespace std;
	// 1. retrieves vertex/fragment source from filepath
	// converts to char*
	string vertex_string;
	string fragment_string;
	ifstream vertex_shader_file;
	ifstream fragment_shader_file;
	// ensure ifstream objects can throw exceptions
	vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		// open files
		vertex_shader_file.open(vertexPath);
		fragment_shader_file.open(fragmentPath);
		// read file contents into streams
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();
		// close files
		vertex_shader_file.close();
		fragment_shader_file.close();
		// stream to string
		vertex_string = vertex_shader_stream.str();
		fragment_string = fragment_shader_stream.str();
	}
	catch (ifstream::failure e) {
		cout << "EROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}
	const char* vertex_shader_source = vertex_string.c_str();
	const char* fragment_shader_source = fragment_string.c_str();

	// 2. compile shader, create and bind to program, line program 
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// clean up resource
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, const bool& value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, const int& value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float& value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

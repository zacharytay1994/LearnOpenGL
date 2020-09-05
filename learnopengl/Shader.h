#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void Use() const;
	// uniform setters
	void SetBool(const std::string& name, const bool& value) const;
	void SetInt(const std::string& name, const int& value) const;
	void SetFloat(const std::string& name, const float& value) const;
private:
	unsigned int ID;
};

#endif
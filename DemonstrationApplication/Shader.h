#pragma once
#include "Common.h"

class Shader {
public:
	void Initialize(string shaderPath1, GLenum shaderType1, string shaderPath2, GLenum shaderType2);
	void LinkShader();
	void UseProgram();

	string ReadShaderFile(const string& filePath);
	GLuint LoadShader(GLenum shaderType, const string& shaderSource);

	void SetUniform(string propName, int value);
	void SetUniform(string propName, float value);
	void SetUniform(string propName, glm::vec2 value);
	void SetUniform(string propName, glm::vec3 value);
	void SetUniform(string propName, glm::vec4 value);
	void SetUniform(string propName, glm::mat3 value);
	void SetUniform(string propName, glm::mat4 value);

private:
	GLint shaderProgramID;
	
};
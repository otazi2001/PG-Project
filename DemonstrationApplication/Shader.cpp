#include "Shader.h"
#include "Common.h"

void Shader::Initialize(string shaderPath1, GLenum shaderType1, string shaderPath2, GLenum shaderType2) {
    string shaderSource1 = ReadShaderFile(shaderPath1);
    GLuint ShaderID1 = LoadShader(shaderType1, shaderSource1);

    std::string shaderSource2 = ReadShaderFile(shaderPath2);
    GLuint ShaderID2 = LoadShader(shaderType2, shaderSource2);

    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, ShaderID1);
    glAttachShader(shaderProgramID, ShaderID2);
}

void Shader::LinkShader() {
    glLinkProgram(shaderProgramID);
}

void Shader::UseProgram() {
    glUseProgram(shaderProgramID);
}

string Shader::ReadShaderFile(const string& filePath) {
    ifstream file(filePath);

    if (!file) {
        const char* errorMessage = "Error reading file: ";
        std::wstring wideErrorMessage(errorMessage, errorMessage + strlen(errorMessage));
        std::wstring wideFilePath(filePath.begin(), filePath.end());
        std::wstring message = wideErrorMessage + wideFilePath;
        MessageBox(NULL, message.c_str(), L"Error", MB_ICONERROR);

    }

    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return source;
}

GLuint Shader::LoadShader(GLenum shaderType, const string& shaderSource) {
    GLint shaderID = glCreateShader(shaderType);
    const GLchar* source = shaderSource.c_str();
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    // Check for shader compilation errors
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        cout << "failed to load Shader!" << endl;
    }


    return shaderID;
}

void Shader::SetUniform(string propName, int value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniform1i(uniLoc, value);
}

void Shader::SetUniform(string propName, float value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniform1fv(uniLoc, 1, &value);
}

void Shader::SetUniform(string propName, glm::vec2 value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniform2fv(uniLoc, 1, (GLfloat*)&value);
}

void Shader::SetUniform(string propName, glm::vec3 value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniform3fv(uniLoc, 1, (GLfloat*)&value);
}

void Shader::SetUniform(string propName, glm::vec4 value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniform4fv(uniLoc, 1, (GLfloat*)&value);
}

void Shader::SetUniform(string propName, glm::mat3 value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniformMatrix3fv(uniLoc, 1, FALSE, (GLfloat*)&value);
}

void Shader::SetUniform(string propName, glm::mat4 value) {
    int uniLoc = glGetUniformLocation(shaderProgramID, propName.c_str());
    glUniformMatrix4fv(uniLoc, 1, FALSE, (GLfloat*)&value);
}


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath),m_RendererID(0)
{

    ShaderProgramSource source = ParseShader(filepath);
    std::cout << source.VertexSource << std::endl;
    std::cout << source.FragmentSource << std::endl;
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}
void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int val)
{
    glUniform1i(GetUniformLocation(name), val);
}

void Shader::SetUniform1f(const std::string& name, float val)
{
    glUniform1f(GetUniformLocation(name), val);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 f0)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &f0[0][0]);
}
void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}
void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    glUniform3f(GetUniformLocation(name), f0, f1, f2);
}

int Shader::GetUniformLocation(const std::string& name)
{
   int location =  glGetUniformLocation(m_RendererID,name.c_str());
   if (location == -1)
       std::cout << "warning : uniform " << name << "doesn't exist " << std::endl;
   return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                // set mode to vertex
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }

    }
    return { ss[0].str(), ss[1].str() };
}



unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    //TODO : ERROR handling 
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        GLchar message[1024];// = (char*)_alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;


    }

    return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmenttShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmenttShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


#include "Shader.h"
#include "util.h"

#include <fstream>
#include <iostream>
#include <sstream>

template <>
constexpr std::string enumToString<ShaderType>(ShaderType type)
{
    switch (type)
    {
    case ShaderType::VERTEX: return "vertex";
    case ShaderType::FRAGMENT: return "fragment";
    case ShaderType::PROGRAM: return "program";
    }
    return "unknown";
}

Shader::Shader(OpenGLContext context, GLuint id) : context(context), id(id) {}

Shader::Shader(Shader&& other) noexcept : context(other.context)
{
    id = other.id;
    other.id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        this->~Shader();
        context = other.context;
        id = other.id;
        other.id = 0;
    }
    return *this;
}

Shader::~Shader()
{
    context.makeCurrent();
    glDeleteProgram(id);
}

GLuint Shader::getId() const noexcept
{
    return id;
}

void Shader::addUniformLocation(const std::string& name)
{
    context.makeCurrent();
    int loc{glGetUniformLocation(id, name.c_str())};
    if (loc == -1) throw std::invalid_argument("uniform location " + name + " does not exist");
    uniformLocations[name] = loc;
}

template <>
void Shader::setUniform<float>(const std::string& name, float value) const
{
    if (!uniformLocations.contains(name)) return;
    context.makeCurrent();
    glUniform1f(uniformLocations.at(name), value);
}

void Shader::use() const
{
    context.makeCurrent();
    glUseProgram(id);
}

void Shader::unuse() const
{
    context.makeCurrent();
    glUseProgram(0);
}

std::optional<Shader> Shader::loadFromFile(std::string_view vspath, std::string_view fspath, OpenGLContext context)
{
    std::string vscode, fscode;
    std::ifstream vsfile, fsfile;
    vsfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vsfile.open(vspath.data());
        fsfile.open(fspath.data());
        std::stringstream vsstream, fsstream;
        vsstream << vsfile.rdbuf();
        fsstream << fsfile.rdbuf();
        vsfile.close();
        fsfile.close();
        vscode = vsstream.str();
        fscode = fsstream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return std::nullopt;
    }
    context.makeCurrent();
    const char* cvscode{vscode.c_str()};
    const char* cfscode{fscode.c_str()};
    GLuint vertex{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertex, 1, &cvscode, nullptr);
    glCompileShader(vertex);
    if (!checkCompileError(vertex, ShaderType::VERTEX)) return std::nullopt;
    GLuint fragment{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragment, 1, &cfscode, nullptr);
    glCompileShader(fragment);
    if (!checkCompileError(fragment, ShaderType::FRAGMENT)) return std::nullopt;
    GLuint program{glCreateProgram()};
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    if (!checkCompileError(program, ShaderType::PROGRAM)) return std::nullopt;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return Shader(context, program);
}

bool Shader::checkCompileError(GLuint shader, ShaderType type)
{
    int success;
    char infoLog[1024];
    if (type != ShaderType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM_COMPILATION_ERROR of type: " << enumToString(type) << '\n' << infoLog << std::endl;
            return false;
        }
        return true;
    }
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM_LINKING_ERROR of type: " << enumToString(type) << '\n' << infoLog << std::endl;
        return false;
    }
    return true;
}

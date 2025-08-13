#pragma once
#include <optional>
#include <string_view>
#include <unordered_map>
#include <string>

#include "OpenGLContext.h"

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    PROGRAM,
};

class Shader 
{
private:
    OpenGLContext context;
    GLuint id;
    std::unordered_map<std::string, int> uniformLocations;
public:
    Shader(OpenGLContext context, GLuint id);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept;
    void addUniformLocation(const std::string& name);

    template<typename T>
    void setUniform(const std::string& name, const T& value) const {}

    void use() const;
    void unuse() const;
    [[nodiscard]] GLuint getId() const noexcept;
    ~Shader() noexcept;

    static std::optional<Shader> loadFromFile(std::string_view vspath, std::string_view fspath, OpenGLContext context);
private:
    static bool checkCompileError(GLuint shader, ShaderType type);
};

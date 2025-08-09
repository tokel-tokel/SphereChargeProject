#pragma once
#include <optional>
#include <string_view>
#include <glad/glad.h>

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    PROGRAM,
};

class Shader 
{
private:
    GLuint id;
public:
    explicit Shader(GLuint id);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept;
    [[nodiscard]] GLuint getId() const noexcept;
    ~Shader() noexcept;

    static std::optional<Shader> loadFromFile(std::string_view vspath, std::string_view fspath);
private:
    static bool checkCompileError(GLuint shader, ShaderType type);
};

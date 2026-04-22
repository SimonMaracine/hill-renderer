#include "hill/primitives/shader.hpp"

#include <format>
#include <ranges>
#include <utility>

#include <glad/gl.h>

namespace hill::shader {
    unsigned int shader_type_enum(ShaderType shader_type) {
        switch (shader_type) {
            case ShaderType::Vertex:
                return GL_VERTEX_SHADER;
            case ShaderType::Fragment:
                return GL_FRAGMENT_SHADER;
        }

        std::unreachable();
    }

    Shader::Shader(ShaderType shader_type) {
        m_shader = glCreateShader(shader_type_enum(shader_type));

        if (!m_shader) {
            throw ShaderError("Could not create shader");
        }
    }

    Shader::~Shader() {
        glDeleteShader(m_shader);
    }

    void Shader::compile(const std::string& source) const {
        const char* string = source.c_str();
        glShaderSource(m_shader, 1, &string, nullptr);
        glCompileShader(m_shader);

        if (!compilation_successful()) {
            throw CompileError(std::format("Could not compile shader: {}", info_log()));
        }
    }

    void Shader::compile(const std::vector<std::string>& sources) const {
        auto strings = std::make_unique<const char*[]>(sources.size());

        for (const auto& [i, source] : sources | std::views::enumerate) {
            strings[std::size_t(i)] = source.c_str();
        }

        glShaderSource(m_shader, int(sources.size()), strings.get(), nullptr);
        glCompileShader(m_shader);

        if (!compilation_successful()) {
            throw CompileError(std::format("Could not compile shader: {}", info_log()));
        }
    }

    bool Shader::compilation_successful() const {
        int compile_status {};
        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compile_status);

        return compile_status;
    }

    std::string Shader::info_log() const {
        int info_log_length {};
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &info_log_length);

        auto info_log = std::string(std::size_t(info_log_length), 0);
        glGetShaderInfoLog(m_shader, info_log_length, nullptr, info_log.data());

        return info_log;
    }

    Program::Program() {
        m_program = glCreateProgram();

        if (!m_program) {
            throw ShaderError("Could not create program");
        }
    }

    Program::~Program() {
        glDeleteProgram(m_program);
    }

    void Program::attach_shader(std::shared_ptr<Shader> shader) {

    }

    void Program::detach_shader(std::shared_ptr<Shader> shader) {

    }

    void Program::detach_shader(unsigned int shader) {

    }

    void Program::link() const {

    }

    void Program::use() const {
        glUseProgram(m_program);
    }

    void Program::unuse() const {
        glUseProgram(0);
    }
}

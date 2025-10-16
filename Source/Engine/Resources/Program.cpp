#include "Program.h"

namespace neu
{
	Program::Program() {
		m_program = glCreateProgram();
	}

	Program::~Program() {
		if (m_program) glDeleteProgram(m_program);
	}

	void Program::AttachShader(const res_t<Shader>& shader) {
		glAttachShader(m_program, shader->m_shader);
	}

	bool Program::Link() {
		glLinkProgram(m_program);

		GLint success = false;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::string infoLog(512, '\0');  // pre-allocate space
			GLsizei length;
			glGetProgramInfoLog(m_program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
			infoLog.resize(length);

			LOG_WARNING("Program link failed: {}", infoLog);

			glDeleteProgram(m_program);
			m_program = 0;

			return false;
		}

		return true;
	}

	void Program::SetUniform(const std::string& name, float value) {
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1f(location, value);
	}

	void Program::SetUniform(const std::string& name, const neu::vec2& value) {
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform2f(location, value.x, value.y);
	}



	GLint Program::GetUniformLocation(const std::string& name) {
		// find uniform location in map
		auto it = m_uniformLocations.find(name);
		// if not found, get uniform in program
		if (it == m_uniformLocations.end())
		{
			// get uniform in program, return -1 if not found
			GLint location = glGetUniformLocation(m_program, name.c_str());
			if (location == -1)
			{
				LOG_WARNING("Could not find uniform: {}", name);
			}
			m_uniformLocations[name] = location;
		}

		return m_uniformLocations[name];
	}
}
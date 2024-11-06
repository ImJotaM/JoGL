#include "Core.hpp"

class Shader {

public:

	Shader(const fs::path& vertexShaderPath, const fs::path& fragmentShaderPath) {

		std::string vertexSource = loadShaderSource(vertexShaderPath);
		std::string fragmentSource = loadShaderSource(fragmentShaderPath);

		GLuint vertex = compileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
		GLuint fragment = compileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

		program = linkProgram(vertex, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use() const {
		glUseProgram(program);
	}

	operator GLint() const { return program; }

	~Shader() {
		glDeleteProgram(program);
	}

private:

	std::string loadShaderSource(const fs::path& shaderPath) {

		if (fs::exists(shaderPath) && fs::is_regular_file(shaderPath) && shaderPath.extension() == ".glsl") {

			std::ifstream shaderFile(shaderPath);

			if (!shaderFile) {
				std::cerr << "Não foi possível abrir o shader: " << shaderPath.filename() << std::endl;
				return false;
			}

			std::stringstream shaderStream;

			shaderStream << shaderFile.rdbuf();
			shaderFile.close();

			return shaderStream.str();
		}
		else {
			std::cerr << "Shader inválido ou extensão incorreta: " << shaderPath.filename() << std::endl;
			return "";
		}
	}

	GLuint compileShader(const char* shaderSource, GLenum shaderType) {

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Erro ao compilar shader:\n" << infoLog << std::endl;
		}

		return shader;
	}

	GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {

		GLuint prog = glCreateProgram();
		glAttachShader(prog, vertexShader);
		glAttachShader(prog, fragmentShader);
		glLinkProgram(prog);

		GLint success;
		glGetShaderiv(prog, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(prog, 512, nullptr, infoLog);
			std::cerr << "Erro ao compilar shader:\n" << infoLog << std::endl;
		}

		return prog;
	}

	GLuint program = 0;
};
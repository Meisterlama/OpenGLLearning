#include "Shader.hpp"

Shader::Shader(const std::string path)
{
	_path = path;
	ID = init_shader(_path);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

unsigned int Shader::init_shader(const std::string path)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(std::filesystem::relative(path + ".vert"));
		fShaderFile.open(std::filesystem::relative(path + ".frag"));
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	check_compile_errors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	check_compile_errors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	check_compile_errors(ID, "PROGRAM");
	// delete the shaders as they're no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ID;
}

void Shader::check_compile_errors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::reload()
{
	glDeleteProgram(ID);
	init_shader(_path);
}

void Shader::set(const std::string &name, bool value) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::set(const std::string &name, int value) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set(const std::string &name, unsigned int value) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set(const std::string &name, float value) const
{
	use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set(const std::string &name, const glm::vec2 &value) const
{
	use();
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y) const
{
	use();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::set(const std::string &name, const glm::vec3 &value) const
{
	use();
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y, float z) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::set(const std::string &name, const glm::vec4 &value) const
{
	use();
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y, float z, float w) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::set(const std::string &name, const glm::mat2 &mat) const
{
	use();
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set(const std::string &name, const glm::mat3 &mat) const
{
	use();
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set(const std::string &name, const glm::mat4 &mat) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

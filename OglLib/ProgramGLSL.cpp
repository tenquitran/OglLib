#include "OglLib.h"

//////////////////////////////////////////////////////////////////////////

using namespace OglLib;

//////////////////////////////////////////////////////////////////////////


ProgramGLSL::ProgramGLSL()
{
}

ProgramGLSL::~ProgramGLSL()
{
	if (0 != m_program)
	{
		glDeleteProgram(m_program);
	}
}

bool ProgramGLSL::initialize(const ShaderCollection& shaders)
{
	m_shaderFiles = shaders;

	m_program = glCreateProgram();

	if (0 == m_program)
	{
		std::wcerr << L"glCreateProgram() failed\n";
		return false;
	}

	if (!buildProgram())
	{
		std::wcerr << L"Failed to build GLSL program\n";
		return false;
	}

	return true;
}

GLuint ProgramGLSL::getProgram() const
{
	return m_program;
}

bool ProgramGLSL::buildProgram()
{
	if (m_shaderFiles.empty())
	{
		std::wcerr << L"No shader files to build\n";
		return false;
	}

	// Process the list of shaders.

	std::vector<GLuint> shaderIds = {};

	for (const auto& itrShader : m_shaderFiles)
	{   // Enumerate shader files.

		// Read the shader's source.

		const std::string fileName = itrShader.second;

		std::ifstream shaderFile(fileName);
		if (!shaderFile.good())
		{
			std::wcerr << L"Failed to load shader file \"" << fileName.c_str() << L"\"\n";
			return false;
		}

		std::istreambuf_iterator<char> itrStream(shaderFile);

		std::string src;
		src.assign(itrStream, std::istreambuf_iterator<char>());

		shaderFile.close();

		// Create shader.

        GLuint shader = glCreateShader(itrShader.first);
		if (0 == shader)
		{
			std::wcerr << L"glCreateShader() failed\n";
			return false;
		}

		shaderIds.push_back(shader);

		// Compile the shader and add it to the GLSL program.

		if (!compileShader(shader, src, fileName))
		{
			std::wcerr << L"Failed to compile shader \"" << fileName.c_str() << L"\"\n";
			return false;
		}

		glAttachShader(m_program, shader);

	}   // Enumerate shader files.

	// Link the program and check the results.
	if (!linkProgram())
	{
		return false;    // the errors are already logged
	}

	// Clean up the shaders data.
	for (const auto& itr : shaderIds)
	{
		glDetachShader(m_program, itr);
		glDeleteShader(itr);
	}

	return true;
}

bool ProgramGLSL::compileShader(GLuint shader, const std::string& src, const std::string& fileName)
{
	const GLchar* pBuff = static_cast<const GLchar*>(src.c_str());

	glShaderSource(shader, 1, (const GLchar**)&pBuff, nullptr);

	glCompileShader(shader);

	GLint res = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);

	if (GL_TRUE != res)
	{
		GLint logLen = {};
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, spLog.get());

			std::wcerr << L"Failed to compile shader \"" << fileName.c_str() << L"\":\n" << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to compile shader %S:\n%S\n", fileName.c_str(), spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}

bool ProgramGLSL::linkProgram()
{
	glLinkProgram(m_program);

	GLint resLink = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &resLink);

	if (GL_TRUE != resLink)
	{
		GLint logLen = {};
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(m_program, logLen, &written, spLog.get());

			std::wcerr << L"Failed to link the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to link the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}

bool ProgramGLSL::validate() const
{
	glValidateProgram(m_program);

	GLint resValidation = GL_FALSE;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &resValidation);

	if (GL_TRUE != resValidation)
	{
		GLint logLen = {};
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(m_program, logLen, &written, spLog.get());

			std::wcerr << L"Failed to validate the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to validate the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}

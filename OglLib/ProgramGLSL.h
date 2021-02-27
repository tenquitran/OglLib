#pragma once


namespace OglLib
{
	// Collection of shaders.
	// Key: shader type; value: full or partial path to the shader file.
	typedef std::map<GLenum, std::string> ShaderCollection;


	// GLSL object; encapsulates shaders.
	class ProgramGLSL
	{
	public:
		ProgramGLSL();

		virtual ~ProgramGLSL();

		// Parameters: shaders - description of the shaders to load and build.
		bool initialize(const ShaderCollection& shaders);

		GLuint getProgram() const;

        // It's better to validate the program after setting all texture sampler uniforms 
		// (usually, at the end of the OpenGL scene initialization).
		// Otherwise, on using cubemap with some other texture, program validation will fail with this error:
		//
		// Validation failed! - Different sampler types for same sample texture unit in fragment shader.
		bool validate() const;

	private:
		ProgramGLSL(const ProgramGLSL&) = delete;
		ProgramGLSL& operator=(const ProgramGLSL&) = delete;

		// Build GLSL program.
		// Returns: true on success, false otherwise.
		// Throws: std::bad_alloc
		bool buildProgram();

		// Compile the shader.
		// Parameters: shader - OpenGL shader object;
		//             src - shader source code;
		//             fileName - shader file name (for logging).
        // Returns: true on success, false otherwise.
		bool compileShader(GLuint shader, const std::string& src, const std::string& fileName);

		// Link the GLSL program.
		// Returns: true on success, false otherwise.
		bool linkProgram();

	private:
		// GLSL program.
        GLuint m_program = {};

		// Shader files data.
		ShaderCollection m_shaderFiles;
	};
}

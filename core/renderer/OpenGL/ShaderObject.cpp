/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderObject.h
* @brief 
* @date 1/2/2011
*/

#include "ShaderObject.h"
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sstream>
#ifdef _WIN32 /*[*/
#include <io.h>
#endif /*]*/

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	ShaderObject::ShaderObject(ShaderType shaderType): _id (0), _shaderSize(0), _shaderType(shaderType)
	{
		printOpenGLError();
	}

	ShaderObject::ShaderObject(ShaderType shaderType, const std::string& shaderFilename): _id (0), _shaderSize(0), _shaderType(shaderType)
	{
		_filename = shaderFilename;
		loadShader();
	}

	ShaderObject::~ShaderObject () 
	{
		releaseResource();
	}

	void ShaderObject::releaseResource()
	{
		printOpenGLError();
		if (_shaderSize != 0)
		{
			glDeleteShader(_id);
		}
		printOpenGLError();

	}


	void ShaderObject::loadShader(const std::string& shaderFilename)
	{
		_filename = shaderFilename;
		char* ShaderSourceOriginal = readShaderSource(shaderFilename, _shaderSize);
		compileAttach(_id, getOpenGLShaderType(_shaderType), ShaderSourceOriginal);
		printOpenGLError();  // Check for OpenGL errors
	}

	void ShaderObject::loadShader()
	{
		loadShader(_filename);
	}

	void ShaderObject::reload()
	{
		char* ShaderSourceOriginal = readShaderSource(_filename, _shaderSize);
		compileAttach(_id, getOpenGLShaderType(_shaderType), ShaderSourceOriginal);
		printOpenGLError();  // Check for OpenGL errors
	}

	void ShaderObject::loadShaderByString(const std::string& shader)
	{
		compileAttach(_id, getOpenGLShaderType(_shaderType), shader);
		printOpenGLError();  // Check for OpenGL errors
	}

	void ShaderObject::compileAttach (GLuint & shader, GLenum type, const std::string& source) 
	{
		printOpenGLError();  // Check for OpenGL errors
		GLint shaderCompiled;
		shader = glCreateShader(type);
		printOpenGLError();  // Check for OpenGL errors
		const GLchar* pSource = ( const GLchar* ) source.c_str();
		glShaderSource (shader, 1, &pSource, NULL);
		printOpenGLError();  // Check for OpenGL errors
		glCompileShader (shader);
		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv (shader, GL_COMPILE_STATUS, &shaderCompiled);
		printOpenGLError();  // Check for OpenGL errors
		std::cerr<< std::endl <<"Shader: ["<< _filename <<"]"<< std::endl;
		printShaderInfoLog(shader);
		//glAttachShader (shaderProgram, shader);
		printOpenGLError ();  // Check for OpenGL errors
		//printProgramInfoLog () ;
	}

	unsigned int ShaderObject::getShaderSize (const std::string& filename) {
		int fd;
		unsigned int count = 0;
#ifdef _WIN32
		fd = _open(filename.c_str (), _O_RDONLY);
		if (fd != -1) {
			count = static_cast<unsigned int>(_lseek (fd, 0, SEEK_END) + 1);
			_close(fd);
		} else{
			//throw ShaderException (string ("getShaderSize: bad Shader File Name") + filename);
			std::cout<<"shader files "<<filename<<" doesn't exist"<<std::endl;
		}
#else
		fd = open (filename.c_str (), O_RDONLY);
		if (fd != -1) {
			count = static_cast<unsigned int>(lseek (fd, 0, SEEK_END) + 1);
			close(fd);
		}else{
#if defined(_DEBUG) || defined(DEBUG)

			std::cout<<"shader files doesn't exist"<<std::endl;

#endif

		}

#endif
		return count;
	}

	char* ShaderObject::readShaderSource(const std::string& shaderFilename, unsigned int & shaderSize) {
		shaderSize = getShaderSize(shaderFilename);
		FILE * fh = fopen (shaderFilename.c_str (), "r");

		if(fh == 0)
		{
			std::cout<<"shader file does not exist :"<<shaderFilename <<std::endl;
			return 0;
		}
		GLchar * shaderSource = new GLchar[shaderSize];
		fseek (fh, 0, SEEK_SET);
		int count = fread (shaderSource, 1, shaderSize, fh);
		shaderSource[count] = '\0';
		fclose (fh);

		return shaderSource;
	}

	void ShaderObject::printShaderInfoLog(GLuint shader) {
		int infologLength = 0;
		int charsWritten  = 0;
		GLchar *infoLog;

		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infologLength);
		printOpenGLError();  // Check for OpenGL errors

		if (infologLength > 0) {
			infoLog = new GLchar[infologLength];
			if (infoLog == NULL) {
				printf("ERROR: Could not allocate InfoLog buffer\n");
				exit(1);
			}
			glGetShaderInfoLog (shader, infologLength, &charsWritten, infoLog);
			std::cerr << "InfoLog:" << std::endl << infoLog << std::endl << std::endl;
			delete [] infoLog;
		}
		printOpenGLError();  // Check for OpenGL errors
	}

	GLenum ShaderObject::getOpenGLShaderType(ShaderType shaderType)
	{
		switch (shaderType) 
		{
		case SHADERTYPE_VERTEX:
			return GL_VERTEX_SHADER_ARB;
			break;
		case SHADERTYPE_TESSELLATION_CTRL:
			return GL_TESS_CONTROL_SHADER;
			break;
		case SHADERTYPE_TESSELLATION_EVAL:
			return GL_TESS_EVALUATION_SHADER;
			break;
		case SHADERTYPE_GEOMETRY:
			return GL_GEOMETRY_SHADER_EXT;
			break;
		case SHADERTYPE_FRAGMENT:
			return GL_FRAGMENT_SHADER_ARB;
			break;
		case SHADERTYPE_COMPUTE:
			return GL_COMPUTE_SHADER;
			break;
		default:
			break;
		}

		return GL_INVALID_ENUM;
	}
}

/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSLGpuProgram.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLSLGpuProgram.h"
#include <fstream>
#include "VertexShader.h"
#include "FragmentShader.h"
#include "ComputeShader.h"
#include "TessCtrlShader.h"
#include "TessEvalShader.h"
#include "GeometryShader.h"

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

	GLSLGpuProgram::GLSLGpuProgram(const std::string& name): GpuProgram(name), _pShader(0), _tessellate(false),_incrementTexture(0)
	{
	}

	GLSLGpuProgram::~GLSLGpuProgram()
	{
		release();

	}

	void GLSLGpuProgram::release()
	{
		printOpenGLError();
		if(_pShader!=0)
			delete _pShader;
		_pShader = 0;
		printOpenGLError();
	}

	void GLSLGpuProgram::reCompile()
	{
		_pShader->reload();
		printOpenGLError();
	}

	void GLSLGpuProgram::loadShader(ShaderProgram* shaderprogram)
	{
		release();
		printOpenGLError();
		_pShader = shaderprogram;
		printOpenGLError();
	}

	void GLSLGpuProgram::loadShaderFiles(std::map<ShaderType, std::string> files)
	{
		_pShader = new ShaderProgram();
		std::map<ShaderType, std::string>::iterator itor = files.begin();
		for(;itor != files.end(); ++itor)
		{
			if(itor->first == SHADERTYPE_VERTEX)
			{
				VertexShader * shader = new VertexShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_TESSELLATION_CTRL)
			{
				TessCtrlShader * shader = new TessCtrlShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_TESSELLATION_EVAL)
			{
				TessEvalShader * shader = new TessEvalShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_GEOMETRY)
			{
				GeometryShader * shader = new GeometryShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_FRAGMENT)
			{
				FragmentShader * shader = new FragmentShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_COMPUTE)
			{
				ComputeShader * shader = new ComputeShader();
				shader->loadShader(itor->second);
				_pShader->addShader(shader);
			}
			else if(itor->first == SHADERTYPE_INVALID)
			{
				assert(0 && "GLSLGpuProgram class: invalid shader object");
			}
		}
		_pShader->link();
	}

	void GLSLGpuProgram::use(std::map<std::string const, Texture*> textureMap)
	{

		printOpenGLError();
		if(_pShader == 0) return;

		_pShader->bind();
		printOpenGLError ();

		std::map<std::string const, Texture*>::const_iterator texturesItor;

		for(texturesItor = textureMap.begin(); texturesItor != textureMap.end(); texturesItor++)
		{

			const std::string& bindingName = (*texturesItor).first;
			Texture* texture  = (*texturesItor).second;
			if(texture==0 || bindingName.empty()) continue;
			GLint location = getUniformLocation(bindingName);
			if(location != -1)
			{
				glUniform1i(location, _incrementTexture);
				glActiveTexture(GL_TEXTURE0 + _incrementTexture);
				texture->use();
				printOpenGLError ();
				_incrementTexture++;
			}
		}

		glActiveTexture(GL_TEXTURE0);
		printOpenGLError ();  // Check for OpenGL errors
		_pShader->unbind();

	}



	void GLSLGpuProgram::use()
	{
		printOpenGLError();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError ();  // Check for OpenGL errors

	}

	void GLSLGpuProgram::unUse()
	{
		printOpenGLError();
		if(_pShader == 0) return;
		_pShader->unbind();
		printOpenGLError();
	}



	void GLSLGpuProgram::bindTexture(std::string const bindingName, Texture* texture)
	{
		printOpenGLError();
		if(_pShader == 0) return;
		if(texture == NULL || bindingName.empty()) return;
		printOpenGLError();
		_pShader->bind();
		printOpenGLError ();
		GLint location = getUniformLocation(bindingName);
		if(location != -1)
		{
			glUniform1i(location, _incrementTexture);
			glActiveTexture(GL_TEXTURE0 + _incrementTexture);
			texture->use();
			printOpenGLError ();
			++_incrementTexture;
		}
		glActiveTexture(GL_TEXTURE0);
		printOpenGLError();  // Check for OpenGL errors
		_pShader->unbind();
		printOpenGLError();
	}


	void GLSLGpuProgram::unBindBindingTextures()
	{
		if(_pShader == 0) return;

		_pShader->bind();
		for(; _incrementTexture > 0; _incrementTexture--)
		{
			glActiveTexture(GL_TEXTURE0 +  _incrementTexture - 1);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		//std::cout<<_incrementTexture<<std::endl;
		glDisable(GL_TEXTURE_2D);
		_pShader->unbind();

	}


	GLint GLSLGpuProgram::getUniformLocation(const std::string& name) const
	{
		printOpenGLError();
		if(_pShader == 0) return -1;
		_pShader->bind();
		return _pShader->getUniformLocation(name.c_str());

	}
	GLint GLSLGpuProgram::getAttributLocation(const std::string& name) const
	{
		printOpenGLError();
		if(_pShader == 0) return -1;
		_pShader->bind();
		return _pShader->getAttributLocation(name.c_str());
	}

	void GLSLGpuProgram::setUniformVariable(const std::string& name, int data)
	{
		printOpenGLError();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform1i(location, data);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();

	}
	void GLSLGpuProgram::setUniformVariable(const std::string& name, float data)
	{
		printOpenGLError();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform1f(location, data);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();

	}
	void GLSLGpuProgram::setUniformVariable(const std::string& name, bool data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform1i(location,(data?1:0));
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();

	}
	void GLSLGpuProgram::setUniformVariable(const std::string& name, const Vec2f& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform2fv(location, 1, &data[0]);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();

	}
	void GLSLGpuProgram::setUniformVariable(const std::string& name, const Vec3f& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform3fv(location, 1, &data[0]);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();
	}

	void GLSLGpuProgram::setUniformVariable(const std::string& name, const Vec4f& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform4fv(location, 1, &data[0]);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();
	}

	void GLSLGpuProgram::setUniformVariable(const std::string& name, const Matrix4f& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniformMatrix4fv(location, 1, false, &(data[0][0]));
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();


	}


	void GLSLGpuProgram::setUniformVariable(const std::string& name, const std::vector<Vec4f>& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform4fv(location, data.size(),  &(data[0][0]));
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();
	}


	void GLSLGpuProgram::setUniformVariable(const std::string& name, const std::vector<Matrix4f>& data)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniformMatrix4fv(location, data.size(), false, &(data[0][0][0]));
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();


	}


	void GLSLGpuProgram::setUniformVariable(const std::string& name, Vec4f* data, int size)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniform4fv(location, size,  &data[0][0]);
		}
		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();
	}


	void GLSLGpuProgram::setUniformVariable(const std::string& name, Matrix4f* data, int size)
	{
		printOpenGLError ();
		if(_pShader == 0) return;
		_pShader->bind();
		printOpenGLError();
		GLint location = getUniformLocation(name);
		printOpenGLError ();
		if(location != -1)
		{
			glUniformMatrix4fv(location, size, false, &(data[0][0][0]));
		}
		printOpenGLError();
		if(_pShader == 0) return;
		_pShader->unbind();
		printOpenGLError();

	}

	//TODO
	void GLSLGpuProgram::setUniformBufferObject(const std::string& name, UniformBufferObject* bufferObject)
	{

		printOpenGLError ();
		if(_pShader == 0) return;
		printOpenGLError ();
		_pShader->bind();
		printOpenGLError();
		GLuint blockIndex = _pShader->getUniformBlockIndex(name);
		if(blockIndex != GL_INVALID_INDEX)
		{
			int blockbindingpoint = _pShader->getUniformBlockBinding(blockIndex);
			// GLint blockSize = _pShader->getUniformBlockSize(blockIndex);

			bufferObject->use(blockbindingpoint);
			_pShader->uniformBlockBinding(blockIndex, blockbindingpoint);
		}

		printOpenGLError();

		_pShader->unbind();
		printOpenGLError();

	}


	void GLSLGpuProgram::transformFeedbackVaryingsSetting(size_t count, const char **varyings, GLenum bufferMode)
	{
		if(_pShader != 0)
			_pShader->transformFeedbackVaryingsSetting(count, varyings, bufferMode);
	}


	void GLSLGpuProgram::drawIndexVBO(GLenum primitive, VBOUnit& pos, VBOUnit& normal, VBOUnit& texCoord, IndexVBO* index)
	{
		printOpenGLError ();
		if(_pShader != 0)
		{
			_pShader->bind();
			printOpenGLError();
			size_t nComponentPerVertex = 3;
			size_t nTextureCoordComponentPerVertex = 2;

			GLint locationTex = getAttributLocation(texCoord._attributeName);
			if(locationTex != -1)
			{
				texCoord._pVBO->use();
				glVertexAttribPointer( locationTex, texCoord._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
				printOpenGLError();
				glEnableVertexAttribArray(locationTex);
			}

			printOpenGLError();

			GLint locationNormal = getAttributLocation(normal._attributeName);
			if(locationNormal != -1)
			{
				normal._pVBO->use();
				glVertexAttribPointer(locationNormal, normal._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
				printOpenGLError();
				glEnableVertexAttribArray(locationNormal);
			}

			printOpenGLError();

			GLint locationVertex = getAttributLocation(pos._attributeName);
			if(locationVertex != -1)
			{
				pos._pVBO->use();
				glVertexAttribPointer(locationVertex, pos._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
				printOpenGLError();
				glEnableVertexAttribArray(locationVertex);
			}

			printOpenGLError();

			index->use();
			if(isTessellationGpuProgram())
			{
				//TODO : tesselation is difficult
				glPatchParameteri(GL_PATCH_VERTICES, 3);
				glDrawElements( GL_PATCHES, index->dataSize(), GL_UNSIGNED_INT, 0 );

			}else
			{
				glDrawElements( primitive, index->dataSize(), GL_UNSIGNED_INT, 0 );
			}

			printOpenGLError();

			index->unUse();
			printOpenGLError();



			if(locationTex != -1)
			{
				glDisableVertexAttribArray(locationTex);
				printOpenGLError();
				texCoord._pVBO->unUse();
				printOpenGLError();
			}
			if(locationNormal != -1)
			{
				glDisableVertexAttribArray(locationNormal);
				printOpenGLError();
				normal._pVBO->unUse();
				printOpenGLError();
			}
			if(locationVertex != -1)
			{
				glDisableVertexAttribArray(locationVertex);
				printOpenGLError();
				pos._pVBO->unUse();
				printOpenGLError();
			}
			_pShader->unbind();
		}

		printOpenGLError();
	}



	void GLSLGpuProgram::drawVBO(VBO* vbo, GLenum primitive, int numberComponents)
	{
		vbo->use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(primitive, 0, vbo->getSize());
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		vbo->unUse();

	}

	void GLSLGpuProgram::drawVBO(VBOUnit& uvbo)
	{

		printOpenGLError ();
		if(_pShader != 0)
		{
			_pShader->bind();
			printOpenGLError();
			GLint location = getAttributLocation(uvbo._attributeName);
			if(location != -1)
			{
				uvbo._pVBO->use();
				glVertexAttribPointer(location, uvbo._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
				printOpenGLError();
				glEnableVertexAttribArray(location);
			}

			glDrawArrays(uvbo._primitive, 0, uvbo._pVBO->getSize());

			printOpenGLError();
			if(location != -1)
			{
				glDisableVertexAttribArray(location);
				printOpenGLError();
				uvbo._pVBO->unUse();
				printOpenGLError();
			}
			_pShader->unbind();
		}

	}

	void GLSLGpuProgram::drawFeedBack(VBO* vbo, TransformFeedbackBufferObject* tfbo, int numberComponents)
	{

		vbo->use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		tfbo->draw();
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		vbo->unUse();
	}

	void GLSLGpuProgram::drawFeedBack(VBOUnit& uvbo, TransformFeedbackBufferObject* tfbo)
	{
		printOpenGLError ();
		if(_pShader != 0){
			_pShader->bind();
			printOpenGLError();
			GLint location = getAttributLocation(uvbo._attributeName);
			if(location != -1)
			{
				uvbo._pVBO->use();
				tfbo->draw();
				printOpenGLError();
				glEnableVertexAttribArray(location);
			}

			tfbo->draw();

			printOpenGLError();
			if(location != -1)
			{
				glDisableVertexAttribArray(location);
				printOpenGLError();
				uvbo._pVBO->unUse();
				printOpenGLError();
			}
			_pShader->unbind();
		}
	}


	void GLSLGpuProgram::drawCaptureTransform(VBO* vbo, TransformFeedbackBufferObject* tfbo, GLenum primitive, int numberComponents)
	{
		tfbo->beginFeedback();
		vbo->use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(primitive, 0, vbo->getSize());
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		vbo->unUse();
		tfbo->endFeedback();
	}

	void GLSLGpuProgram::drawCaptureTransform(VBOUnit& uvbo, TransformFeedbackBufferObject* tfbo)
	{
		tfbo->beginFeedback();
		printOpenGLError ();
		if(_pShader != 0)
		{
			_pShader->bind();
			printOpenGLError();
			GLint location = getAttributLocation(uvbo._attributeName);
			if(location != -1)
			{
				uvbo._pVBO->use();
				glVertexAttribPointer(location, uvbo._numberComponents, GL_FLOAT, GL_FALSE, 0, 0);
				printOpenGLError();
				glEnableVertexAttribArray(location);
			}

			glDrawArrays(uvbo._primitive, 0, uvbo._pVBO->getSize());

			printOpenGLError();
			if(location != -1)
			{
				glDisableVertexAttribArray(location);
				printOpenGLError();
				uvbo._pVBO->unUse();
				printOpenGLError();
			}
			tfbo->endFeedback();
			_pShader->unbind();
		}
	}

	void GLSLGpuProgram::drawComputerShader()
	{
		if(_pShader != 0)
		{
			_pShader->bind();
			glDispatchCompute(512/16, 512/16, 1);
			_pShader->unbind();
		}
	}
	/*
	void GLSLGpuProgram::outputParameters(std::ofstream* outfile)
	{
	(*outfile)<< _name<< std::endl;
	std::map<std::string, float>::iterator vi;
	for ( vi = _parametersMap.begin(); vi != _parametersMap.end(); ++vi )
	{
	std::string name = (*vi).first;
	float value = (*vi).second;
	(*outfile)<< name<<": "<<value<< std::endl;
	}
	(*outfile)<< std::endl<< std::endl;
	}
	*/

}

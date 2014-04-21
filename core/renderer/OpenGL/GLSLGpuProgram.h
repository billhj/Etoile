/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSLGpuProgram.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/GpuProgram.h"
#include "geometry/Texture.h"
#include "ShaderProgram.h"
#include "math/Vectors.h"
#include "math/Matrix.h"
#include "UniformBufferObject.h"
#include "TransformFeedbackBufferObject.h"
#include "VBO.h"
#include "IBO.h"



namespace Etoile
{

	class GLSLGpuProgram : public GpuProgram
	{
	public:
		GLSLGpuProgram(const std::string& name);
		~GLSLGpuProgram();
		virtual void release() override;
		virtual void reCompile() override;
		virtual void use() override;
		virtual void unUse() override;
		void loadShaderFiles(std::map<ShaderType, std::string> files);
		void loadShader(ShaderProgram* shaderprogram);
		GLint getAttributLocation(const std::string& name) const;
		GLint getUniformLocation(const std::string& name) const;


		void setUniformVariable(const std::string& name, int data);
		void setUniformVariable(const std::string& name, float data);
		void setUniformVariable(const std::string& name, bool data);
		void setUniformVariable(const std::string& name, const Vec2f& data);
		void setUniformVariable(const std::string& name, const Vec3f& data);
		void setUniformVariable(const std::string& name, const Vec4f& data);
		void setUniformVariable(const std::string& name, const Matrix4f& data);
		void setUniformVariable(const std::string& name, const std::vector<Vec4f>& data);
		void setUniformVariable(const std::string& name, const std::vector<Matrix4f>& data);

		void setUniformVariable(const std::string& name, Vec4f* data, int size);
		void setUniformVariable(const std::string& name, Matrix4f* data, int size);

		//void setUniformBufferObject(const std::string& name, UniformBufferObject* bufferObject);

		void transformFeedbackVaryingsSetting(size_t count, const char **varyings, GLenum bufferMode = GL_SEPARATE_ATTRIBS);

		void use(std::map<std::string const, Texture*> textureMap);


		virtual void bindTexture(std::string const, Texture*);
		virtual void unBindBindingTextures();

		void drawIBO(GLenum primitive, VBOUnit& pos, VBOUnit& normal,  VBOUnit& texCoord, IBO* index);
		void drawVBO( VBOFloat*, GLenum primitive = GL_TRIANGLES, int numberComponents = 4);
		void drawVBO( VBOUnit&);
		void drawComputerShader();

		void drawFeedBack(VBOFloat*, TransformFeedbackBufferObject* tfbo, int numberComponents = 4);
		void drawFeedBack(VBOUnit&, TransformFeedbackBufferObject* tfbo);

		void drawCaptureTransform(VBOFloat*, TransformFeedbackBufferObject* tfbo, GLenum primitive = GL_TRIANGLES, int numberComponents = 4);
		void drawCaptureTransform(VBOUnit&, TransformFeedbackBufferObject* tfbo);


		ShaderProgram* getShader(){return _pShader;}
		GLuint getShaderID()
		{
			if(_pShader == NULL) return 0;
			return _pShader->getId();
		}

		//void outputParameters(std::ofstream* outfile);
		bool isTessellationGpuProgram(){return _tessellate;}
	private:
		ShaderProgram* _pShader;
		bool _tessellate;
		//std::string _vertex, _fragment, _geometry, _control, _evaluation;
		int _incrementTexture;
	};

}

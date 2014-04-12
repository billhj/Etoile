/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TransformFeedbackBufferObject.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "assert.h"
#include "glhead.h"
#include "VBO.h"

namespace Etoile
{
	class TransformFeedbackBufferObject
	{
	public:
		TransformFeedbackBufferObject();
		~TransformFeedbackBufferObject();
		GLuint getID(){return _feedbackId;}
		void use();
		void unUse();

		void setPrimitiveMode(GLenum primitiveMode = GL_TRIANGLES);
		void setDiscardRasterizer(bool discardRasterizer = true);

		void bindBuffer(int bindingpoint, GLuint bufferId);
		void bindBuffer(int bindingpoint, VBO* vbo);
		void beginFeedback();
		GLuint endFeedback();

		void draw();
		void drawCaptureTransform(VBO*, GLenum primitive = GL_TRIANGLES, int numberComponents = 4);
		void drawFeedBack(VBO*, int numberComponents = 4);
		void checkExtensions();
	private:

		GLuint _query;
		GLuint _feedbackId;
		GLenum _primitiveMode;
		bool _discardRasterizer;

	};


}

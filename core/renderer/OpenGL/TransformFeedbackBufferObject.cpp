/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TransformFeedbackBufferObject.cpp
* @brief 
* @date 1/2/2011
*/

#include "TransformFeedbackBufferObject.h"

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
	void TransformFeedbackBufferObject::checkExtensions()
	{
		if ( glewIsSupported( "GL_VERSION_2_0 GL_NV_gpu_program4 GL_NV_transform_feedback" ) ) {

		}
		GLint iTemp = 0;

		glGetIntegerv( GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, &iTemp );
		glGetIntegerv( GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &iTemp );

		if ( glewIsSupported( "GL_ARB_transform_feedback2" ) ) {
			std::cout<<"GL_ARB_transform_feedback2 is supported"<<std::endl;
		}

		if ( glewIsSupported( "GL_ARB_transform_feedback3" ) ) {
			std::cout<<"GL_ARB_transform_feedback3 is supported"<<std::endl;
		}

		printOpenGLError();

	}

	TransformFeedbackBufferObject::TransformFeedbackBufferObject()
	{
		checkExtensions();
		printOpenGLError();
		glGenTransformFeedbacks(1, &_feedbackId);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _feedbackId);
		glGenQueries(1, &_query);

		setPrimitiveMode();
		setDiscardRasterizer();
	}


	TransformFeedbackBufferObject::~TransformFeedbackBufferObject()
	{
		printOpenGLError();
		glDeleteQueries(1, &_query);
		glDeleteTransformFeedbacks(1, &_feedbackId);
	}


	void TransformFeedbackBufferObject::bindBuffer(int bindingpoint, GLuint bufferId)
	{
		glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, bindingpoint, bufferId);
		printOpenGLError();
	}

	void TransformFeedbackBufferObject::bindBuffer(int bindingpoint, VBO* vbo)
	{
		glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, bindingpoint, vbo->getID());
		printOpenGLError();
	}

	void TransformFeedbackBufferObject::use()
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _feedbackId);
	}

	void TransformFeedbackBufferObject::unUse()
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	}

	void TransformFeedbackBufferObject::beginFeedback()
	{
		use();
		glBeginTransformFeedback(_primitiveMode);
		if(_discardRasterizer)
			glEnable(GL_RASTERIZER_DISCARD);
		glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, _query);
	}

	GLuint TransformFeedbackBufferObject::endFeedback()
	{
		GLuint uiFeedBack = 0;
		//GLint primitiveNum = 0;
		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
		if(_discardRasterizer)
			glDisable(GL_RASTERIZER_DISCARD);
		glGetQueryObjectuiv(_query, GL_QUERY_RESULT, &uiFeedBack );
		//glGetQueryiv(_query, GL_CURRENT_QUERY, &primitiveNum);
		//glGetQueryObjectuiv(_query, GL_QUERY_RESULT, &uiFeedBack );
		glEndTransformFeedback();
		printOpenGLError();
		unUse();
		return uiFeedBack;
	}


	void TransformFeedbackBufferObject::draw()
	{
		glDrawTransformFeedback(_primitiveMode, _feedbackId);
	}

	void TransformFeedbackBufferObject::drawCaptureTransform(VBO* vbo, GLenum primitive, int numberComponents)
	{
		beginFeedback();
		vbo->use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(primitive, 0, vbo->getSize());
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		vbo->unUse();
		endFeedback();
	}

	void TransformFeedbackBufferObject::drawFeedBack(VBO* vbo, int numberComponents)
	{

		vbo->use();
		glVertexPointer(numberComponents, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		draw();
		glDisableClientState(GL_VERTEX_ARRAY);
		printOpenGLError();
		vbo->unUse();
	}


	void TransformFeedbackBufferObject::setPrimitiveMode(GLenum primitiveMode)
	{
		_primitiveMode = primitiveMode;
	}

	void TransformFeedbackBufferObject::setDiscardRasterizer(bool discardRasterizer)
	{
		_discardRasterizer = discardRasterizer;
	}

}

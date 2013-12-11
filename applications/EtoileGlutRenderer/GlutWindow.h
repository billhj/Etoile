/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GlutWindow.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#define USING_GLUT

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include <GL/glut.h>

namespace Etoile
{

	void display(void)
	{

	}

	void reshape(GLint width, GLint height)
	{

		glViewport(0, 0, g_Width, g_Height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(65.0, (float)g_Width / g_Height, g_nearPlane, g_farPlane);
		glMatrixMode(GL_MODELVIEW);
	}


	void MouseButton(int button, int state, int x, int y)
	{
		// Respond to mouse button presses.
		// If button1 pressed, mark this state so we know in motion function.
		if (button == GLUT_LEFT_BUTTON)
		{

		}
	}

	void MouseMotion(int x, int y)
	{
		// If button1 pressed, zoom in/out if mouse is moved up/down.
		if (x)
		{
			glutPostRedisplay();
		}
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27:             // ESCAPE key
			exit (0);
			break;
		case 'l':

			break;
		case 'p':

			break;
		case 't':

			break;
		}
	}

	int initWindow(int w, int h, void(*display)())
	{
		// GLUT Window Initialization:
		//glutInit (&argc, argv);
		glutInitWindowSize (w, h);
		glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow ("Glut Renderer");
		// Initialize OpenGL graphics state
		// Register callbacks:
		glutDisplayFunc (display);
		glutReshapeFunc (reshape);
		glutKeyboardFunc (Keyboard);
		glutMouseFunc (MouseButton);
		glutMotionFunc (MouseMotion);
		//glutIdleFunc (AnimateScene);

		glutAttachMenu (GLUT_RIGHT_BUTTON);

		// Turn the flow of control over to GLUT
		glutMainLoop ();
		return 0;
	}

}


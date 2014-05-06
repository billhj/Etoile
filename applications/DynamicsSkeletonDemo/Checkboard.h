#pragma once
#include "renderer/OpenGL/glhead.h"

class Checkboard 
{
	int displayListId;
	int width;
	int depth;
public:
	Checkboard(int width = 10, int depth= 10): width(width), depth(depth) {create();}
	double centerx() {return width / 2;}
	double centerz() {return depth / 2;}
	void create() {
		static const GLfloat WHITE[] = {1, 1, 1};
		static const GLfloat RED[] = {1, 0, 0};
		static const GLfloat GRAY[] = {.75, .75, .75};
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		glDisable(GL_COLOR_MATERIAL);

		GLfloat lightPosition[] = {4, 3, (depth + 1) / 2 + 1, 1};
		glTranslatef(-(width - 1)/2.f, 0, -(depth-1)/2.f);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
					(x + z) % 2 == 0 ? GRAY : WHITE);
				glVertex3d(x, 0, z);
				glVertex3d(x, 0, z+1);
				glVertex3d(x+1, 0, z+1);
				glVertex3d(x+1, 0, z);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};
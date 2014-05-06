#pragma once
#include "renderer/OpenGL/glhead.h"

class Checkboard 
{
	int displayListId;
	int width;
	int depth;
	int size;
public:
	Checkboard(int width = 50, int depth= 50, int size = 5): width(width), depth(depth), size(size) {}
	double centerx() {return width / 2;}
	double centerz() {return depth / 2;}
	void create() {
		static const GLfloat WHITE[] = {1, 1, 1};
		static const GLfloat RED[] = {1, 0, 0};
		static const GLfloat GRAY[] = {.75, .75, .75};
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		glEnable(GL_COLOR_MATERIAL);
		glTranslatef(-(width - 1) * size/2.f, 0, -(depth-1) * size/2.f);

		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				glColor3fv(
					(x + z) % 2 == 0 ? GRAY : WHITE);
				glVertex3d(x * size, 0, z * size);
				glVertex3d(x * size, 0, (z+1) * size);
				glVertex3d((x+1) * size, 0, (z+1) * size);
				glVertex3d((x+1) * size, 0, z * size);
			}
		}

		glEnd();
		glDisable(GL_COLOR_MATERIAL);
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};
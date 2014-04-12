/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file glDrawFunctions.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "../DrawFunctions.h"
#include "glhead.h"

namespace Etoile
{
	class glDrawFunctions: public DrawFunctions
	{
	public:
		//dont understand why glDisable(GL_LIGHTING);
		void drawBone(Vec3f posParent, Vec3f pos, Vec3f originalDirection, Quaternionf orientation) override
		{
			//bool a = glIsEnabled(GL_LIGHTING);
			glDisable(GL_LIGHTING);
			Vec3f dirOrig = originalDirection.normalized();
			Vec3f localvec(0,1,0);
			Vec3f cross = localvec.cross3(dirOrig);
			//if the direction is inverse, so weird, then i change another axis
			if(cross == Vec3f())
			{
				cross = Vec3f(0,0,1);
			}
			float angle = (float)acos(dirOrig.dot3(localvec));
			Quaternionf newQuat(cross, angle);

			float scaleFactor = (pos - posParent).length();
			float hfactor = 0.1f;
			float wfactor = 0.2f;

			Vec3f p1 = orientation.rotate(newQuat.rotate( Vec3f(wfactor*scaleFactor, hfactor*scaleFactor, 0)));
			Vec3f p2 = orientation.rotate(newQuat.rotate( Vec3f(0,hfactor*scaleFactor, -wfactor*scaleFactor)));
			Vec3f p3 = orientation.rotate(newQuat.rotate( Vec3f(-wfactor*scaleFactor, hfactor*scaleFactor, 0)));
			Vec3f p4 = orientation.rotate(newQuat.rotate( Vec3f(0, hfactor*scaleFactor, wfactor*scaleFactor)));

			p1 += posParent;
			p2 += posParent;
			p3 += posParent;
			p4 += posParent;

			Vec3f n1 = compute_normal(p1,p2,pos);
			Vec3f n2 = compute_normal(p2,p3,pos);
			Vec3f n3 = compute_normal(p3,p4,pos);
			Vec3f n4 = compute_normal(p4,p1,pos);

			Vec3f n5 = compute_normal(p1,posParent,p2);
			Vec3f n6 = compute_normal(p2,posParent,p3);
			Vec3f n7 = compute_normal(p3,posParent,p4);
			Vec3f n8 = compute_normal(p4,posParent,p1);

			glBegin(GL_TRIANGLES);
			//		//float DiffuseMaterial[4] = {0.6, 0.3, 0.3,1};          // define bones color
			glColor4f(0.6, 0.3, 0.3,1);
			//glNormal3f(n1.x(), n1.y(), n1.z());
			glVertex3f(p1.x(), p1.y(), p1.z());
			glVertex3f(p2.x(), p2.y(), p2.z());
			glVertex3f(pos.x(), pos.y(), pos.z());

			glColor4f(0.6, 0.2, 0.3,1);
			//glNormal3f(n2.x(), n2.y(), n2.z());
			glVertex3f(p2.x(), p2.y(), p2.z());
			glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(pos.x(), pos.y(), pos.z());

			glColor4f(0.5, 0.3, 0.3,1);
			//glNormal3f(n3.x(), n3.y(), n3.z());
			glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(p4.x(), p4.y(), p4.z());
			glVertex3f(pos.x(), pos.y(), pos.z());

			glColor4f(0.6, 0.3, 0.4,1);
			//glNormal3f(n4.x(), n4.y(), n4.z());
			glVertex3f(p4.x(), p4.y(), p4.z());
			glVertex3f(p1.x(), p1.y(), p1.z());
			glVertex3f(pos.x(), pos.y(), pos.z());

			glColor4f(0.5, 0.3, 0.4,1);
			//glNormal3f(n5.x(), n5.y(), n5.z());
			glVertex3f(p1.x(), p1.y(), p1.z());
			glVertex3f(posParent.x(), posParent.y(), posParent.z());
			glVertex3f(p2.x(), p2.y(), p2.z());

			glColor4f(0.5, 0.4, 0.4,1);
			//glNormal3f(n6.x(), n6.y(), n6.z());
			glVertex3f(p2.x(), p2.y(), p2.z());
			glVertex3f(posParent.x(), posParent.y(), posParent.z());
			glVertex3f(p3.x(), p3.y(), p3.z());

			glColor4f(0.5, 0.4, 0.3,1);
			//glNormal3f(n7.x(), n7.y(), n7.z());
			glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(posParent.x(), posParent.y(), posParent.z());
			glVertex3f(p4.x(), p4.y(), p4.z());

			glColor4f(0.4, 0.3, 0.4,1);
			//glNormal3f(n8.x(), n8.y(), n8.z());
			glVertex3f(p4.x(), p4.y(), p4.z());
			glVertex3f(posParent.x(), posParent.y(), posParent.z());			
			glVertex3f(p1.x(), p1.y(), p1.z());
			glEnd();
			printOpenGLError();
		}

		void drawBone2(Vec3f posParent, Vec3f pos, Vec3f originalDirection, Quaternionf orientation, float scaleFactor = 1, Vec4f color = Vec4f(0.85, 0.3, 0.3, 0.5)) 
		{
			//bool a = glIsEnabled(GL_LIGHTING);
			glDisable(GL_LIGHTING);
			Vec3f dirOrig = originalDirection.normalized();
			Vec3f localvec(0,1,0);
			Vec3f cross = localvec.cross3(dirOrig);
			//if the direction is inverse, so weird, then i change another axis
			if(cross == Vec3f())
			{
				cross = Vec3f(0,0,1);
			}
			float angle = (float)acos(dirOrig.dot3(localvec));
			Quaternionf newQuat(cross, angle);

			float wfactor = 0.3f;

			Vec3f p1 = orientation.rotate(newQuat.rotate( Vec3f(wfactor*scaleFactor, 0, wfactor*scaleFactor)));
			Vec3f p2 = orientation.rotate(newQuat.rotate( Vec3f(wfactor*scaleFactor,0, -wfactor*scaleFactor)));
			Vec3f p3 = orientation.rotate(newQuat.rotate( Vec3f(-wfactor*scaleFactor, 0, -wfactor*scaleFactor)));
			Vec3f p4 = orientation.rotate(newQuat.rotate( Vec3f(-wfactor*scaleFactor, 0, wfactor*scaleFactor)));

			Vec3f p5 = orientation.rotate(newQuat.rotate( Vec3f(wfactor*scaleFactor, 0, wfactor*scaleFactor)));
			Vec3f p6 = orientation.rotate(newQuat.rotate( Vec3f(wfactor*scaleFactor,0, -wfactor*scaleFactor)));
			Vec3f p7 = orientation.rotate(newQuat.rotate( Vec3f(-wfactor*scaleFactor, 0, -wfactor*scaleFactor)));
			Vec3f p8 = orientation.rotate(newQuat.rotate( Vec3f(-wfactor*scaleFactor, 0, wfactor*scaleFactor)));

			p1 += posParent;
			p2 += posParent;
			p3 += posParent;
			p4 += posParent;
			p5 += pos;
			p6 += pos;
			p7 += pos;
			p8 += pos;

			glColor4f(1 - color.x(), 1 - color.y(), 1 - color.z(), 1);
			glBegin(GL_LINE_LOOP);
			glVertex3f(p4.x(), p4.y(), p4.z());glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(p2.x(), p2.y(), p2.z());glVertex3f(p1.x(), p1.y(), p1.z());
			glVertex3f(p5.x(), p5.y(), p5.z());glVertex3f(p6.x(), p6.y(), p6.z());
			glVertex3f(p7.x(), p7.y(), p7.z());glVertex3f(p8.x(), p8.y(), p8.z());
			glVertex3f(p5.x(), p5.y(), p5.z());glVertex3f(p8.x(), p8.y(), p8.z());
			glVertex3f(p4.x(), p4.y(), p4.z());glVertex3f(p1.x(), p1.y(), p1.z());
			glVertex3f(p8.x(), p8.y(), p8.z());glVertex3f(p7.x(), p7.y(), p7.z());
			glVertex3f(p3.x(), p3.y(), p3.z());glVertex3f(p4.x(), p4.y(), p4.z());
			glVertex3f(p7.x(), p7.y(), p7.z());glVertex3f(p6.x(), p6.y(), p6.z());
			glVertex3f(p2.x(), p2.y(), p2.z());glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(p6.x(), p6.y(), p6.z());glVertex3f(p5.x(), p5.y(), p5.z());
			glVertex3f(p1.x(), p1.y(), p1.z());glVertex3f(p2.x(), p2.y(), p2.z());
			glEnd();

			glBegin(GL_QUADS);
			//		//float DiffuseMaterial[4] = {0.6, 0.3, 0.3,1};          // define bones color
			glColor4f(color.x(), color.y(), color.z(), color.w());
			//glNormal3f(n1.x(), n1.y(), n1.z());
			glVertex3f(p4.x(), p4.y(), p4.z());glVertex3f(p3.x(), p3.y(), p3.z());
			glVertex3f(p2.x(), p2.y(), p2.z());glVertex3f(p1.x(), p1.y(), p1.z());



			glColor4f(color.x() - 0.01, color.y(), color.z(), color.w());
			//glNormal3f(n2.x(), n2.y(), n2.z());
			glVertex3f(p5.x(), p5.y(), p5.z());glVertex3f(p6.x(), p6.y(), p6.z());
			glVertex3f(p7.x(), p7.y(), p7.z());glVertex3f(p8.x(), p8.y(), p8.z());




			glColor4f(color.x(), color.y()-0.01, color.z(), color.w());
			//glNormal3f(n3.x(), n3.y(), n3.z());	
			glVertex3f(p5.x(), p5.y(), p5.z());glVertex3f(p8.x(), p8.y(), p8.z());
			glVertex3f(p4.x(), p4.y(), p4.z());glVertex3f(p1.x(), p1.y(), p1.z());


			glColor4f(color.x(), color.y(), color.z() +0.01, color.w());
			//glNormal3f(n4.x(), n4.y(), n4.z());
			glVertex3f(p8.x(), p8.y(), p8.z());glVertex3f(p7.x(), p7.y(), p7.z());
			glVertex3f(p3.x(), p3.y(), p3.z());glVertex3f(p4.x(), p4.y(), p4.z());



			glColor4f(color.x()-0.01, color.y()+0.01, color.z(), color.w());
			//glNormal3f(n5.x(), n5.y(), n5.z());
			glVertex3f(p7.x(), p7.y(), p7.z());glVertex3f(p6.x(), p6.y(), p6.z());
			glVertex3f(p2.x(), p2.y(), p2.z());glVertex3f(p3.x(), p3.y(), p3.z());



			glColor4f(color.x()+0.01, color.y()-0.01, color.z()-0.01, color.w());
			//glNormal3f(n6.x(), n6.y(), n6.z());
			glVertex3f(p6.x(), p6.y(), p6.z());glVertex3f(p5.x(), p5.y(), p5.z());
			glVertex3f(p1.x(), p1.y(), p1.z());glVertex3f(p2.x(), p2.y(), p2.z());

			glEnd();
			printOpenGLError();
		}


		void drawCurrentAxis(float x, float y, float z, Quaternionf q, float scaleFactor)
		{
			glPushMatrix();
			float length = 1.0 * scaleFactor;
			Vec3f xAxis(1,0,0);
			Vec3f yAxis(0,1,0);
			Vec3f zAxis(0,0,1);
			glLineWidth(3);

			glTranslatef(x,y,z);
			xAxis = q * xAxis;
			yAxis = q * yAxis;
			zAxis = q * zAxis;
			glPushMatrix();
			glColor4f(1,0,0,1);
			glBegin(GL_LINES);
			glVertex3f(xAxis.x() * length , xAxis.y() * length , xAxis.z() * length);
			glVertex3f(0,0,0);
			glEnd();

			glColor4f(0,1,0,1);
			glBegin(GL_LINES);
			glVertex3f(yAxis.x() * length , yAxis.y() * length , yAxis.z() * length);
			glVertex3f(0,0,0);
			glEnd();

			glColor4f(0,0,1,1);
			glBegin(GL_LINES);
			glVertex3f(zAxis.x() * length , zAxis.y() * length ,zAxis.z() * length);
			glVertex3f(0,0,0);
			glEnd();
			glPopMatrix();
			//    drawAxis(10);
			glPopMatrix();
		}

		void drawSphere(float x, float y, float z, float r = 10.0f, int lats = 10, int longs = 10) 
		{
			glPushMatrix();
			glTranslatef(x,y,z);
			drawSphere(r, lats, longs);
			glPopMatrix();
		}

		void drawSphere(float r, int lats, int longs) 
		{
			int i, j;
			for(i = 0; i <= lats; i++) {
				float lat0 = M_PI * (-0.5 + (float) (i - 1) / lats);
				float z0  = sin(lat0);
				float zr0 =  cos(lat0);

				float lat1 = M_PI * (-0.5 + (float) i / lats);
				float z1 = sin(lat1);
				float zr1 = cos(lat1);

				glBegin(GL_QUAD_STRIP);
				for(j = 0; j <= longs; j++) {
					float lng = 2 * M_PI * (float) (j - 1) / longs;
					float x = cos(lng);
					float y = sin(lng);
					glNormal3f(x * zr1, y * zr1, z1);
					glVertex3f(x * zr1, y * zr1, z1);
					glNormal3f(x * zr0, y * zr0, z0);
					glVertex3f(x * zr0, y * zr0, z0);
					
				}
				glEnd();
			}
		}

	};

}



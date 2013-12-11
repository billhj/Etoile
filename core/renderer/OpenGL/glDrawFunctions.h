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
		void drawBone(Vec3f posParent, Vec3f pos, Vec3f originalDirection, Quaternionf orientation) override
		{
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
			float wfactor = 0.1f;

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
			glNormal3d(n1.x(), n1.y(), n1.z());
			glVertex3d(p1.x(), p1.y(), p1.z());
			glVertex3d(p2.x(), p2.y(), p2.z());
			glVertex3d(pos.x(), pos.y(), pos.z());

			glNormal3d(n2.x(), n2.y(), n2.z());
			glVertex3d(p2.x(), p2.y(), p2.z());
			glVertex3d(p3.x(), p3.y(), p3.z());
			glVertex3d(pos.x(), pos.y(), pos.z());

			glNormal3d(n3.x(), n3.y(), n3.z());
			glVertex3d(p3.x(), p3.y(), p3.z());
			glVertex3d(p4.x(), p4.y(), p4.z());
			glVertex3d(pos.x(), pos.y(), pos.z());

			glNormal3d(n4.x(), n4.y(), n4.z());
			glVertex3d(p4.x(), p4.y(), p4.z());
			glVertex3d(p1.x(), p1.y(), p1.z());
			glVertex3d(pos.x(), pos.y(), pos.z());


			glNormal3d(n5.x(), n5.y(), n5.z());
			glVertex3d(p1.x(), p1.y(), p1.z());
			glVertex3d(posParent.x(), posParent.y(), posParent.z());
			glVertex3d(p2.x(), p2.y(), p2.z());

			glNormal3d(n6.x(), n6.y(), n6.z());
			glVertex3d(p2.x(), p2.y(), p2.z());
			glVertex3d(posParent.x(), posParent.y(), posParent.z());
			glVertex3d(p3.x(), p3.y(), p3.z());

			glNormal3d(n7.x(), n7.y(), n7.z());
			glVertex3d(p3.x(), p3.y(), p3.z());
			glVertex3d(posParent.x(), posParent.y(), posParent.z());
			glVertex3d(p4.x(), p4.y(), p4.z());

			glNormal3d(n8.x(), n8.y(), n8.z());
			glVertex3d(p4.x(), p4.y(), p4.z());
			glVertex3d(posParent.x(), posParent.y(), posParent.z());			
			glVertex3d(p1.x(), p1.y(), p1.z());
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

	};

}



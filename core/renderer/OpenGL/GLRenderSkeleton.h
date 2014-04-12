/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLRenderSkeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "glDrawFunctions.h"
#include "geometry/skeleton.h"
#include "geometry/AxisAlignedBoundingBox.h"


namespace Etoile
{
	class GLRenderSkeleton : public Skeleton
	{
	protected:
		glDrawFunctions _glDrawFunctions;
		bool _drawAABBs;
	public:
		GLRenderSkeleton(const std::string& name) : Skeleton(name), _drawAABBs(true)
		{
			
		}

		void setAABBenable(bool b)
		{
			_drawAABBs = b;
		}
		virtual void draw(Matrix4f& gltransformation) 
		{
			//drawAABB(gltransformation);
			drawSkeleton(gltransformation);
		}

		virtual void drawSkeleton(Matrix4f& gltransformation)
		{
			//dont understand these parameters
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glPushMatrix();
			//bool a = glIsEnabled(GL_LIGHTING);
			glMultMatrixf(&gltransformation[0][0]);
			float scale = 1;
			if(_joints.size() > 1)
			{
				scale = _joints[_joints.size()-1]->getLocalPosition().length();
			}
			for(unsigned int i = 0; i <_joints.size(); ++i)
			{
				Joint* joint = _joints[i];
				if(joint->getParent() != NULL)
				{
					Joint* jp = joint->getParent();
					Vec3f posP = jp->getWorldPosition();
					Vec3f pos = joint->getWorldPosition();
			//		// define bones color
					/*glColor4f(0.5, 0.0, 0.0,1);
					glPointSize(10);
					glBegin(GL_POINTS);
					glVertex3f(posP.x(), posP.y(), posP.z());
					glEnd();*/
					
					glColor4f(0.8, 0.5, 0.5,1);
					glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					_glDrawFunctions.drawBone2(posP, pos, joint->getLocalPosition(), jp->getWorldRotation(), 0.4 * scale, Vec4f(0.3, 0.8,0.6, 0.6));
					glDisable (GL_BLEND); 
					glColor4f(0.5, 0.0, 0.0, 1);
					_glDrawFunctions.drawSphere(posP.x(), posP.y(), posP.z(), 0.02 * scale);
					drawCurrentAxis(posP, jp->getWorldRotation(), 0.05 * scale);
				}
			}
			glPopMatrix();	
			drawCurrentAxis(Vec3f(), Quaternionf(), 0.002);
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
		}

		void drawAABB(AxisAlignedBoundingBoxf* aabb, Matrix4f& glworld)
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			Vec4f minV = Vec4f(aabb->minimum().x(), aabb->minimum().y(), aabb->minimum().z(), 1);
			Vec4f maxV = Vec4f(aabb->maximum().x(), aabb->maximum().y(), aabb->maximum().z(), 1);
			Vec3f min = Vec3f(minV.x(), minV.y(), minV.z());
			Vec3f max = Vec3f(maxV.x(), maxV.y(), maxV.z());
			glLineWidth(2.5); 
			glColor4f(1, 1, 1, 1);
			glPushMatrix();
			glMultMatrixf(&(glworld[0][0]));
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),min.z());
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),min.z());
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(min.x(),min.y(),min.z());
			glVertex3f(min.x(),max.y(),min.z());

			glVertex3f(min.x(),min.y(),max.z());
			glVertex3f(min.x(),max.y(),max.z());

			glVertex3f(max.x(),min.y(),max.z());
			glVertex3f(max.x(),max.y(),max.z());

			glVertex3f(max.x(),min.y(),min.z());
			glVertex3f(max.x(),max.y(),min.z());
			glEnd();
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}

		void drawAABB(Matrix4f& gltransformation)
		{
			if(_drawAABBs)
			{
				//drawAABB(getAABB(), gltransformation);
				/*glBegin(GL_LINE_STRIP);
				glVertex3f(0,0,0);
				glVertex3f(1,1,1);
				glVertex3f(2,2,2);
				glVertex3f(2,1,3);
				glVertex3f(0,0,0);
				glEnd();*/
			}
		}


		void drawCurrentAxis(Vec3f pos, Quaternionf q, float scaleFactor)
		{
			glDisable(GL_LIGHTING);
			glPushMatrix();
			double length = 5.0 * scaleFactor;
			Vec3f xAxis(1,0,0);
			Vec3f yAxis(0,1,0);
			Vec3f zAxis(0,0,1);
			glLineWidth(5);
			Vec3f angle = q.getEulerAngleXYZBYAngle();
			glTranslatef(pos.x(),pos.y(),pos.z());
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

			/*glRotatef(angle.z(), 0, 0, 1); 
			glRotatef(angle.y(), 0, 1, 0);
			glRotatef(angle.x(), 1, 0, 0);
			drawAxis(length);*/
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}

		virtual void perform(Matrix4f& gltransformation) override
		{
			draw(gltransformation);
		}


		//void applyMaterial(Material* material)
		//{
		//	if(material == NULL) return;
		//	Vec4f ambient = material->getAmbientFinal();
		//	glMaterialfv(GL_FRONT, GL_AMBIENT, &Vec4f((float)ambient[0], (float)ambient[1], (float)ambient[2], (float)ambient[3])[0]);
		//	Vec4f diffuse = material->getDiffuseFinal();
		//	glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], (float)diffuse[3])[0]);
		//	Vec4f specular = material->getSpecularFinal();
		//	glMaterialfv(GL_FRONT, GL_SPECULAR, &Vec4f((float)specular[0], (float)specular[1], (float)specular[2], (float)specular[3])[0]);
		//	glMateriali(GL_FRONT, GL_SHININESS, material->getSpecularShininess());
		//	float tranparencyValue = material->getDissolvedOpacity();
		//	//make sure need to use  applying glEnable(GL_POLYGON_STIPPLE);
		//	glEnable(GL_POLYGON_STIPPLE);
		//	glPolygonStipple(__stippleMask[int(tranparencyValue * __screenDoorMaskRange)]);
		//}


	};

}

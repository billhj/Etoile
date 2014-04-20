/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLRenderMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/RenderMesh.h"
#include "glhead.h"

namespace Etoile
{
	class GLRenderMesh : public RenderMesh
	{
	protected:
		bool _drawAABBs;
	public:
		GLRenderMesh(const std::string& name) : RenderMesh(name), _drawAABBs(true)
		{
		}

		void setAABBenable(bool b)
		{
			_drawAABBs = b;
		}
		virtual void draw(Matrix4f& gltransformation) override
		{
			drawAABB(gltransformation);
			drawMesh(gltransformation);
		}

		virtual void drawMesh(Matrix4f& gltransformation)
		{
			const std::vector<SubMesh*>& submeshlist = this->getSubMeshList();

			for(unsigned int i = 0; i < submeshlist.size(); ++i)
			{
				SubMesh* submesh = submeshlist[i];
				drawSubMesh(submesh, gltransformation);
			}	
		}

		virtual void drawSubMesh(SubMesh* submesh, Matrix4f& gltransformation)
		{
			const std::vector<Vec3f>& vertices = submesh->getSkin()._vdata;
			const std::vector<Vec3f>& normals = submesh->getSkin()._ndata;
			const std::vector<Vec2f>& texs = submesh->getSkin()._tdata;
			const std::vector<unsigned int>& faceIndices = submesh->getOriginalVertexIndexForFaces();

			Material* material = submesh->getMaterial();
			if(material != NULL)
			{
				applyMaterial(material);
				glPushMatrix();
				glLoadMatrixf(&gltransformation[0][0]);
				Texture* t = material->getDiffuseTexture();
				if(t != NULL)
				{
					t->use();
				}

				glBegin(GL_TRIANGLES);
				for(unsigned int i = 0; i < faceIndices.size(); ++i)
				{
					glTexCoord2fv( &texs[faceIndices[i]][0] );
					glNormal3fv( &normals[faceIndices[i]][0] );
					glVertex3fv( &vertices[faceIndices[i]][0] );
				}
				glEnd();

				if(t != NULL)
				{
					t->unUse();
				}
				glPopMatrix();
			}
			else
			{
				assert(0 && "GLRenderMesh: no material");
			}
		}

		

		void drawAABB(AxisAlignedBoundingBoxf* aabb, Matrix4f& glworld)
		{
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_LIGHTING);
			Vec4f minV = Vec4f(aabb->minimum().x(), aabb->minimum().y(), aabb->minimum().z(), 1);
			Vec4f maxV = Vec4f(aabb->maximum().x(), aabb->maximum().y(), aabb->maximum().z(), 1);
			Vec3f min = Vec3f(minV.x(), minV.y(), minV.z());
			Vec3f max = Vec3f(maxV.x(), maxV.y(), maxV.z());
			glLineWidth(2.5); 
			
			glPushMatrix();
			glMultMatrixf(&(glworld[0][0]));

			glColor4f(1, 1, 1, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),min.z());
			glEnd();

			//glColor4f(0, 0, 1, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),min.z());
			glEnd();

			//glColor4f(0, 1, 0, 1);
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
				drawAABB(getAABB(), gltransformation);
			}
		}

		void applyMaterial(Material* material)
		{
			if(material == NULL) return;
			//glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);
			Vec4f ambient = material->getAmbientFinal();
			glMaterialfv(GL_FRONT, GL_AMBIENT, &Vec4f((float)ambient[0], (float)ambient[1], (float)ambient[2], (float)ambient[3])[0]);
			Vec4f diffuse = material->getDiffuseFinal();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], (float)diffuse[3])[0]);
			Vec4f specular = material->getSpecularFinal();
			glMaterialfv(GL_FRONT, GL_SPECULAR, &Vec4f((float)specular[0], (float)specular[1], (float)specular[2], (float)specular[3])[0]);
			glMateriali(GL_FRONT, GL_SHININESS, material->getSpecularShininess());
			float tranparencyValue = material->getDissolvedOpacity();
			//make sure need to use  applying glEnable(GL_POLYGON_STIPPLE);
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(__stippleMask[int(tranparencyValue * __screenDoorMaskRange)]);
		}


	};

}

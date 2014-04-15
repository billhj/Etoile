/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLMeshRenderUnit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/MeshRenderUnit.h"
#include "glhead.h"

namespace Etoile
{
	class GLMeshRenderUnit : public MeshRenderUnit
	{
	protected:
		bool _drawAABBs;
	public:
		GLMeshRenderUnit(const std::string& name) : MeshRenderUnit(name), _drawAABBs(true)
		{
		}

		void setAABBenable(bool b)
		{
			_drawAABBs = b;
		}

		virtual void drawMesh()
		{
			if(_pMesh == NULL) return;
			
			const std::vector<SubMesh*>& submeshlist = _pMesh->getSubMeshList();

			for(unsigned int i = 0; i < submeshlist.size(); ++i)
			{
				SubMesh* submesh = submeshlist[i];
				drawSubMesh(submesh);
			}
			
			drawAABB();
		}

		virtual void drawSubMesh(SubMesh* submesh)
		{
			const std::vector<Vec3f>& vertices = submesh->getSkin()._vdata;
			const std::vector<Vec3f>& normals = submesh->getSkin()._ndata;
			const std::vector<Vec2f>& texs = submesh->getSkin()._tdata;
			const std::vector<int>& faceIndices = submesh->getOriginalVertexIndexForFaces();

			Material* material = submesh->getMaterial();
			if(material != NULL)
			{
				applyMaterial(material);
				Matrix4f modelM = submesh->getGLModelMatrix() * _pMesh->getGLModelMatrix();
				glPushMatrix();
				glLoadMatrixf(&modelM[0][0]);
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
				assert(0 && "GLMeshRenderUnit: no material");
			}
		}

		

		void drawAABB(AxisAlignedBoundingBoxf* aabb, Matrix4f world)
		{
			Vec4f minV = Vec4f(aabb->minimum().x(), aabb->minimum().y(), aabb->minimum().z(), 1);
			Vec4f maxV = Vec4f(aabb->maximum().x(), aabb->maximum().y(), aabb->maximum().z(), 1);
			Vec3f min = Vec3f(minV.x(), minV.y(), minV.z());
			Vec3f max = Vec3f(maxV.x(), maxV.y(), maxV.z());
			glLineWidth(2.5); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f(0, 0, 1, 1)[0]);
			glPushMatrix();
			world.transpose();
			glMultMatrixf(&(world[0][0]));
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
		}

		void drawAABB()
		{
			if(_pMesh == NULL) return;
			const std::vector<SubMesh*>& submeshlist = _pMesh->getSubMeshList();
			if(_drawAABBs)
			{
				for(unsigned int i = 0; i < submeshlist.size(); ++i)
				{
					SubMesh* submesh = submeshlist[i];
					Matrix4f& model = submesh->getModelMatrix();
					drawAABB(submesh->getAABB(), _pMesh->getModelMatrix() * model);
				}
			}
		}

		void applyMaterial(Material* material)
		{
			if(material == NULL) return;
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

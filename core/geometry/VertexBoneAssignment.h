/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VertexBoneAssignment.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "Math/Vectors.h"

namespace Etoile
{
	struct BoneAssignment
	{
		int _boneIdx;
		float _boneweight;
	};

	class VertexBoneAssignment
	{
	public:
		VertexBoneAssignment(int idx): _idxVertex(idx){}
		void setPosition(Vec3f pos){
			_originalPos = pos;
		}
		void setNormal(Vec3f normal){
			_originalNormal = normal;
		}
		Vec3f getOriginalPosition(){return _originalPos;}
		Vec3f getOriginalNormal(){return _originalNormal;}
		VertexBoneAssignment(){}
		void setIdx(int idx){_idxVertex = idx;}
		int getIdx(){return _idxVertex;}
		void addBoneWeight(int idx, float weight)
		{
			BoneAssignment assign;
			assign._boneIdx = idx;
			assign._boneweight = weight;
			_boneAssignments.push_back(assign);
		}
		std::vector<BoneAssignment>& getBoneAssignments(){return _boneAssignments;}

	private:
		int _idxVertex;
		Vec3f _originalPos;
		Vec3f _originalNormal;
		std::vector<BoneAssignment> _boneAssignments;

	};

}

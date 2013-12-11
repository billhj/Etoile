/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MotionNode.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "../../algo/sort/Graph.h"

namespace Etoile
{

	class MotionNode;
	typedef std::vector<MotionNode*> MotionNodes;

	class MotionNode : public GraphNode
	{
	public:
		MotionNode(const std::string& name, unsigned int clipId, unsigned int frameId);
		~MotionNode();
		unsigned int getClipId(){return _clipId;}
		void setClip(unsigned int clipId){ _clipId = clipId;}
		unsigned int getFrameId(){return _frameId;}
		void setFrameId(unsigned int frameId){ _frameId = frameId;}
	
	private:
		unsigned int _clipId;
		unsigned int _frameId;
	};





}

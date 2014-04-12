/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include "module/flowgraph/SocketNode.h"
#include "RenderTarget.h"
#include "RenderUnit.h"

namespace Etoile
{
	class RenderPass : public SocketNode
	{
	public:
		RenderPass(const std::string& name)
		{
			SocketNode::getType()._name = name;
		}
		virtual ~RenderPass()
		{
			clear();
		}
		void clearAll()
		{
			SocketNode::clear();
			for(unsigned int i = 0; i < _renderUnits.size(); ++i)
			{
				delete _renderUnits[i];
			}
			_renderUnits.clear();
		}

		virtual void draw() 
		{
			useRenderTargets();
			for(unsigned int i = 0; i < _renderUnits.size(); ++i)
			{
				_renderUnits[i]->draw();
			}
			unUseRenderTargets();
		}

		virtual void addRenderUnit(RenderUnit* ru)
		{
			_renderUnits.push_back(ru);
		}

		virtual void setRenderUnit(unsigned int idx, RenderUnit* ru)
		{
			_renderUnits[idx] = ru;
		}

		RenderUnit* getRenderUnitByIndex(unsigned int idx)
		{
			if(idx < _renderUnits.size())
			{
				return _renderUnits[idx];
			}
			return NULL;
		}

		std::vector<RenderUnit*>& getRenderUnits(){return _renderUnits;}
		const std::string getName(){return _type._name;}
		void setName(const std::string& name){ _type._name = name;}
		virtual void useRenderTargets() = 0;
		virtual void unUseRenderTargets() = 0;
		
		/*virtual void excute() override
		{
			draw();
		}*/
	protected:
		std::vector<RenderUnit*> _renderUnits;
	};
}

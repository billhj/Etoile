#include "QglviewerPlugin.h"
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QglviewerPlugin.cpp
* @brief 
* @date 1/2/2011
*/

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::QglviewerPluginDescription();
}

Etoile::EPlugin* loadQglviewerRendererPlugin()
{
	return new Etoile::QglviewerPlugin("QglviewerRenderer");
}

namespace Etoile
{
	QglTextureInputSocket::QglTextureInputSocket(const std::string& name):TextureInputSocket(name)
	{
	}

	void QglTextureInputSocket::perform(Texture* signal)
	{
		if(signal == NULL) return;
		QglviewerPlugin* plugin = (QglviewerPlugin*)(this->getNode());
		plugin->setupRenderOrder();
		plugin->getRenderer()->setOutputTexture(signal);
		plugin->getRenderer()->setVisible(true);
	}

	void QglTextureInputSocket::retrieve(Texture* signal)
	{
		if(signal == NULL) return;
		QglviewerPlugin* plugin = (QglviewerPlugin*)(this->getNode());
		if(plugin->getRenderer()->getOutputTexture() == signal)
		{
			plugin->getRenderer()->setOutputTexture(NULL);
			plugin->getRenderer()->getRenderPasses().clear();
		}
		plugin->setupRenderOrder();
	}

	QglviewerPlugin::QglviewerPlugin(const std::string& name):EPlugin(),SocketNode()
	{
		this->getType()._description = "QglviewerRenderer";
		this->getType()._name = name;
		this->getType()._w = 80;
		this->getType()._h = 100;
		this->getType()._color._r = 130;
		this->getType()._color._g = 250;
		this->getType()._color._b = 240;
		this->getType()._color._a = 255;
		_pRenderer = new QglviewerRenderer(this);
		_pRenderer->setVisible(false);
		_socket = new QglTextureInputSocket("OutputTexture");
		this->addInputSocket(_socket);
		_pviewsocket = new QglViewMatrix4fOutputSocket("ViewMatrix");
		_pprojsocket = new QglProjectionMatrix4fOutputSocket("ProjectionMatrix");
		_pviewprojsocket = new QglViewProjectionMatrix4fOutputSocket("ViewProjectionMatrix");
		this->addOutputSocket(_pviewsocket);
		this->addOutputSocket(_pprojsocket);
		this->addOutputSocket(_pviewprojsocket);
		_pRenderer->setVisible(true);

		_pCamera = new CameraWidget("Camera", _pRenderer);
	}

	QglviewerPlugin::~QglviewerPlugin()
	{
		release();
	}
	void QglviewerPlugin::init()
	{
		
	}

	void QglviewerPlugin::apply()
	{
		//setupRenderOrder();
		_pCamera->setVisible(true);
		_pRenderer->setVisible(true);
		_pRenderer->raise();
		_pCamera->raise();
		((GraphScene*)(this->getGraph()))->addGraphSceneSignalPerformer(this);
	}

	void QglviewerPlugin::release()
	{
		_pRenderer->close();
	}

	void QglviewerPlugin::setupRenderOrder()
	{
		if(_pGraph == NULL) return;
		std::vector<BasicNode*> nodes;
		BasicNode* current = this;
		sort(current, nodes);
		std::vector<RenderPass*> passes;
		for(unsigned int i = 0; i < nodes.size(); ++i)
		{
			RenderPass* p = dynamic_cast<RenderPass*>(nodes[nodes.size() - 1 - i]);
			if(p != NULL)
			{
				passes.push_back(p);
			}
		}
		_pRenderer->setRenderPasses(passes);
	}

	void QglviewerPlugin::sort(BasicNode* current, std::vector<BasicNode*>& nodes)
	{
		nodes.push_back(current);
		std::vector<BasicNode*> next;
		for(unsigned int i = 0; i < _pGraph->getEdges().size(); ++i)
		{
			BasicEdge* edge = _pGraph->getEdges()[i];
			if(edge->getTail() == current)
			{
				BasicNode* head = edge->getHead();
				RenderPass* p = dynamic_cast<RenderPass*>(head);
				if(p != NULL)
				{
					next.push_back(head);
				}
			}
		}

		for(unsigned int i = 0; i < next.size(); ++i)
		{
			BasicNode* node = next[i];
			bool travel = true;
			for(unsigned int j = 0; j < nodes.size(); ++j)
			{
				BasicEdge* edge = _pGraph->getEdge(node, nodes[j]);
				if(nodes[j] != current && edge != NULL)
				{
					RenderPass* p = dynamic_cast<RenderPass*>(nodes[j]);
					if(p != NULL)
					{
						travel = false;
						j = nodes.size();
					}
				}
			}
			if(travel)
			{
				sort(node, nodes);
			}
		}

	}


	void QglviewerPlugin::updateModelViewMatrix(Matrix4f* view, Matrix4f* project)
	{
		_pviewsocket->signalEmit(view);
		_pprojsocket->signalEmit(project);
		_pviewprojsocket->signalEmit(&((*project) * (*view)));
	}


	void QglviewerPlugin::perform(GraphScene* signal) 
	{
		setupRenderOrder();
	}

}

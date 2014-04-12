#include "ODEDynamicsPlugin.h"

/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ODEDynamicsPlugin.cpp
* @brief 
* @date 1/2/2011
*/

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::ODEDynamicsPluginDescription();
}

Etoile::EPlugin* loadODEDynamicsPlugin()
{
	return new Etoile::ODEDynamicsPlugin("OpenDynamics");
}

namespace Etoile
{

	ODESceneInputSocket::ODESceneInputSocket(const std::string& name): SceneInputSocket(name)
	{
	}

	void ODESceneInputSocket::perform(Scene* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
		plugin->setScene(signal);
		/*ODEPhysicsWorld * world = plugin->getPhysicsWorld();

		{
			std::string name = "" + rand();
			Mesh* mesh = (Mesh*)(signal);
			if(mesh != NULL)
			{
				name = mesh->getName();
			}
			ODEPhysicsObject* obj = new ODEPhysicsObject(name, signal);
			obj->init(world->getWorldID(), world->getSpaceID());
			obj->initBox();
			obj->updatePhysicsObject();
			world->addPhysicsObject(obj);
		}*/
	}

	void ODESceneInputSocket::retrieve(Scene* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
		plugin->setScene(NULL);
		/*ODEPhysicsWorld * world = plugin->getPhysicsWorld();
		std::map<std::string, ODEPhysicsObject*>::iterator itor = world->getPhysicsObjects().begin();
		for(; itor != world->getPhysicsObjects().end(); ++itor)
		{
			if(signal == itor->second->getSceneNode())
			{
				world->getPhysicsObjects().erase(itor);
				return;
			}
		}*/
	}

	/*ODEPlaneInputSocket::ODEPlaneInputSocket(const std::string& name): PlaneInputSocket(name)
	{
	}

	void ODEPlaneInputSocket::perform(Plane* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
		ODEPhysicsWorld * world = plugin->getPhysicsWorld();
		Matrix4f model = signal->getModelMatrix();
		Vec4f abcw = model * Vec4f(0,1,0,0);
		world->createPlane(abcw.x(), abcw.y(), abcw.z(), abcw.w());
	}

	void ODEPlaneInputSocket::retrieve(Plane* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
	}*/

	ODEDynamicsPlugin::ODEDynamicsPlugin(const std::string& name): EPlugin(), SocketNode(), _pScene(NULL)
	{
		this->getType()._description = "OpenDynamics";
		this->getType()._name = name;
		_pSceneInput = new ODESceneInputSocket();
		this->addInputSocket(_pSceneInput);

		world.init();
		world.createGravity(0,-0.001,0);

		_panel.setODEPhysicsWorld(&world);
		_panel.setVisible(false);
		/*dBody* body = new dBody();
		body->create(world.getWorldID());
		body->setPosition(10, 10 , 0);

		dMass* m = new dMass();
		m->setBox (1, 10, 10, 10);
		body->setMass(m);

		dBox* box = new dBox();
		box->create (world.getSpaceID(),10,10,10);
		box->setBody(*body);

		ODEPhysicsObject* obj = new ODEPhysicsObject("first", NULL);
		obj->setBody(body);
		obj->setMass(m);
		obj->setGeometry(box);
		obj->init(world.getWorldID(), world.getSpaceID());
		world.addPhysicsObject(obj);*/
	}

	ODEDynamicsPlugin::~ODEDynamicsPlugin()
	{

	}
	void ODEDynamicsPlugin::init()
	{

	}

	void ODEDynamicsPlugin::apply()
	{
		_panel.setVisible(true);
		_panel.raise();
	}

	void ODEDynamicsPlugin::release()
	{
	
	}

	void ODEDynamicsPlugin::setScene(Scene* scene)
	{
		_pScene = scene;
	}
}
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

	ODESceneObjectInputSocket::ODESceneObjectInputSocket(const std::string& name): SceneObjectInputSocket(name)
	{
	}

	void ODESceneObjectInputSocket::perform(SceneObject* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
		ODEPhysicsWorld * world = plugin->getPhysicsWorld();

		{
			std::string name = "" + rand();
			/*Mesh* mesh = dynamic_cast<Mesh*>(signal);
			if(mesh != NULL)
			{
				name = mesh->getName();
			}*/
			ODEPhysicsObject* obj = new ODEPhysicsObject(name, signal);
			obj->init(world->getWorldID(), world->getSpaceID());
			obj->initBox();
			obj->updatePhysicsObject();
			world->addPhysicsObject(obj);
		}
	}

	void ODESceneObjectInputSocket::retrieve(SceneObject* signal) 
	{
		if(signal == NULL) return;
		ODEDynamicsPlugin* plugin = (ODEDynamicsPlugin*)(this->getNode());
		ODEPhysicsWorld * world = plugin->getPhysicsWorld();
		std::map<std::string, ODEPhysicsObject*>::iterator itor = world->getPhysicsObjects().begin();
		for(; itor != world->getPhysicsObjects().end(); ++itor)
		{
			if(signal == itor->second->getSceneObject())
			{
				world->getPhysicsObjects().erase(itor);
			}
		}
	}

	ODEPlaneInputSocket::ODEPlaneInputSocket(const std::string& name): PlaneInputSocket(name)
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
	}

	ODEDynamicsPlugin::ODEDynamicsPlugin(const std::string& name): EPlugin(), SocketNode()
	{
		this->getType()._description = "OpenDynamics";
		this->getType()._name = name;
		_pSceneobjectInput = new ODESceneObjectInputSocket();
		this->addInputSocket(_pSceneobjectInput);
		_pPlaneInput = new ODEPlaneInputSocket();
		this->addInputSocket(_pPlaneInput);
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
}
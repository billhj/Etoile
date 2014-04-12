#include "RigidDynamicsBodyPlugin.h"
#include "geometry/SceneEntity.h"
#include "renderer/OpenGL/GLRenderSkeleton.h"
#include "RigidBodySkeleton.h"

/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RigidDynamicsBodyPlugin.cpp
* @brief 
* @date 1/2/2011
*/

void loadDescription(Etoile::EPluginsDescription** descrip)
{
	descrip[0] = new Etoile::RigidDynamicsBodyPluginDescription();
}

Etoile::EPlugin* loadRigidDynamicsBodyPlugin()
{
	return new Etoile::RigidDynamicsBodyPlugin("RigidDynamicsBody");
}

namespace Etoile
{

	RDBSceneInputSocket::RDBSceneInputSocket(const std::string& name): SceneInputSocket(name)
	{
	}

	void RDBSceneInputSocket::perform(Scene* signal) 
	{
		if(signal == NULL) return;
		RigidDynamicsBodyPlugin* plugin = (RigidDynamicsBodyPlugin*)(this->getNode());
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

	void RDBSceneInputSocket::retrieve(Scene* signal) 
	{
		if(signal == NULL) return;
		RigidDynamicsBodyPlugin* plugin = (RigidDynamicsBodyPlugin*)(this->getNode());
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
		RigidDynamicsBodyPlugin* plugin = (RigidDynamicsBodyPlugin*)(this->getNode());
		ODEPhysicsWorld * world = plugin->getPhysicsWorld();
		Matrix4f model = signal->getModelMatrix();
		Vec4f abcw = model * Vec4f(0,1,0,0);
		world->createPlane(abcw.x(), abcw.y(), abcw.z(), abcw.w());
	}

	void ODEPlaneInputSocket::retrieve(Plane* signal) 
	{
		if(signal == NULL) return;
		RigidDynamicsBodyPlugin* plugin = (RigidDynamicsBodyPlugin*)(this->getNode());
	}*/

	RigidDynamicsBodyPlugin::RigidDynamicsBodyPlugin(const std::string& name): EPlugin(), SocketNode(), _pScene(NULL)
	{
		this->getType()._description = "RigidDynamicsBody";
		this->getType()._name = name;
		_pSceneInput = new RDBSceneInputSocket();
		this->addInputSocket(_pSceneInput);

	
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

	RigidDynamicsBodyPlugin::~RigidDynamicsBodyPlugin()
	{

	}
	void RigidDynamicsBodyPlugin::init()
	{
		buildSkeleton();
	}
GLRenderSkeleton* sk;
RigidBodySkeleton *rsk;
QTimer timer;
	RigidBodyDynamics::Model* model;
	RigidBodyDynamics::Math::Vector3d r;
	RigidBodyDynamics::Math::Vector3d v;
	void RigidDynamicsBodyPlugin::apply()
	{
		connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
		timer.start(10);
	}

	void RigidDynamicsBodyPlugin::update()
	{	
		//static float now = time(0);
		
		/*QTime myTimer;
		myTimer.start();*/
		rsk->perform(0.01);
		/*rsk->perform(0.01);		rsk->perform(0.01);
		int nMilliseconds = myTimer.elapsed();
		std::cout<<" time :          "<<nMilliseconds<<std::endl;
*/
		//_panel.setVisible(true);
		//_panel.raise();
		/*RigidBodyDynamics::Math::VectorNd acce(3);
		RigidBodyDynamics::ForwardDynamics(*model, r, v, RigidBodyDynamics::Math::Vector3d(0,0,0), acce, NULL);
		v += acce * 0.05;
		r += v * 0.05;
		std::cout<<"           "<<acce<<std::endl;*/

	/*	printf("%.3f\n", time(0) - now);

		now = time(0);*/
	}

	void RigidDynamicsBodyPlugin::release()
	{
		rsk->addTorque(1, 5);
	}
	
	void RigidDynamicsBodyPlugin::buildSkeleton()
	{
		r = RigidBodyDynamics::Math::Vector3d(0,0,0);
		v = RigidBodyDynamics::Math::Vector3d(0,0,0);
		if(_pScene != NULL)
		{
			Etoile::SceneNode* sn = new Etoile::SceneNode();
			_pScene->addSceneNode(sn);
			Etoile::SceneEntity* entity = new Etoile::SceneEntity();
			sn->attachMovableObject(entity);
			sk = new GLRenderSkeleton("rigid dynamics body");  
			
			entity->addSkeleton(sk);

			Etoile::Joint* j = new Etoile::Joint("0");
			j->setLocalPosition(Etoile::Vec3f(0.0,0,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("1");
			j->setLocalPosition(Etoile::Vec3f(5,0,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("2");
			j->setLocalPosition(Etoile::Vec3f(5,0,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("3");
			j->setLocalPosition(Etoile::Vec3f(5,0,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("4");
			j->setLocalPosition(Etoile::Vec3f(2,2,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			/*j = new Etoile::Joint("4");
			j->setLocalPosition(Etoile::Vec3f(0,5,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("5");
			j->setLocalPosition(Etoile::Vec3f(0,5,0));
			sk->addJoint(j, sk->getJoints().size() - 1);
			j = new Etoile::Joint("6");
			j->setLocalPosition(Etoile::Vec3f(0,5,0));
			sk->addJoint(j, sk->getJoints().size() - 1);*/
			sk->update();
			rsk = new RigidBodySkeleton("rigid dynamics body");
			rsk->setSkeleton(sk);

			/*model = new RigidBodyDynamics::Model();
			model->gravity = RigidBodyDynamics::Math::Vector3d (0., -9.81, 0.);
			RigidBodyDynamics::Body* body_a = new RigidBodyDynamics::Body (1., RigidBodyDynamics::Math::Vector3d (0.5, 0, 0), RigidBodyDynamics::Math::Vector3d (1., 1., 1.));
			RigidBodyDynamics::Joint* joint_a = new RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d (0., 0., 1.));
			int body_a_id = model->AddBody(0, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(0.0, 0, 0.)), *joint_a, *body_a);

			RigidBodyDynamics::Body* body_b = new RigidBodyDynamics::Body (1, RigidBodyDynamics::Math::Vector3d (0.0, 0.5, 0), RigidBodyDynamics::Math::Vector3d (1., 1., 1.));
			RigidBodyDynamics::Joint* joint_b = new RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d (0., 0., 1.));
			int body_b_id = model->AddBody(body_a_id, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(1, 0, 0.)), *joint_b, *body_b);

			RigidBodyDynamics::Body* body_c = new RigidBodyDynamics::Body (1, RigidBodyDynamics::Math::Vector3d (0.5, 0, 0.0), RigidBodyDynamics::Math::Vector3d (1., 1., 1.));
			RigidBodyDynamics::Joint* joint_c = new RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d (0., 0., 1.));
			int body_c_id = model->AddBody(body_b_id, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(0., 1, 0.)), *joint_c, *body_c);

*/
			/*VectorNd Q = VectorNd::Zero (model->dof_count);
			VectorNd QDot = VectorNd::Zero (model->dof_count);
			VectorNd Tau = VectorNd::Zero (model->dof_count);
			VectorNd QDDot = VectorNd::Zero (model->dof_count);*/

		}
	}

	void RigidDynamicsBodyPlugin::setScene(Scene* scene)
	{
		_pScene = scene;
	}
}
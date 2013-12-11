#include "ODEDynamicsPanel.h"
#include "ODEPhysicsWorld.h"

namespace Etoile
{
	ODEDynamicsPanel::ODEDynamicsPanel(QWidget *parent)
		: QWidget(parent), _pWorld(NULL)
	{
		setWindowIcon(QIcon("./styles/icon.png"));
		ui.setupUi(this);
		ui.start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		ui.stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
		connect(ui.start, SIGNAL(clicked()), this, SLOT(start()));
		connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
		connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	}

	ODEDynamicsPanel::~ODEDynamicsPanel()
	{

	}

	void ODEDynamicsPanel::start()
	{
		if(_pWorld != NULL)
		{
			_pWorld->updatePhysicsObjects();
			_timer.setInterval(1000.0/ui.fps->value());
			_timer.start();
		}
	}

	void ODEDynamicsPanel::stop()
	{
		_timer.stop();
	}

	void ODEDynamicsPanel::update()
	{
		if(_pWorld != NULL)
		{
			_pWorld->doPhysics();
			
		/*if(world.getPhysicsObjects().size() > 0)
		{
			dBody* body = world.getPhysicsObjects().begin()->second->getBody();
			const dReal* v = body->getPosition();
			std::cout<<v[0]<< "  "<<v[1]<<"  "<<v[2] <<std::endl;
		}*/
		}
	}
}
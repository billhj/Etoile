#ifndef ODEDYNAMICSPANEL_H
#define ODEDYNAMICSPANEL_H

#include <QWidget>
#include "ui_ODEDynamicsPanel.h"
#include <qtimer.h>
namespace Etoile
{
	class ODEPhysicsWorld;
	class ODEDynamicsPanel : public QWidget
	{
		Q_OBJECT

	public:
		ODEDynamicsPanel(QWidget *parent = 0);
		~ODEDynamicsPanel();
		void setODEPhysicsWorld(ODEPhysicsWorld* world){_pWorld = world;}
		public slots:
			void start();
			void stop();
			void update();
	private:
		Ui::ODEDynamicsPanel ui;
		QTimer _timer;
		ODEPhysicsWorld* _pWorld;
	};
}
#endif // ODEDYNAMICSPANEL_H

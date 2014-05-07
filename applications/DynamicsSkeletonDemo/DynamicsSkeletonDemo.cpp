#include "DynamicsSkeletonDemo.h"
#include <QFormLayout>

DynamicsSkeletonDemo::DynamicsSkeletonDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_widget = new DynamicsGLWidget(this);
	this->setCentralWidget(_widget);


	{
		_dock_pd = new QDockWidget(this);
		this->addDockWidget(Qt::RightDockWidgetArea, _dock_pd);
		QWidget* widget = new QWidget();
		_dock_pd->setWidget(widget);
		QFormLayout* layout = new QFormLayout();
		QDoubleSpinBox* dt = new QDoubleSpinBox();
		dt->setRange(0.001, 0.1);
		dt->setSingleStep(0.001);
		dt->setDecimals(3);
		layout->addRow("dt", dt);
		kp = new QDoubleSpinBox();
		kp->setRange(0.01, 3000);
		kp->setValue(1000);
		kp->setSingleStep(0.1);
		layout->addRow("kp", kp);
		kd = new QDoubleSpinBox();
		kd->setRange(0.01, 3000);
		kd->setSingleStep(0.1);
		kd->setValue(10);
		layout->addRow("kd", kd);

		widget->setLayout(layout);

		connect(dt, SIGNAL(valueChanged(double)), this, SLOT(changeDeltaT(double)));
		connect(_widget, SIGNAL(jointClicked()), this, SLOT(jointSelected()));
		connect(kp, SIGNAL(valueChanged(double)), this, SLOT(setKp(double)));
		connect(kd, SIGNAL(valueChanged(double)), this, SLOT(setKd(double)));
	}

	{
		_dock_ag = new QDockWidget(this);
		this->addDockWidget(Qt::RightDockWidgetArea, _dock_ag);
		QWidget* widget2 = new QWidget();
		_dock_ag->setWidget(widget2); 

		QFormLayout* layout2 = new QFormLayout();
		QDoubleSpinBox* dt2 = new QDoubleSpinBox();
		dt2->setRange(0.001, 0.1);
		dt2->setSingleStep(0.001);
		dt2->setDecimals(3);
		layout2->addRow("dt", dt2);
		kh = new QDoubleSpinBox();
		kh->setRange(0.01, 3000);
		kh->setValue(1000);
		kh->setSingleStep(0.1);
		layout2->addRow("kh", kh);
		kl = new QDoubleSpinBox();
		kl->setRange(0.01, 3000);
		kl->setSingleStep(0.1);
		kl->setValue(10);
		layout2->addRow("kl", kl);

		h = new QDoubleSpinBox();
		h->setRange(-3.1, 3.1);
		h->setValue(3);
		h->setSingleStep(0.01);
		layout2->addRow("h", h);
		l = new QDoubleSpinBox();
		l->setRange(-3.1, 3.1);
		l->setSingleStep(0.01);
		l->setValue(0);
		layout2->addRow("l", l);

		tension = new QDoubleSpinBox();
		tension->setRange(0.1, 3000);
		tension->setSingleStep(1);
		tension->setValue(kh->value() + kl->value());
		layout2->addRow("tension", tension);

		widget2->setLayout(layout2);
		connect(dt2, SIGNAL(valueChanged(double)), this, SLOT(changeDeltaT(double)));
		connect(kl, SIGNAL(valueChanged(double)), this, SLOT(setKl(double)));
		connect(kh, SIGNAL(valueChanged(double)), this, SLOT(setKh(double)));
		connect(l, SIGNAL(valueChanged(double)), this, SLOT(seth(double)));
		connect(h, SIGNAL(valueChanged(double)), this, SLOT(setl(double)));
		connect(tension, SIGNAL(valueChanged(double)), this, SLOT(setTension(double)));
		connect(_widget, SIGNAL(changedController()), this, SLOT(changeMotor()));
	}

	changeMotor();
}

void DynamicsSkeletonDemo::setKp(double)
{
	if(!_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::PDJointMotor*)motor)->setKp(kp->value());

		}
	}
}

void DynamicsSkeletonDemo::setKd(double)
{
	if(!_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::PDJointMotor*)motor)->setKd(kd->value());

		}
	}
}

void DynamicsSkeletonDemo::changeDeltaT(double t)
{
	_widget->dt = t;
}

void DynamicsSkeletonDemo::jointSelected()
{
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];
			if(_widget->_usePD)
			{
				kd->setValue(((Etoile::PDJointMotor*)motor)->Kd());
				kp->setValue(((Etoile::PDJointMotor*)motor)->Kp());
			}else
			{
				kl->setValue(((Etoile::AntagonisticJointMotor*)motor)->Kl());
				kh->setValue(((Etoile::AntagonisticJointMotor*)motor)->Kh());
				h->setValue(((Etoile::AntagonisticJointMotor*)motor)->high());
				l->setValue(((Etoile::AntagonisticJointMotor*)motor)->low());
				tension->setValue(((Etoile::AntagonisticJointMotor*)motor)->tensionConstant());
			}
		}
	}
}


void DynamicsSkeletonDemo::setKl(double)
{
	if(_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::AntagonisticJointMotor*)motor)->setKl(kl->value());

		}
	}
}

void DynamicsSkeletonDemo::setKh(double)
{
	if(_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::AntagonisticJointMotor*)motor)->setKh(kh->value());

		}
	}
}
void DynamicsSkeletonDemo::setl(double)
{
	if(_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::AntagonisticJointMotor*)motor)->setLow(l->value());

		}
	}
}

void DynamicsSkeletonDemo::seth(double)
{
	if(_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::AntagonisticJointMotor*)motor)->setHigh(h->value());

		}
	}
}

void DynamicsSkeletonDemo::setTension(double)
{
	if(_widget->_usePD) return;
	for(int i = 0; i < _widget->_id.size(); ++i)
	{
		Etoile::Joint* j = _widget->_skeleton->getJoint(_widget->_selectedJointIndex);
		if(j == NULL) continue;
		if(_widget->_id[i].compare(j->getName()) == 0)
		{
			Etoile::JointMotor* motor = _widget->_motors[i];

			((Etoile::AntagonisticJointMotor*)motor)->setTensionConstant(tension->value());
			kl->setValue(((Etoile::AntagonisticJointMotor*)motor)->Kl());
			kh->setValue(((Etoile::AntagonisticJointMotor*)motor)->Kh());
			h->setValue(((Etoile::AntagonisticJointMotor*)motor)->high());
			l->setValue(((Etoile::AntagonisticJointMotor*)motor)->low());
		}
	}
}

void DynamicsSkeletonDemo::changeMotor()
{
	if(_widget->_usePD)
	{
		_dock_pd->setVisible(true);
		_dock_ag->setVisible(false);
		setKd(0);
		setKp(0);
	}else
	{
		_dock_pd->setVisible(false);
		_dock_ag->setVisible(true);
		seth(0);
		setl(0);
		setKl(0);
		setKh(0);
		setTension(0);
	}
}

DynamicsSkeletonDemo::~DynamicsSkeletonDemo()
{

}

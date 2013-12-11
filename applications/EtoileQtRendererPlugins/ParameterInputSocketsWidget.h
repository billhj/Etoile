#pragma once

#include <QWidget>
#include "module/flowgraph/GeneralSocket.h"
#include <qlabel.h>
#include <string>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QFormLayout>

class ParameterInputSocketsWidget : public QWidget
{
public:
	ParameterInputSocketsWidget(QWidget *parent = 0)
	{
		layout = new QVBoxLayout;
		this->setLayout(layout);
	}
	void addWidget(QWidget* widget)
	{
		layout->addWidget(widget);
	}
private:
	QVBoxLayout *layout;
};

class IntInputSocketWidget : public QWidget
{
	Q_OBJECT

public:
	IntInputSocketWidget(QWidget *parent = 0)
	{
		_label = new QLabel();
		_spin = new QSpinBox();
		QFormLayout *layout = new QFormLayout;
		layout->addRow(_label, _spin);
		this->setLayout(layout);
		connect(_spin,SIGNAL(valueChanged(int)), this, SLOT(sendValue(int)));
	}
	~IntInputSocketWidget(){}
	void bindParameter(Etoile::IntInputSocket* socket, int value)
	{
		if(socket == NULL) return;
		_socket = socket;
		_label->setText(QString(socket->getName().c_str()));
		_spin->setValue(value);
	}
public slots:
	void sendValue(int)
	{
		if(_socket != NULL)
		{
			int value = _spin->value();
			_socket->perform(&value);
		}
	}

private:
	Etoile::IntInputSocket* _socket;
	QLabel* _label;
	QSpinBox* _spin;
};

class FloatInputSocketWidget : public QWidget
{
	Q_OBJECT

public:
	FloatInputSocketWidget(QWidget *parent = 0)
	{
		_label = new QLabel();
		_spin = new QDoubleSpinBox();
		_spin->setSingleStep(0.1);
		QFormLayout *layout = new QFormLayout;
		layout->addRow(_label, _spin);
		this->setLayout(layout);
		connect(_spin,SIGNAL(valueChanged(double)), this, SLOT(sendValue(double)));
	}
	~FloatInputSocketWidget(){}
	void bindParameter(Etoile::FloatInputSocket* socket, float value)
	{
		if(socket == NULL) return;
		_socket = socket;
		_label->setText(QString(socket->getName().c_str()));
		_spin->setValue(value);
	}
public slots:
	void sendValue(double)
	{
		if(_socket != NULL)
		{
			float value = _spin->value();
			_socket->perform(&value);
		}
	}

private:
	Etoile::FloatInputSocket* _socket;
	QLabel* _label;
	QDoubleSpinBox* _spin;
};

#include "EtoileGLMeshRenderPassPlugin.h"
#include "EtoileGLQuadRenderPassPlugin.h"
#include <qpushbutton.h>

class GpuCompileWidget : public QWidget
{
	Q_OBJECT

public:
	GpuCompileWidget(QWidget *parent = 0) : _pass(NULL)
	{
		_label = new QLabel("GpuProgram");
		_button = new QPushButton("reCompile");
		QFormLayout *layout = new QFormLayout;
		layout->addRow(_label, _button);
		this->setLayout(layout);
		connect(_button,SIGNAL(clicked()), this, SLOT(compile()));
	}
	~GpuCompileWidget(){}
	void bindPass(Etoile::RenderPass* pass)
	{
		if(pass == NULL) return;
		_pass = pass;
	}
public slots:
	void compile()
	{
		if(_pass != NULL)
		{
			Etoile::GLMeshRenderPass* meshpass = dynamic_cast<Etoile::GLMeshRenderPass*>(_pass);
			if(meshpass != NULL)
			{
				std::vector<Etoile::RenderUnit*>& units = meshpass->getRenderUnits();
				for(unsigned int i = 0; i < units.size(); ++i)
				{
					Etoile::RenderUnit* unit = units[i];
					Etoile::MeshRenderUnit* munit = dynamic_cast<Etoile::MeshRenderUnit*>(unit);
					if(munit != NULL)
					{
						Etoile::Mesh* m = munit->getMesh();
						for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
						{
							Etoile::SubMesh* submesh = m->getSubMeshList()[j];
							Etoile::Material* mt = submesh->getMaterial();
							if(mt->getGpuProgram() != NULL)
								mt->getGpuProgram()->reCompile();
						}
					}
				}
			}

			Etoile::GLQuadRenderPass* quadpass = dynamic_cast<Etoile::GLQuadRenderPass*>(_pass);
			if(quadpass != NULL)
			{
				Etoile::Material* mt = quadpass->getMaterial();
				if(mt->getGpuProgram() != NULL)
					mt->getGpuProgram()->reCompile();
			}
		}
	}

private:
	Etoile::RenderPass* _pass;
	QPushButton* _button;
	QLabel* _label;
};

#include <QCheckBox>
class AABBWidget : public QWidget
{
	Q_OBJECT

public:
	AABBWidget(QWidget *parent = 0) : _pass(NULL)
	{
		_label = new QLabel("AABB");
		_box = new QCheckBox("show BoundingBox");
		QFormLayout *layout = new QFormLayout;
		layout->addRow(_label, _box);
		this->setLayout(layout);
		connect(_box,SIGNAL(stateChanged(int)), this, SLOT(changed(int)));
	}
	~AABBWidget(){}
	void bindPass(Etoile::GLMeshRenderPass* pass)
	{
		if(pass == NULL) return;
		_pass = pass;
		_box->setChecked(true);
	}
public slots:
	void changed(int v)
	{
		if(_pass != NULL)
		{
			Etoile::GLMeshRenderPass* meshpass = dynamic_cast<Etoile::GLMeshRenderPass*>(_pass);
			if(meshpass != NULL)
			{
				std::vector<Etoile::RenderUnit*>& units = meshpass->getRenderUnits();
				for(unsigned int i = 0; i < units.size(); ++i)
				{
					Etoile::RenderUnit* unit = units[i];
					Etoile::GLMeshRenderUnit* munit = dynamic_cast<Etoile::GLMeshRenderUnit*>(unit);
					if(munit != NULL)
					{
						munit->setAABBenable(v);
					}
				}
			}
		}
	}

private:
	Etoile::GLMeshRenderPass* _pass;
	QCheckBox* _box;
	QLabel* _label;
};
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file CameraWidget.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include <QDockWidget>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGLViewer/qglviewer.h>
#include <QSettings>
#include <QFileDialog>
#include <QThread>
#include <QTime>
#include <QTimer>

class CameraWidget : public QDockWidget
{
	Q_OBJECT
public:
	CameraWidget(const std::string& title, QGLViewer* widget) : QDockWidget(QString(title.c_str()))
	{
		glwidget = widget;
		_qgl = new QGridLayout();
		_x = new QLabel("x:"); 
		_y = new QLabel("y:"); 
		_z = new QLabel("z:");
		x = new QDoubleSpinBox(); 
		y = new QDoubleSpinBox(); 
		z = new QDoubleSpinBox();
		x->	setDecimals(10);
		y->	setDecimals(10);
		z->	setDecimals(10);
		_w = new QLabel("w:"); 
		_i = new QLabel("i:"); 
		_j = new QLabel("j:");
		_k = new QLabel("k:");
		w = new QDoubleSpinBox(); 
		i = new QDoubleSpinBox(); 
		j = new QDoubleSpinBox();
		k = new QDoubleSpinBox();
		w->	setDecimals(10);
		i->	setDecimals(10);
		j->	setDecimals(10);
		k->	setDecimals(10);

		_fv = new QLabel("Field of View:"); 
		_fieldView = new QDoubleSpinBox();
		_fieldView->setDecimals(2);
		_fieldView->setSingleStep(0.01);
		_qgl->addWidget(_x, 0, 0);
		_qgl->addWidget(x, 0, 1);
		_qgl->addWidget(_y, 1, 0);
		_qgl->addWidget(y, 1, 1);
		_qgl->addWidget(_z, 2, 0);
		_qgl->addWidget(z, 2, 1);
		_qgl->addWidget(_w, 3, 0);
		_qgl->addWidget(w, 3, 1);
		_qgl->addWidget(_i, 4, 0);
		_qgl->addWidget(i, 4, 1);
		_qgl->addWidget(_j, 5, 0);
		_qgl->addWidget(j, 5, 1);
		_qgl->addWidget(_k, 6, 0);
		_qgl->addWidget(k, 6, 1);

		x->setRange(-1000,1000);
		y->setRange(-1000,1000);
		z->setRange(-1000,1000);
		x->setSingleStep(0.01);
		y->setSingleStep(0.01);
		z->setSingleStep(0.01);

		w->setRange(-1000,1000);
		i->setRange(-1000,1000);
		j->setRange(-1000,1000);
		k->setRange(-1000,1000);
		w->setSingleStep(0.01);
		i->setSingleStep(0.01);
		j->setSingleStep(0.01);
		k->setSingleStep(0.01);

		QPushButton* save = new QPushButton("saveCamera");
		QPushButton* load = new QPushButton("loadCamera");
		_qgl->addWidget(save, 7, 0);
		_qgl->addWidget(load, 8, 0);

		_qgl->addWidget(_fv, 9, 0);
		_qgl->addWidget(_fieldView, 9, 1);

		_rotationAxis =  new QLabel("Rotation Axis:");
		rX = new QDoubleSpinBox(); 
		rY = new QDoubleSpinBox();
		rZ = new QDoubleSpinBox();
		//rY->setValue(1);
		_qgl->addWidget(_rotationAxis, 10, 0);
		_qgl->addWidget(rX, 11, 0);
		_qgl->addWidget(rY, 11, 1);
		_qgl->addWidget(rZ, 11, 2);


		_fps = new QLabel("fps:");
		_videoLength = new QLabel("video length:");

		fps = new QSpinBox(), videoLength = new QSpinBox(); 
		fps->setValue(25);
		videoLength->setValue(10);
		screenshot = new QPushButton("videoshot");

		_qgl->addWidget(_fps, 12, 0);
		_qgl->addWidget(fps, 12, 1);
		_qgl->addWidget(_videoLength, 13, 0);
		_qgl->addWidget(videoLength, 13, 1);
		_qgl->addWidget(screenshot, 14, 0);

		connect(screenshot, SIGNAL(clicked() ), this, SLOT(applyVideoScreenShot()));

		if(glwidget!=0)
		{
			qglviewer::Camera *cam = glwidget->camera();
			qglviewer::Vec pos = cam->position();
			x->setValue(pos[0]);
			y->setValue(pos[1]);
			z->setValue(pos[2]);

			w->setValue(cam->orientation()[3]); 
			i->setValue(cam->orientation()[0]); 
			j->setValue(cam->orientation()[1]); 
			k->setValue(cam->orientation()[2]); 
			_fieldView->setValue(cam->fieldOfView());
		}

		QVBoxLayout * upperLayout = new QVBoxLayout;
		upperLayout->addLayout (_qgl);
		upperLayout->addStretch (0);
		_qwidget = new QWidget;
		_qwidget->setLayout(upperLayout);
		this->setWidget(_qwidget);

		connect(x, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(y, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(z, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(w, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(i, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(j, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(k, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(_fieldView, SIGNAL(editingFinished () ), this, SLOT(updateCamera()));
		connect(load,  SIGNAL(clicked()), this, SLOT(loadStateCamera()));
		connect(save,  SIGNAL(clicked()), this, SLOT(saveStateCamera()));
		QTimer* timer = new QTimer();
		connect(timer, SIGNAL(timeout()), this, SLOT(updatePositionFromCamera()));
		timer->start(1000);
		dirSave = "./img/";
		connect(&_videoshot, SIGNAL(timeout()), this, SLOT(applySingleScreenShot()));
	}
	~CameraWidget(){}

	public slots:
		void updateCamera()
		{
			if(glwidget!=0)
			{
				qglviewer::Camera *cam = glwidget->camera();
				cam->setPosition(qglviewer::Vec(x->value(), y->value(), z->value()));
				cam->setOrientation(qglviewer::Quaternion(i->value(), j->value(), k->value(), w->value()));
				cam->setFieldOfView(_fieldView->value());
				glwidget->updateGL();
			}
		}
		void updatePositionFromCamera()
		{
			if(glwidget!=0)
			{
				qglviewer::Camera *cam = glwidget->camera();
				qglviewer::Vec pos = cam->position();
				x->setValue(pos[0]);
				y->setValue(pos[1]);
				z->setValue(pos[2]);
				i->setValue(cam->orientation()[0]); 
				j->setValue(cam->orientation()[1]); 
				k->setValue(cam->orientation()[2]); 
				w->setValue(cam->orientation()[3]); 
				_fieldView->setValue(cam->fieldOfView());
			}
		}

		void applyVideoScreenShot()
		{
			if(glwidget!=0)
			{
				_count = 0;
				QString dirName = dirSave;//QFileDialog::getExistingDirectory(this, "Choose dir for save images", "./savedata");
				if (dirName.isEmpty()) return;

				std::cout<<"screen shot animation"<<std::endl;

				int frameCount  = fps->value() *  videoLength->value();
				QString t = QDate::currentDate().toString();

				if(rX->value() != 0 || rY->value() != 0 || rZ->value() != 0 )
				{
					const qglviewer::Vec& initPoint = glwidget->camera()->position();
					qglviewer::Vec rotationCenter(0,0,0);
					float rotationAngle = 2 * M_PI / (float)frameCount;

						
					qglviewer::Frame* camframe =  glwidget->camera()->frame();
					qglviewer::Vec localAxis = camframe->localTransformOf(qglviewer::Vec(rX->value(), rY->value(),rZ->value()));
					qglviewer::Quaternion rotationQuaternion(localAxis, rotationAngle);

					for(int i = 0; i < frameCount; ++i )
					{
						camframe->rotateAroundPoint(rotationQuaternion, rotationCenter);
						glwidget->updateGL();
						applySingleScreenShot();
						/*
						QString  fileName = QString("%3/%1_%2.png").arg(t).arg(i,5 , 10 ,QLatin1Char('0')).arg(dirName);
						glwidget->setSnapshotFormat("PNG");
						glwidget->saveSnapshot(fileName); 
						*/
					}
				}
				else
				{
					_videoshot.setInterval(1000.0/ fps->value());
					_videoshot.start();
					/*for(int i = 0; i < frameCount; ++i )
					{
						glwidget->updateGL();
						QString  fileName = QString("%3/%1_%2.png").arg(t).arg(i,5 , 10 ,QLatin1Char('0')).arg(dirName);
						glwidget->setSnapshotFormat("PNG");
						glwidget->saveSnapshot(fileName); 
					}*/
				}
			}
		}


		void saveStateCamera()
		{
			QSettings settings(QString("./savedata/camera.ini"),QSettings::IniFormat);
			settings.setValue("x", x->value());
			settings.setValue("y", y->value());
			settings.setValue("z", z->value());
			settings.setValue("w", w->value());
			settings.setValue("i", i->value());
			settings.setValue("j", j->value());
			settings.setValue("k", k->value());
			settings.setValue("fieldView", _fieldView->value());
		}

		void loadStateCamera()
		{
			QSettings settings(QString("./savedata/camera.ini"), QSettings::IniFormat);
			x->setValue(settings.value("x").toFloat());
			y->setValue(settings.value("y").toFloat());
			z->setValue(settings.value("z").toFloat());
			w->setValue(settings.value("w").toFloat());
			i->setValue(settings.value("i").toFloat());
			j->setValue(settings.value("j").toFloat());
			k->setValue(settings.value("k").toFloat());
			_fieldView->setValue(settings.value("fieldView").toFloat());
			updateCamera();
		}
		public slots:
			void applySingleScreenShot()
			{
				QString  fileName = QString("%3/%1_%2.png").arg("").arg(_count ,5 , 10 ,QLatin1Char('0')).arg(dirSave);
						glwidget->setSnapshotFormat("PNG");
						glwidget->saveSnapshot(fileName); 
						int frameCount  = fps->value() *  videoLength->value();
						if(_count >= frameCount)
						{
							_videoshot.stop();
						}
						_count++;
			}
private:

	QGridLayout* _qgl;
	QWidget* _qwidget;
	QLabel *_x, *_y, *_z, *_w, *_i, *_j, *_k , *_fv;
	QDoubleSpinBox *x, *y, *z, *w, *i, *j, *k;
	QDoubleSpinBox* _fieldView;

	QLabel *_rotationAxis;
	QDoubleSpinBox *rX, *rY, *rZ;

	QLabel *_fps, *_videoLength;
	QSpinBox *fps, *videoLength; 
	QPushButton* screenshot;
	QGLViewer* glwidget;
	QString dirSave;
	int _count;
	QTimer _videoshot;
};

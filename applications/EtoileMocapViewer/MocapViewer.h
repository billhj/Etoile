/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapViewer.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include <QtGui/QMainWindow>
#include "ui_MocapViewer.h"
#include "MocapWidget.h"
#include "module/flowgraph/GraphScene.h"

class MocapViewer : public QMainWindow
{
	Q_OBJECT

public:
	MocapViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MocapViewer();
	void addMenu();
	void openFile(const std::string& signal);
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void openBVH();
	void openAbout();
	void checkPauseButton();
	void pushPauseButton();
	void checkCycleButton();
	void pushCycleButton();
	void slideFrame(int);
	void sliderChanged(int);
	void spinChangeFps(int);
private:
	Ui::MocapViewerClass ui;
	MocapWidget* _pMocapWidget;
};



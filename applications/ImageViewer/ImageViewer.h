#pragma once

#include <QMainWindow>
#include <QPrinter>
#include "util/ImageLoader/Image.h"
#include "module/flowgraph/ImageSocket.h"
#include "module/flowgraph/GraphScene.h"

 class QAction;
 class QLabel;
 class QMenu;
 class QScrollArea;
 class QScrollBar;

 class ImageViewer : public QMainWindow
 {
     Q_OBJECT

 public:
     ImageViewer();
	 void openImage(Etoile::Image* image);
	 void openFile(const std::string& filename);
	protected:
 private slots:
     void open();
	 void save();
     void print();
     void zoomIn();
     void zoomOut();
     void normalSize();
     void fitToWindow();
     void about();
	 //void fft();
	 void original();

 private:
     void createActions();
     void createMenus();
     void updateActions();
     void scaleImage(double factor);
     void adjustScrollBar(QScrollBar *scrollBar, double factor);

     QLabel *imageLabel;
     QScrollArea *scrollArea;
     double scaleFactor;

     QPrinter printer;

     QAction *openAct;
     QAction *printAct;
	 QAction *saveAct;
     QAction *exitAct;
     QAction *zoomInAct;
     QAction *zoomOutAct;
     QAction *normalSizeAct;
     QAction *fitToWindowAct;
     QAction *aboutAct;
	 //QAction *fftAct;
	 QAction *originalAct;

     QMenu *fileMenu;
     QMenu *viewMenu;
	 QMenu *filteringMenu;
     QMenu *helpMenu;

	 QImage *qimage;
 };

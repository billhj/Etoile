#include <QtGui>
#include <Qcolor>

#include "ImageViewer.h"
#include "util/ImageLoader/ReadImage.h"
#include "math/Vec4.h"

//extern int applyFFT(float* input, float* output, int size);
ImageViewer* viewer = NULL;
void startEtoileImageViewer()
{
	viewer = new ImageViewer();
	viewer->setVisible(true);
}

void endEtoileImageViewer()
{
	if(viewer != NULL)
	{
		viewer->setVisible(false);
		viewer = NULL;
	}
}

ImageViewer::ImageViewer()
{
	setWindowIcon(QIcon("./styles/icon.png"));
	qimage = NULL;
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	setCentralWidget(scrollArea);

	createActions();
	createMenus();

	setWindowTitle(tr("Image Viewer"));
	resize(500, 400);
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::openImage(Etoile::Image * image)
{
	if(image == NULL) return;
	qimage = new QImage(image->getWidth(), image->getHeight(), QImage::Format_ARGB32);
	for(int i = 0; i < image->getHeight(); ++i)
	{
		for(int j = 0; j < image->getWidth(); ++j)
		{
			Etoile::Vec4f color = image->getData()[i * image->getWidth() + j];
			QRgb value = qRgb(color.r() * 255, color.g() * 255, color.b() * 255); // 0xffbd9527
			qimage->setPixel(j, image->getHeight() - i - 1, value);
		}
	}
	//image.loadFromData( (uchar *)&eimage.getData()[0], eimage.getWidth() * eimage.getHeight(), QImage::Format_ARGB32);

	imageLabel->setPixmap(QPixmap::fromImage(*qimage));
	scaleFactor = 1.0;

	printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	updateActions();

	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}

void ImageViewer::openFile(const std::string& filename)
{
	Etoile::Image eimage;
	bool bo = Etoile::ReadImage::loadImageFromFile(filename, eimage);
	if(!bo) return;
	openImage(&eimage);
}

void ImageViewer::open()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		//        QImage image(fileName);
		//        if (image.isNull()) {
		//            QMessageBox::information(this, tr("Image Viewer"),
		//                                     tr("Cannot load %1.").arg(fileName));
		//            return;
		//        }

		openFile(fileName.toStdString());
	}else{
		QMessageBox::information(this, tr("Image Viewer"),
			tr("Cannot load %1.").arg(fileName));
	}
}

void ImageViewer::save()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Image"), "",
		tr("Images (*.png *.xpm *.jpg)"));
	imageLabel->pixmap()->save(fileName);
}

void ImageViewer::print()
{
	Q_ASSERT(imageLabel->pixmap());
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = imageLabel->pixmap()->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(imageLabel->pixmap()->rect());
		painter.drawPixmap(0, 0, *imageLabel->pixmap());
	}
}

void ImageViewer::zoomIn()
{
	scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
	scaleImage(0.8);
}

void ImageViewer::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow) {
		normalSize();
	}
	updateActions();
}

void ImageViewer::about()
{
	QMessageBox::about(this, tr("About Image Viewer"),
		tr("Graphics Interface for image processing"));
}

void ImageViewer::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(tr("&Save..."), this);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	printAct = new QAction(tr("&Print..."), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setEnabled(false);
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	//fftAct = new QAction(tr("fft"), this);
	///connect(fftAct, SIGNAL(triggered()), this, SLOT(fft()));
	originalAct = new QAction(tr("original"), this);
	connect(originalAct, SIGNAL(triggered()), this, SLOT(original()));
}

void ImageViewer::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	viewMenu->addSeparator();
	viewMenu->addAction(fitToWindowAct);

	filteringMenu = new QMenu(tr("&Filter"), this);
	//	filteringMenu->addAction(fftAct);
	filteringMenu->addSeparator();
	filteringMenu->addAction(originalAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(filteringMenu);
	menuBar()->addMenu(helpMenu);
}

void ImageViewer::updateActions()
{
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep()/2)));
}

/*
void ImageViewer::fft()
{
if(qimage != NULL)
{

std::vector<float> pixels;
std::vector<float> output;
for(int i = 0; i < qimage->height(); ++i)
{
for(int j = 0; j < qimage->width(); ++j)
{
QRgb value = qimage->pixel(j, i);
QColor v(value);
pixels.push_back(v.redF());
pixels.push_back(v.greenF());
pixels.push_back(v.blueF());
output.push_back(0.0f);
output.push_back(0.0f);
output.push_back(0.0f);
}
}

applyFFT(&pixels[0], &output[0], pixels.size());
QImage image(qimage->width(), qimage->height(), QImage::Format_ARGB32);;
int idx = 0;
for(int i = 0; i < qimage->height(); ++i)
{
for(int j = 0; j < qimage->width(); ++j)
{
QRgb value = qRgb(output[idx] * 255, output[idx + 1] * 255, output[idx + 2] * 255);
image.setPixel(j, i, value);
idx += 3;
//std::cout<<i <<" " << j <<std::endl;
}
}
imageLabel->setPixmap(QPixmap::fromImage(image));
}
}*/

void ImageViewer::original()
{
	if(qimage != NULL)
	{
		imageLabel->setPixmap(QPixmap::fromImage(*qimage));
	}
}
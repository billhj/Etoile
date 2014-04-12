#include "EtoileGraphEditor.h"

EtoileGraphEditor::EtoileGraphEditor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_editor = new GraphEditor(this);
	this->setCentralWidget(_editor);
}

EtoileGraphEditor::~EtoileGraphEditor()
{

}

#ifndef ETOILEGRAPHEDITOR_H
#define ETOILEGRAPHEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_EtoileGraphEditor.h"
#include "GraphEditor.h"

class EtoileGraphEditor : public QMainWindow
{
	Q_OBJECT

public:
	EtoileGraphEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~EtoileGraphEditor();

private:
	Ui::EtoileGraphEditorClass ui;
	GraphEditor* _editor;
};

#endif // ETOILEGRAPHEDITOR_H

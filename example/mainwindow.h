/*
** Arabtype Copyright 2012-2015(c) Wael El Oraiby. All Rights Reserved
**
** This file is in public domain
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	virtual void		paintEvent(QPaintEvent* event);

private:
	Ui::MainWindow*		ui__;
	uchar*			data__;
	static FT_Library	ftlib__;
};

#endif // MAINWINDOW_H

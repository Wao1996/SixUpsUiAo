#pragma once

#include <QWidget>
#include "ui_WaitWindow.h"

class WaitWindow : public QWidget
{
	Q_OBJECT

public:
	WaitWindow(QWidget *parent = Q_NULLPTR);
	~WaitWindow();

private:
	Ui::WaitWindow ui;
};

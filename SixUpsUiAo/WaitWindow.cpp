#include "WaitWindow.h"

WaitWindow::WaitWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.progressBar->setMinimum(0);  // ��Сֵ
	ui.progressBar->setMaximum(0);  // ���ֵ
}

WaitWindow::~WaitWindow()
{
}

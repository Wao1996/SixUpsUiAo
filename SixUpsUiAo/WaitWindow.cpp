#include "WaitWindow.h"

WaitWindow::WaitWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.progressBar->setMinimum(0);  // 最小值
	ui.progressBar->setMaximum(0);  // 最大值
}

WaitWindow::~WaitWindow()
{
}

#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "ui_SixUpsUiAo.h"
#include "ParameterCalibrate.h"
#include "Global.h"
#include "QPmacThread.h"

class SixUpsUiAo : public QMainWindow
{
    Q_OBJECT

public:
    SixUpsUiAo(QWidget *parent = Q_NULLPTR);
	~SixUpsUiAo();

	ParameterCalibrate * ParameterCalibrateUI;
private:
    Ui::SixUpsUiAoClass ui;

	QPixmap onIcon;
	QPixmap offIcon;
	void initIcon();

	//Pmac线程相关
	QPmacThread *myPmacThread = nullptr;
	QThread *pmacQthread = nullptr;
	void switchPmacThread();//切换Pmac线程的开启与关闭
signals:
	void getMotorDisp();//获取电机位置
private slots:
	/*工具栏*/
	void on_paraCailbrate_triggered();

	/*******************PMAC*******************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();
};

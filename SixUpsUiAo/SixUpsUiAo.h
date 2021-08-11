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

	//Pmac�߳����
	QPmacThread *myPmacThread = nullptr;
	QThread *pmacQthread = nullptr;
	void switchPmacThread();//�л�Pmac�̵߳Ŀ�����ر�
signals:
	void getMotorDisp();//��ȡ���λ��
private slots:
	/*������*/
	void on_paraCailbrate_triggered();

	/*******************PMAC*******************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();
};

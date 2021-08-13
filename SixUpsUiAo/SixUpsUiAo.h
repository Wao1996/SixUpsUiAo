#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "ui_SixUpsUiAo.h"
#include "ParameterCalibrate.h"
#include "Global.h"
#include "QPmac.h"
#include "UpsCalculateThread.h"

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
	QPixmap loadingIcon;

	//各支链限位状态图片列表  因为C++不能使用eval语句
	QList<QLabel*> qlabNegLimit_group;
	QList<QLabel*> qlabPosLimit_group;
	QList<QLineEdit *> realTimeLengths_group;
	QList<QLineEdit *> realTimePos_group;

	/************定时器***********/
	QTimer *dataGatherTimer;//数据收集定时器
	QTimer *updateUiDataTimer;//刷新ui定时器
	QTimer *upsCalculateTimer;//并联机构计算定时器

	/***********并联机构计算线程**********/
	QThread *calculateQthread = nullptr;
	UpsCalculateThread *myUpsCalculateThread = nullptr;

	/***********初始化相关********/
	void initIcon();
	void initUIList();
	void initStructPara();
	/*********Pmac相关************/
	QPmac *myPmac = nullptr;
	void switchPmacThread();//切换Pmac线程的开启与关闭

signals:

private slots:
	/*********工具栏************/
	void on_paraCailbrate_triggered();
	/*********定时器***********/
	void on_updateUiDataTimer();//刷新界面数据定时器溢出
	
	/**********PMAC************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();

};

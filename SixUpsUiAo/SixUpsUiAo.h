#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "ui_SixUpsUiAo.h"
#include "ParameterCalibrate.h"
#include "Global.h"
#include "QPmac.h"

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

	//��֧����λ״̬ͼƬ�б�  ��ΪC++����ʹ��eval���
	QList<QLabel*> qlabNegLimit_group;
	QList<QLabel*> qlabPosLimit_group;

	void initIcon();
	void initQlab();
	/************��ʱ��***********/
	QTimer *dataGatherTimer;//�����ռ���ʱ��
	QTimer *updateUiDataTimer;//ˢ��ui��ʱ��
	
	/*********Pmac�߳����************/
	QPmac *myPmac = nullptr;
	QThread *pmacQthread = nullptr;
	void switchPmacThread();//�л�Pmac�̵߳Ŀ�����ر�
signals:
	void getMotorDisp();//��ȡ���λ��
private slots:
	/*********������************/
	void on_paraCailbrate_triggered();
	/*********��ʱ��***********/
	void on_updateUiDataTimer();//ˢ�½������ݶ�ʱ�����
	
	/**********PMAC************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();
};

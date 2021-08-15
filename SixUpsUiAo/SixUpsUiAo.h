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

	//��֧����λ״̬ͼƬ�б�  ��ΪC++����ʹ��eval���
	QList<QLabel*> qlabNegLimit_group;
	QList<QLabel*> qlabPosLimit_group;
	QList<QLineEdit *> realTimeLengths_group;//�˳���ʾ
	QList<QLineEdit *> realTimePos_group;//λ����ʾ


	QList<QDoubleSpinBox *> AbsTarPos_group;//�����˶� ����λ�������

	QList<QDoubleSpinBox *> jogInc_group;//�����˶� ����㶯����
	QList<QToolButton *> dipJog_group;//�����˶� ����㶯��ť
	QList<QToolButton *> prsJogPos_group;//�����˶� ���������˶�
	QList<QToolButton *> prsJogNeg_group;//�����˶� ���������˶�

	/************��ʱ��***********/
	QTimer *dataGatherTimer;//�����ռ���ʱ��
	QTimer *updateUiDataTimer;//ˢ��ui��ʱ��
	QTimer *upsCalculateTimer;//�����������㶨ʱ��

	/***********�������������߳�**********/
	QThread *calculateQthread = nullptr;
	UpsCalculateThread *myUpsCalculateThread = nullptr;

	/***********��ʼ�����********/
	void initIcon();
	void initUIList();
	void initStructPara();
	void initConnect();
	/*********Pmac���************/
	QPmac *myPmac = nullptr;
	void switchPmacThread();//�л�Pmac�̵߳Ŀ�����ر�


signals:

private slots:
	/*********������************/
	void on_paraCailbrate_triggered();
	/*********��ʱ��***********/
	void on_updateUiDataTimer();//ˢ�½������ݶ�ʱ�����
	
	/**********PMAC************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();

	/**************���ʹ��************/
	void on_servoOnBtn_clicked();
	void on_servoOffBtn_clicked();
	/***********�����˶�***************/
	void on_getRealTimePosBtn_clicked();//
	void absTarPos_group_valueChange(double);
	/***********�����˶�***************/
	void on_led_jogSpeed_valueChanged(double);//�����˶��ٶȸı�
	void on_jogStopBtn_clicked();//���е��ֹͣ
	void led_jogInc_valueChanged(double);//����㶯 ����� �źŲ�
	void dipJog_Clicked();//����綯 ��ť��� �źŲ�
	void prsJogPos_pressed();//�����㶯 ������ť���� �źŲ�
	void prsJogPos_released();//�����㶯 ������ť�ɿ� �źŲ�
	void prsJogNeg_pressed();//�����㶯 ������ť���� �źŲ�
	void prsJogNeg_released();//�����㶯 ������ť�ɿ� �źŲ�

};

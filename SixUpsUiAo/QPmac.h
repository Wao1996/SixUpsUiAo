#pragma once
//#pragma execution_character_set("utf-8")
#include <QtCore>
#include <QMessageBox>
#include "PmacDeviceLib.h"
#include "Global.h"

class QPmac :public QObject
{
	Q_OBJECT
public:
	QPmac(QObject *parent = Q_NULLPTR);
	virtual ~QPmac();
	
	bool creatPmacSelect();//ѡ��pmac
	void initPmac();//��ʼ��pmac

	int getNegLimitState(int);//��ȡ����λ״̬
	int getPosLimitState(int);//��ȡ����λ״̬
	double getCurLengths(int);//��ȡ֧������

	void jogDisp(int num, double disp);//����㶯
	void jogPosContinuously(int num);//����+�˶�
	void jogNegContinuously(int num);//����-�˶�
	void jogStop();//��������ֹͣ ͬʱҲ�ǿ���ʹ��
	void jogStop(int num);//ĳ����ֹͣ
	void setJogSpeed(int num, double speed);//�����ֶ��˶��ٶ�
	void setServoOff();//���е��ʧȥʹ��
public slots:
	void on_dataGatherTimer();
public:
	PCOMMSERVERLib::PmacDevice *Pmac0;
	int pDeviceNumber;
	bool pbSuccess_select;
	bool pbSuccess_open;
	bool pbSucess_download;
	QString pAnswer;
	bool bAddLF;
	int pstatus;
};


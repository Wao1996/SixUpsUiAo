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
	bool downloadFile(QString strFile);

	/*��ȡ״̬*/
	int getNegLimitState(int);//��ȡ����λ״̬
	int getPosLimitState(int);//��ȡ����λ״̬
	int getHomeCompleteState(int);//��ȡ�������״̬
	double getCurLengths(int);//��ȡ֧������
	
	/*�����˶�����*/
	void jogDisp(int num, double disp);//����㶯
	void jogPosContinuously(int num);//����+�˶�
	void jogNegContinuously(int num);//����-�˶�
	void jogStop();//��������ֹͣ ͬʱҲ�ǿ���ʹ��
	void jogStop(int num);//ĳ����ֹͣ
	void setJogSpeed(int num, double speed);//�����ֶ��˶��ٶ�
	void setServoOff();//���е��ʧȥʹ��

	/*���������λ*/
	void setAllAxleHome();//���������
	void setALLaxleHomez();//������ٻ���
	/*�����˶�*/
	void upsAbsMove(Matrix<double, 6, 1> absL);//������������λ���˶�
	void upsIncMove(Matrix<double, 6, 1> incL);//�����������λ���˶�
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


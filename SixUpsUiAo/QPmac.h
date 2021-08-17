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
	bool downloadFile(QString strFile);//���س���

	/*��ȡ״̬*/
	int getNegLimitState(int);//��ȡĳһ����ĸ���λ״̬
	void getNegLimitState(VectorXi & negLimitState);//ֱ�ӻ�ȡ������ĸ���λ״̬
	int getPosLimitState(int);//��ȡĳһ���������λ״̬
	void getPosLimitState(VectorXi & posLimitState);//ֱ�ӻ�ȡ�����������λ״̬
	int getHomeCompleteState(int);//��ȡĳһ����Ļ���״̬
	void getHomeCompleteState(VectorXi &axleHomeCompleteState);//ֱ�ӻ�ȡ������ĵ������
	double getCurLengths(int);//��ȡĳһ����ĵ�ǰ֧������
	void getCurLengths(VectorXd &curLengths);//ֱ�ӻ�ȡ������ĵ�ǰ֧������
	
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


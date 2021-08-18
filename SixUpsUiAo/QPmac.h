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
	
	PCOMMSERVERLib::PmacDevice *Pmac0;
	int pDeviceNumber;
	bool pbSuccess_select;
	bool pbSuccess_open;
	bool pbSucess_download;
	QString pAnswer;
	bool bAddLF;
	int pstatus;

	QList<PmacVariable> pmacVariableRecipe;//����Ҫ�ɼ����������Ͱ�˳�����
	QList<VectorXd *>		pmacVariableList;//�ŵ������ݽ���˳����䵽����

	bool creatPmacSelect();//ѡ��pmac
	void initPmac();//��ʼ��pmac
	bool downloadFile(QString strFile);//���س���

	/*��ȡ״̬*/
	int getNegLimitState(int);//��ȡĳһ����ĸ���λ״̬
	void getNegLimitState(VectorXd & negLimitState);//ֱ�ӻ�ȡ������ĸ���λ״̬
	int getPosLimitState(int);//��ȡĳһ���������λ״̬
	void getPosLimitState(VectorXd & posLimitState);//ֱ�ӻ�ȡ�����������λ״̬
	int getHomeCompleteState(int);//��ȡĳһ����Ļ���״̬
	void getHomeCompleteState(VectorXd &axleHomeCompleteState);//ֱ�ӻ�ȡ������ĵ������
	double getCurLengths(int);//��ȡĳһ����ĵ�ǰ֧������
	void getCurLengths(VectorXd &curLengths);//ֱ�ӻ�ȡ������ĵ�ǰ֧������
	/*��ȡ���״̬*/
	void getMotorsState(QList<PmacVariable> &pmacVariableRecipe, QList<VectorXd*> pmacVariableList);

	/*��ȡP����*/
	void getPVariableList(VectorXd &pVariable);
	/*����P����*/
	void setPvariable(int p, double data);

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
	void upsHomeMove(Matrix<double, 6, 1> absL);//�������� ��ƽ̨�����˶�

public slots:
	void on_dataGatherTimer();

};


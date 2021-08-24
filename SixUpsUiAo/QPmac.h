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

	QList<PmacVariable> pmacVariableRecipe; //����Ҫ�ɼ����������Ͱ�˳����õ�QList��
	QList<VectorXd *>	pmacVariableList;//���ɼ��������ݽ���˳����䵽����

	bool creatPmacSelect();//ѡ��pmac
	void initPmac();//��ʼ��pmac
	bool downloadFile(QString strFile);//���س���

	/*��ȡ���״̬������*/
	QString creatPmacVariableCommand(QList<PmacVariable> &pmacVariableRecipe);//������ȡpmac��������������ַ���
	void getPmacVariable(QList<PmacVariable> &pmacVariableRecipe, QString strCommand, QList<VectorXd*> pmacVariableList);

	/*����P����*/
	void setPvariable(int p, double data);
	
	void enablePLC(int num);//����PLC
	void disablePLC(int num);//�ر�PLC
	/*�����˶�����*/
	void jogDisp(int num, double disp);//����㶯
	void jogPosContinuously(int num);//����+�˶�
	void jogNegContinuously(int num);//����-�˶�
	void jogStop();//��������ֹͣ ͬʱҲ�ǿ���ʹ��
	void jogStop(int num);//ĳ����ֹͣ
	void setJogSpeed(int num, double speed);//�����ֶ��˶��ٶ�
	void setServoOff();//���е��ʧȥʹ��

	/***********���������λ*****************/
	void setHomeCompleteZero();//����㿪ʼǰ��� �������״̬ ��ֹ����
	void axlesHomeMove();//���������
	void axlesHomeZMove();//������ٻ���

	/**************�����˶�********************/
	//������������λ���˶�
	void upsAbsMove(Matrix<double, 6, 1> absL,//��֧����Ҫ�˶����ľ���λ��(��Ը����ԭ��) ��λmm
					double vel);//�������ٶ� ��λmm/s

	//�������� ��ƽ̨�����˶�
	void upsHomeMove(Matrix<double, 6, 1> absL,//��֧����Ҫ�˶����ľ���λ��(��Ը����ԭ��) ��λmm
					double vel);//����������ٶ� ��λmm/s

	//�������������㶯 Jģʽ
	void upsJogJMove(Matrix<double, 6, 1> tarAxlesL_norm,//�������� 
		Matrix<double, 6, 1> speed);//�˶��ٶ�

	//�������������㶯 Linearģʽ
	void upsJogLinearMove(Matrix<double, 6, 1> moveDirection,//�������� �˶�����
					double speed,//�˶��ٶ�
					int flag);//flag Ϊ1����ƽ�� Ϊ2����ת��
	//�������������㶯 Spline1ģʽ
	void upsJogSpline1Move(Matrix<double, 6, 1> moveDirection,//�������� �˶�����
		double speed,//�˶��ٶ�
		int flag);//flag Ϊ1����ƽ�� Ϊ2����ת��



public slots:
	void on_dataGatherTimer();

};


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
	void getMotorsState(QList<PmacVariable> &pmacVariableRecipe, QList<VectorXd*> pmacVariableList);

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

	//�����������λ���˶�
	void upsIncMove(Matrix<double, 6, 1> incL);


public slots:
	void on_dataGatherTimer();

};


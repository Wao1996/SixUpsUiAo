#include "UpsCalculateThread.h"

extern QReadWriteLock rwLock;

UpsCalculateThread::UpsCalculateThread(QObject *parent): QObject(parent)
{
}

UpsCalculateThread::~UpsCalculateThread()
{
}

void UpsCalculateThread::on_upsCalculateTimer()
{
	
	//�˶�ѧ���� ��ƽ̨��Ծ�ƽ̨ʵʱλ��
	forwardSolution(UPSData::initPosAndAngle_DS, UPSData::curL_norm, UPSData::curPosAndAngle_DS, UPSData::D, UPSData::S);
	//�����˶�ѧ����ĳ�ʼ�������� ��������ʱ��
	UPSData::initPosAndAngle_DS = UPSData::curPosAndAngle_DS;
	//��ƽ̨��Ծ�ƽ̨ʵʱ��α任����
	UPSData::Trans_DS = posAngle2Trans(UPSData::curPosAndAngle_DS);
	//���˶�����ϵ��Զ�ƽ̨ʵʱ��α任����
	UPSData::Trans_setS = UPSData::Trans_DS * UPSData::Trans_setD;
	//����ƽ̨����˶�����ϵ��ʵʱ��α任����ת��Ϊλ������
	UPSData::curPosAndAngle_setS = trans2PosAngle(UPSData::Trans_setS);
	for (int i = 0; i < 6; i++)
	{
		//��ֹUI��ʾʱ ��-0.000��0.000����
		if (UPSData::curPosAndAngle_DS(i) > -0.0005 && UPSData::curPosAndAngle_DS(i) < 0.0005)
		{
			UPSData::curPosAndAngle_DS(i) = 0;
		}

		if (UPSData::curPosAndAngle_setS(i) > -0.0005 && UPSData::curPosAndAngle_setS(i) < 0.0005)
		{
			UPSData::curPosAndAngle_setS(i) = 0;
		}
	}

}

#include "UpsCalculateThread.h"

UpsCalculateThread::UpsCalculateThread(QObject *parent): QObject(parent)
{
}

UpsCalculateThread::~UpsCalculateThread()
{
}

void UpsCalculateThread::on_upsCalculateTimer()
{
	//qDebug() << "UpsCalculateThread thread id:" << QThread::currentThreadId();
	
	//�˶�ѧ���� ��ƽ̨��Ծ�ƽ̨ʵʱλ��
	forwardSolution(UPSData::initPosAndAngle_DS, UPSData::curL_norm, UPSData::curPosAndAngle_DS, UPSData::D, UPSData::S);
	//cout << "UPSData::initPosAndAngle_DS" << endl;
	//cout << UPSData::initPosAndAngle_DS << endl;
	//cout << "UPSData::curL_norm" << endl;
	//cout << UPSData::curL_norm << endl;
	//cout << "UPSData::curPosAndAngle_DS" << endl;
	//cout << UPSData::curPosAndAngle_DS << endl;
	//�����˶�ѧ����ĳ�ʼ�������� ��������ʱ��
	UPSData::initPosAndAngle_DS = UPSData::curPosAndAngle_DS;
	//��ƽ̨��Ծ�ƽ̨ʵʱ��α任����
	UPSData::Trans_DS = posAngle2Trans(UPSData::curPosAndAngle_DS);
	//��ƽ̨����˶�����ϵ��ʵʱ��α任����
	UPSData::Trans_Dset = UPSData::Trans_setS.inverse() * UPSData::Trans_DS;
	//����ƽ̨����˶�����ϵ��ʵʱ��α任����ת��Ϊλ������
	UPSData::curPosAndAngle_Dset = trans2PosAngle(UPSData::Trans_Dset);
	for (int i = 0; i < 6; i++)
	{
		//��ֹUI��ʾʱ ��-0.000��0.000����
		if (UPSData::curPosAndAngle_DS(i) > -0.0005 && UPSData::curPosAndAngle_DS(i) < 0.0005)
		{
			UPSData::curPosAndAngle_DS(i) = 0;
		}

		if (UPSData::curPosAndAngle_Dset(i) > -0.0005 && UPSData::curPosAndAngle_Dset(i) < 0.0005)
		{
			UPSData::curPosAndAngle_Dset(i) = 0;
		}
	}

}

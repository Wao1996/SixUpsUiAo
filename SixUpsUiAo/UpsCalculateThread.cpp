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
	forwardSolution(UPSData::initPosAndAngle, UPSData::curL_norm, UPSData::curPosAndAngle, UPSData::D, UPSData::S);
	UPSData::initPosAndAngle = UPSData::curPosAndAngle;
	for (int i = 0; i < 6; i++)
	{
		//��ֹUI��ʾʱ ��-0.000��0.000����
		if (UPSData::curPosAndAngle(i) > -0.0005 && UPSData::curPosAndAngle(i) < 0.0005)
		{
			UPSData::curPosAndAngle(i) = 0;
		}
	}

}

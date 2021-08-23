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
		//防止UI显示时 在-0.000和0.000跳变
		if (UPSData::curPosAndAngle(i) > -0.0005 && UPSData::curPosAndAngle(i) < 0.0005)
		{
			UPSData::curPosAndAngle(i) = 0;
		}
	}

}

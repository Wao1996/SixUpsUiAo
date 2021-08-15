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

}

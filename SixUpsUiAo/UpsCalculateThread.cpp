#include "UpsCalculateThread.h"

UpsCalculateThread::UpsCalculateThread(QObject *parent): QObject(parent)
{
}

UpsCalculateThread::~UpsCalculateThread()
{
}

void UpsCalculateThread::on_upsCalculateTimer()
{

	forwardSolution(UPSData::initPosAndAngle, UPSData::curL_norm, UPSData::curPosAndAngle, UPSData::D, UPSData::S);
	UPSData::initPosAndAngle = UPSData::curPosAndAngle;

}

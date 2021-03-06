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
	
	//运动学正解 求动平台相对静平台实时位姿
	forwardSolution(UPSData::initPosAndAngle_DS, UPSData::curL_norm, UPSData::curPosAndAngle_DS, UPSData::D, UPSData::S);
	//更新运动学正解的初始搜索条件 减少正解时间
	UPSData::initPosAndAngle_DS = UPSData::curPosAndAngle_DS;
	//求动平台相对静平台实时齐次变换矩阵
	UPSData::Trans_DS = posAngle2Trans(UPSData::curPosAndAngle_DS);
	//求运动坐标系相对动平台实时齐次变换矩阵
	UPSData::Trans_setS = UPSData::Trans_DS * UPSData::Trans_setD;
	//将动平台相对运动坐标系的实时齐次变换矩阵转换为位姿向量
	UPSData::curPosAndAngle_setS = trans2PosAngle(UPSData::Trans_setS);
	for (int i = 0; i < 6; i++)
	{
		//防止UI显示时 在-0.000和0.000跳变
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

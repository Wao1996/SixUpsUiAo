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
	
	//运动学正解 求动平台相对静平台实时位姿
	forwardSolution(UPSData::initPosAndAngle_DS, UPSData::curL_norm, UPSData::curPosAndAngle_DS, UPSData::D, UPSData::S);
	//cout << "UPSData::initPosAndAngle_DS" << endl;
	//cout << UPSData::initPosAndAngle_DS << endl;
	//cout << "UPSData::curL_norm" << endl;
	//cout << UPSData::curL_norm << endl;
	//cout << "UPSData::curPosAndAngle_DS" << endl;
	//cout << UPSData::curPosAndAngle_DS << endl;
	//更新运动学正解的初始搜索条件 减少正解时间
	UPSData::initPosAndAngle_DS = UPSData::curPosAndAngle_DS;
	//求动平台相对静平台实时齐次变换矩阵
	UPSData::Trans_DS = posAngle2Trans(UPSData::curPosAndAngle_DS);
	//求动平台相对运动坐标系的实时齐次变换矩阵
	UPSData::Trans_Dset = UPSData::Trans_setS.inverse() * UPSData::Trans_DS;
	//将动平台相对运动坐标系的实时齐次变换矩阵转换为位姿向量
	UPSData::curPosAndAngle_Dset = trans2PosAngle(UPSData::Trans_Dset);
	for (int i = 0; i < 6; i++)
	{
		//防止UI显示时 在-0.000和0.000跳变
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

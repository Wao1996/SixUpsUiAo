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
	
	bool creatPmacSelect();//选择pmac
	void initPmac();//初始化pmac
	bool downloadFile(QString strFile);

	/*获取状态*/
	int getNegLimitState(int);//获取负限位状态
	int getPosLimitState(int);//获取正限位状态
	int getHomeCompleteState(int);//获取电机回零状态
	double getCurLengths(int);//获取支链长度
	
	/*单轴运动控制*/
	void jogDisp(int num, double disp);//距离点动
	void jogPosContinuously(int num);//持续+运动
	void jogNegContinuously(int num);//持续-运动
	void jogStop();//所有轴电机停止 同时也是开启使能
	void jogStop(int num);//某轴电机停止
	void setJogSpeed(int num, double speed);//设置手动运动速度
	void setServoOff();//所有电机失去使能

	/*所有轴归零位*/
	void setAllAxleHome();//所有轴回零
	void setALLaxleHomez();//所有轴假回零
	/*多轴运动*/
	void upsAbsMove(Matrix<double, 6, 1> absL);//并联机构绝对位置运动
	void upsIncMove(Matrix<double, 6, 1> incL);//并联机构相对位置运动
public slots:
	void on_dataGatherTimer();
public:
	PCOMMSERVERLib::PmacDevice *Pmac0;
	int pDeviceNumber;
	bool pbSuccess_select;
	bool pbSuccess_open;
	bool pbSucess_download;
	QString pAnswer;
	bool bAddLF;
	int pstatus;
};


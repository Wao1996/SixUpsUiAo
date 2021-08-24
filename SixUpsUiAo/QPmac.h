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

	QList<PmacVariable> pmacVariableRecipe; //将需要采集的数据类型按顺序放置到QList中
	QList<VectorXd *>	pmacVariableList;//将采集到的数据将按顺序填充到这里

	bool creatPmacSelect();//选择pmac
	void initPmac();//初始化pmac
	bool downloadFile(QString strFile);//下载程序

	/*获取电机状态及变量*/
	QString creatPmacVariableCommand(QList<PmacVariable> &pmacVariableRecipe);//创建获取pmac变量在线命令的字符串
	void getPmacVariable(QList<PmacVariable> &pmacVariableRecipe, QString strCommand, QList<VectorXd*> pmacVariableList);

	/*设置P变量*/
	void setPvariable(int p, double data);
	
	void enablePLC(int num);//启动PLC
	void disablePLC(int num);//关闭PLC
	/*单轴运动控制*/
	void jogDisp(int num, double disp);//距离点动
	void jogPosContinuously(int num);//持续+运动
	void jogNegContinuously(int num);//持续-运动
	void jogStop();//所有轴电机停止 同时也是开启使能
	void jogStop(int num);//某轴电机停止
	void setJogSpeed(int num, double speed);//设置手动运动速度
	void setServoOff();//所有电机失去使能

	/***********所有轴归零位*****************/
	void setHomeCompleteZero();//轴回零开始前清空 回零完成状态 防止误判
	void axlesHomeMove();//所有轴回零
	void axlesHomeZMove();//所有轴假回零

	/**************多轴运动********************/
	//并联机构绝对位置运动
	void upsAbsMove(Matrix<double, 6, 1> absL,//各支链需要运动到的绝对位置(相对各轴的原点) 单位mm
					double vel);//进给轴速度 单位mm/s

	//并联机构 动平台回零运动
	void upsHomeMove(Matrix<double, 6, 1> absL,//各支链需要运动到的绝对位置(相对各轴的原点) 单位mm
					double vel);//回零进给周速度 单位mm/s

	//并联机构长按点动 J模式
	void upsJogJMove(Matrix<double, 6, 1> tarAxlesL_norm,//并联机构 
		Matrix<double, 6, 1> speed);//运动速度

	//并联机构长按点动 Linear模式
	void upsJogLinearMove(Matrix<double, 6, 1> moveDirection,//并联机构 运动方向
					double speed,//运动速度
					int flag);//flag 为1则是平动 为2则是转动
	//并联机构长按点动 Spline1模式
	void upsJogSpline1Move(Matrix<double, 6, 1> moveDirection,//并联机构 运动方向
		double speed,//运动速度
		int flag);//flag 为1则是平动 为2则是转动



public slots:
	void on_dataGatherTimer();

};


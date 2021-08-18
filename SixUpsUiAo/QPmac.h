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

	QList<PmacVariable> pmacVariableRecipe;//将需要采集的数据类型按顺序放置
	QList<VectorXd *>		pmacVariableList;//才到的数据将按顺序填充到这里

	bool creatPmacSelect();//选择pmac
	void initPmac();//初始化pmac
	bool downloadFile(QString strFile);//下载程序

	/*获取状态*/
	int getNegLimitState(int);//获取某一个轴的负限位状态
	void getNegLimitState(VectorXd & negLimitState);//直接获取所有轴的负限位状态
	int getPosLimitState(int);//获取某一个轴的正限位状态
	void getPosLimitState(VectorXd & posLimitState);//直接获取所有轴的正限位状态
	int getHomeCompleteState(int);//获取某一个轴的回零状态
	void getHomeCompleteState(VectorXd &axleHomeCompleteState);//直接获取所有轴的电机回零
	double getCurLengths(int);//获取某一个轴的当前支链长度
	void getCurLengths(VectorXd &curLengths);//直接获取所有轴的当前支链长度
	/*获取电机状态*/
	void getMotorsState(QList<PmacVariable> &pmacVariableRecipe, QList<VectorXd*> pmacVariableList);

	/*获取P变量*/
	void getPVariableList(VectorXd &pVariable);
	/*设置P变量*/
	void setPvariable(int p, double data);

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
	void upsHomeMove(Matrix<double, 6, 1> absL);//并联机构 动平台回零运动

public slots:
	void on_dataGatherTimer();

};


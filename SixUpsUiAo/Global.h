#pragma once
#include <QtCore>
#include <QReadWriteLock>
#include "AlgorithmFor6UPS.h"

class GlobalSta;
class PmacData;
class LazerData;
class UPSData;
class SingleJogData;

enum PmacVariable
{
	NEGLIMITSTATE = 1,          //轴负限位
	POSLIMITSTATE = 2,          //轴正限位
	AXLEHOMECOMPLETESTATE = 3,  //轴回零完成位
	CURLENGTHSMM = 4,           //轴杆长
	PVARIABLE = 5,              //P变量
	ORIGINSTATE = 6,            //原点开关
	CURFORCE =7,				//力
};

class GlobalSta 
{
public:
	//数据地址
	static QString dataFile;
	//设备状态
	static bool pmacIsConnected;//PMAC
	static bool pmacIsInitialed;
	static bool axlesIshome;//所有轴已经回零位
	static bool upsIsHome;//并联机构动平台回零位

	static bool handWheelIsOpened;//手轮
	static bool clinometerIsConnected;//倾角仪

	static bool lazerTrackerIsConnected;//激光跟踪仪
	//线程状态
	static bool tcpServerQthreadIsSarted;//tcp for unity线程
	static bool pmacQthreadIsSarted;//pmac线程
	//仿真模式
	static bool isOnlineSim;//是在线模式
};
class PmacData
{
public:

	static QString pmacGetVariableCommand;//获取pmac变量在线命令的字符串
	static int numL;//一共有多少根支链
	static int cts2mm;//多少个脉冲代表电动缸走1mm
	static VectorXd negLimitState;//负限位开关状态
	static VectorXd posLimitState;//正限位开关状态
	static VectorXd originState;  //原点开关状态
	static VectorXd axleHomeCompleteState;//pmac中 各轴回零完成状态
	static VectorXd curLengthsMM;//pmac中显示的杆长  (相对零位的杆长) 单位mm
	static VectorXd curForce;//支链力
	static VectorXd pVariable;//P变量列表
	
};

class SingleJogData
{
public:

	static double jogSpeed;//单轴点动速度
	static VectorXd jogInc;//单轴点动距离增量

};
class LazerData
{
public:
	
};
class UPSData
{
public:

	//并联机构运动参数
	static double multiSpeed;							//多轴联动 运动速度 单位mm/
	static int multiSpeedID;							//多轴联动 运动速度ID
	static double multiJogTranslationSpeed;				//多轴点动 平动运动速度 单位mm/s
	static double multiJogTranslationStep;				//多轴点动 平动运动步长 单位mm
	static double multiJogRotateSpeed;					//多轴点动 转动运动速度 单位°/s
	static double multiJogRotateStep;					//多轴点动 转动运动步长 单位°
	static Matrix<double, 6, 1> multiJogMoveDirection;	//多轴点动 运动方向
	static int multiJogMoveDirectionID;					//多轴点动 运动方向ID
	static double multiJogMoveStep;						//多轴长按点动 运动步长

	//建立坐标系
	static MatrixXd circlePt;				//3*n(圆心拟合数据点个数) 
	static Matrix <double, 3, 1> circleC;	//拟合的圆心坐标
	static double r;						//拟合的半径

	//运动原点设置相关
	static Vector3d XYZ_setD;				//运动原点在动坐标系下的位置
	static Matrix<double, 3, 6> D_set;		//修改运动原点后 动平台铰链点在运动坐标系下的坐标
	static Matrix4d Trans_setS;				//运动坐标系相对静坐标系的齐次变换矩阵

	static int n_D;							//测量的动平台靶标点个数
	static int n_S;							//测量的静平台靶标点个数
	static MatrixXd Q_DM;					//测量坐标系下 动平台靶标点坐标 3*n_S
	static MatrixXd Q_SM;					//测量坐标系下 静平台靶标点坐标 3*n_S
	static Matrix<double, 3, 6> D_M;		//测量坐标系下 动平台铰链点坐标
	static Matrix<double, 3, 6> S_M;		//测量坐标系下 静平台铰链点坐标
	static Matrix4d Trans_DM;				//动平台坐标系相对测量坐标系齐次变换矩阵(平台标定时用，其余时候不用)
	static Matrix4d Trans_SM;				//静平台坐标系相对测量坐标系齐次变换矩阵(平台标定时用，其余时候不用)
	static Matrix4d Trans_DS;				//动平台坐标系相对静平台坐标系齐次变换矩阵
	static Matrix4d Trans_setD;				//运动坐标系相对动平台齐次变换矩阵
				

	//结构参数与初始杆长标定
	static MatrixXd Q_DD;						//3*n_D_struct 动平台靶标点在动系下坐标
	static MatrixXd Q_SS;						//3*n_S_struct 静平台靶标点在静系下坐标
	static MatrixXd	Q_GD;						//工装靶标点在动系下坐标

	static Matrix<double, 3, 6> D;				//动坐标系下 动平台铰链点坐标(实际值)
	static Matrix<double, 3, 6> S;				//静坐标系下 静平台铰链点坐标(实际值)
	static Matrix<double, 3, 6> D_theoretical;	//动坐标系下 动平台铰链点坐标(理论值) 用于确定动平台坐标系原点
	static Matrix<double, 3, 6> S_theoretical;	//静坐标系下 静平台铰链点坐标(理论值) 用于确定静平台坐标系原点
	static Matrix<double, 6, 1> initL_norm;		//并联机构各支链处于零位时 实际的杆长 单位 mm 


	//实时位姿
	static Matrix<double, 6, 1> curPosAndAngle_setS;	//运动坐标系相对静平台 实时位姿 单位 mm °
	static Matrix<double, 6, 1> curPosAndAngle_DS;		//动平台相对静平台     实时位姿 单位 mm °
	static Matrix<double, 6, 1> curPosAndAngle_setD;	//运动坐标系相对动平台 实时位姿 单位 mm °
	//目标位姿
	static Matrix<double, 6, 1> tarPosAndAngle_Dset; 	//动平台相对运动坐标系的目标位姿 xyzabc（按照该位姿执行运动） 单位 mm °
	static Matrix<double, 6, 1> tarPosAndAngle_setS;	//运动坐标系相对静平台的目标位姿 xyzabc（按照该位姿执行运动） 单位 mm °
	
	static Matrix<double, 6, 1> prsPosAndAngle_setS;	//长按点动按下时的运动坐标系相对静平台
	static Matrix<double, 6, 1>	homePosAndAngle_DS;		//并联机构 动平台相对静平台的零位位姿   单位 mm °
	static Matrix<double, 6, 1> initPosAndAngle_DS;		//动平台相对静平台 正解初始位姿 单位 mm °
	
	static Matrix<double, 6, 1> tarL_norm;				//由目标位姿反解得到的目标铰点杆长(带有norm后缀的表示从虎克铰到球铰点的长度) 单位mm
	static Matrix<double, 6, 1> tarAxlesL_norm;			//得到目标杆长后 每个轴相对自身零位所需要的移动的距离 单位mm
	static Matrix<double, 6, 1> lastAxlesL_norm;		//上一步轴长
	static Matrix<double, 6, 1> curL_norm;				//由PMAC值换算得到的实时铰点杆长 单位mm
	static Matrix<double, 6, 1> lastL_norm;				//上一步的铰点杆长
	

	static Matrix<double, 6, 1> realPosAndAngleByQD;	//激光跟踪仪测量靶标点计算得到的实际位姿
	static Matrix<double, 6, 1> realPosAndAngleByMPt;	//激光跟踪仪测量待测点计算得到的实际位姿
	static Matrix<double, 6, 1> compTarPosAndAngleByQD; //通过靶标点实际位姿补偿后得到的新目标位姿
	static Matrix<double, 6, 1> compTarPosAndAngleByMPt;//通过待测点实际位姿补偿后得到的新目标位姿

	//待测点组相关
	static int n_MPt;					//待测点组点的个数
	static MatrixXd MPt_M;				//3*n_MPt 待测点组在测量系系下坐标
	static MatrixXd MPt_D;				//3*n_MPt待测点组在动系下的坐标(较为稳定)
	static MatrixXd tarMPt_M;			//目标待测点组在测量坐标系下的坐标
	static Matrix<double, 6, 1> tarPosAndAngleByMPt;//基于待测点的并联平台目标位姿
};
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
};

class GlobalSta 
{
public:
	//皮肤路径
	static QString skinPath;
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
	//运动参数
	static double multiSpeed;//多轴联动 运动速度 单位mm/
	static double multiJogTranslationSpeed;//多轴点动 平动运动速度 单位mm/s
	static double multiJogTranslationStep;//多轴点动 平动运动步长 单位mm
	static double multiJogRotateSpeed;//多轴点动 转动运动速度 单位°/s
	static double multiJogRotateStep;//多轴点动 转动运动步长 单位°
	static Matrix<double, 6, 1> multiJogMoveDirection;//多轴点动运动方向
	static double multiJogMoveStep;//多轴长按点动 运动步长

	//建立坐标系
	static MatrixXd circlePt;			//3*n(圆心拟合数据点个数) 
	static Matrix <double, 3, 1> circleC;		//拟合的圆心坐标
	static double r;						//拟合的半径
	//动平台坐标系
	static Matrix <double, 3, 1> O_m_D;	//建立的动平台坐标系原点：圆心或跟踪仪测量点
	static Matrix <double, 3, 1> X_m_D;	//第二个测量点为新坐标系x轴 **正** 方向上的一点
	static Matrix <double, 3, 1> XOY_m_D;	//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)
	//静平台坐标系
	static Matrix <double, 3, 1> O_m_S;	//建立的静平台坐标系原点：圆心或跟踪仪测量点
	static Matrix <double, 3, 1> X_m_S;	//第二个测量点为新坐标系x轴 **正** 方向上的一点
	static Matrix <double, 3, 1> XOY_m_S;	//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)

	static Matrix<double, 3, 3> R_SM;	//静平台坐标系相对测量坐标系的关系:旋转矩阵(每次重新放置跟踪仪都会测量计算)
	static Matrix<double, 3, 1> t_SM;	//静平台坐标系相对测量坐标系的关系:平移矩阵

	static Matrix<double, 3, 3> R_DM;	//动平台坐标系相对测量坐标系的关系:旋转矩阵(仅在结构参数标定时使用，其他情况在对应函数中作为中间变量直接计算得到)
	static Matrix<double, 3, 1> t_DM;	//动平台坐标系相对测量坐标系的关系:平移矩阵
	//运动坐标系下静平台下的描述
	static Matrix<double, 3, 1>	O_set_S;//建立的运动坐标系原点，在静坐标系下的位置(默认与静坐标系相同)
	static Matrix<double, 3, 1> X_set_S;
	static Matrix<double, 3, 1> XOY_set_S;
	static Matrix<double, 3, 3> R_SetS;//运动坐标系相对静坐标系的关系:旋转矩阵
	static Matrix<double, 3, 1> t_SetS;//运动坐标系相对静坐标系的关系:平移矩阵

	//测量相关
	static int n_D;						//测量的动平台靶标点个数
	static MatrixXd Q_DM;				//3*n_D 动平台靶标点在测量系下坐标
	static int n_S;						//测量的静平台靶标点个数
	static MatrixXd Q_SM;				//3*n_S 静平台靶标点在测量系下坐标
	static Matrix<double, 3, 6> D_M;		//动平台铰链点在测量坐标系下坐标
	static Matrix<double, 3, 6> S_M;		//静平台铰链点在测量坐标系下坐标
	//运动坐标系下测量坐标系下的描述
	static Matrix<double, 3, 1>	O_set_M;	   //设定相对运动原点，在测量坐标系下的坐标
	

	//结构参数与初始杆长标定
	static Matrix<double, 3, 6> D;				//动平台铰链点在动坐标系下坐标
	static int n_D_struct;						//动平台上靶标点个数
	static MatrixXd Q_DD;						//3*n_D_struct 动平台靶标点在动系下坐标
	static Matrix<double, 3, 6> S;				//静平台铰链点在静坐标系下坐标
	static int n_S_struct;						//静平台上靶标点个数
	static MatrixXd Q_SS;						//3*n_S_struct 静平台靶标点在静系下坐标
	static Matrix<double, 6, 1> initL_norm;		//并联机构各支链处于零位时 实际的杆长 单位 mm 


	//位姿杆长数据
	static Matrix<double, 6, 1>	homePosAndAngle;		//并联机构 平台零位位姿   单位 mm °
	static Matrix<double, 6, 1> tarPosAndAngle;			//目标位姿 xyzabc（按照该位姿执行运动） 单位 mm °
	static Matrix<double, 6, 1> prsPosAndAngle;			//长按点动按下时的位姿
	static Matrix<double, 6, 1> tarL_norm;				//有目标位姿反解得到的目标杆长 单位mm
	static Matrix<double, 6, 1> tarAxlesL_norm;			//得到目标杆长后 每个轴相对自身零位所需要的移动的距离 单位mm
	static Matrix<double, 6, 1> lastAxlesL_norm;		//上一步轴长
	static Matrix<double, 6, 1> curL_norm;				//由PMAC值换算得到的实时杆长 单位mm
	static Matrix<double, 6, 1> lastL_norm;				//上一步的杆长
	static Matrix<double, 6, 1> initPosAndAngle;		//正解初始位姿 单位 mm °
	static Matrix<double, 6, 1> curPosAndAngle;		    //正解实时位姿 单位 mm °
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
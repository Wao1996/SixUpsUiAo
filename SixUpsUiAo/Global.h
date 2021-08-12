#pragma once
#include <QtCore>
#include <QReadWriteLock>
#include "AlgorithmFor6UPS.h"

class GlobalSta;
class PmacData;
class LazerData;
class UPSData;



class GlobalSta 
{
public:
	//皮肤路径
	static QString skinPath;
	//设备状态
	static bool pmacIsConnected;//PMAC
	static bool pmacIsInitialed;

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
	static double multiSpeed;//多轴运动速度
	static VectorXi negLimitState;//负限位状态
	static VectorXi posLimitState;//正限位状态

};

class LazerData
{
public:

};
class UPSData
{
public:
	//建立坐标系
	static MatrixXd circlePt;			//3*n(圆心拟合数据点个数) 
	static Matrix <double, 3, 1> circleC;		//拟合的圆心坐标
	static double r;						//拟合的半径
	//动平台坐标系
	static Matrix <double, 3, 1> O_m_D;	//建立的坐标系原点：圆心或跟踪仪测量点
	static Matrix <double, 3, 1> X_m_D;	//第二个测量点为新坐标系x轴 **正** 方向上的一点
	static Matrix <double, 3, 1> XOY_m_D;	//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)
	//静平台坐标系
	static Matrix <double, 3, 1> O_m_S;	//建立的坐标系原点：圆心或跟踪仪测量点
	static Matrix <double, 3, 1> X_m_S;	//第二个测量点为新坐标系x轴 **正** 方向上的一点
	static Matrix <double, 3, 1> XOY_m_S;	//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)

	static Matrix<double, 3, 3> R_SM;	//静平台坐标系相对测量坐标系的关系:旋转矩阵(每次重新放置跟踪仪都会测量计算)
	static Matrix<double, 3, 1> t_SM;	//静平台坐标系相对测量坐标系的关系:平移矩阵


	static Matrix<double, 3, 3> R_DM;	//动平台坐标系相对测量坐标系的关系:旋转矩阵(仅在结构参数标定时使用，其他情况在对应函数中作为中间变量直接计算得到)
	static Matrix<double, 3, 1> t_DM;	//动平台坐标系相对测量坐标系的关系:平移矩阵

	//测量相关
	static int n_D;						//测量的动平台靶标点个数
	static MatrixXd Q_DM;				//3*n_D 动平台靶标点在测量系下坐标
	static int n_S;						//测量的静平台靶标点个数
	static MatrixXd Q_SM;				//3*n_S 静平台靶标点在测量系下坐标
	static Matrix<double, 3, 6> D_M;		//动平台铰链点在测量坐标系下坐标
	static Matrix<double, 3, 6> S_M;		//静平台铰链点在测量坐标系下坐标

	//结构参数与初始杆长标定
	static Matrix<double, 3, 6> D;				//动平台铰链点在动坐标系下坐标
	static int n_D_struct;						//动平台上靶标点个数
	static MatrixXd Q_DD;						//3*n_D_struct 动平台靶标点在动系下坐标
	static Matrix<double, 3, 6> S;				//静平台铰链点在静坐标系下坐标
	static int n_S_struct;						//静平台上靶标点个数
	static MatrixXd Q_SS;						//3*n_S_struct 静平台靶标点在静系下坐标
	static Matrix<double, 6, 1> initL_norm;		//初始杆长

	//位姿杆长数据
	static Matrix<double, 6, 1> tarPosAndAngle;		//当前面板输入目标位姿 xyzabc（按照该位姿执行运动）
	static Matrix<double, 6, 1> tarL_norm;			//当前面板输入目标位姿反解得的杆长
	static Matrix<double, 6, 1> curL_norm;			//由PMAC值换算得到的实时杆长
	static Matrix<double, 6, 1> initPosAndAngle;		//正解初始位姿
	static Matrix<double, 6, 1> curPosAndAngle;		//正解实时位姿
	static Matrix<double, 6, 1> realPosAndAngleByQD;		//激光跟踪仪测量靶标点计算得到的实际位姿
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
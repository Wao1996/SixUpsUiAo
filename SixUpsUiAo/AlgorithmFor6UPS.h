#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <Eigen/Dense>
#include <iostream>

using namespace std;
using namespace  Eigen;

//xyzabc姿态转换为旋转矩阵R和平移矩阵t
extern void posAngleToRt(const Matrix<double, 6, 1>& posAndAngle,//xyzabc姿态信息
	Matrix<double, 3, 3>& R,//输出旋转矩阵R
	Matrix<double, 3, 1>& t);//输出平移矩阵t

//旋转矩阵R和平移矩阵t转换为xyzabc姿态
extern void RtToPosAngle(const Matrix<double, 3, 3>& R,//旋转矩阵
	const Matrix<double, 3, 1>& t,//平移矩阵
	Matrix<double, 6, 1>& posAndAngle);//输出姿态信息

//xyzabc姿态转换为齐次矩阵Trans
extern void posAngle2Trans(const Matrix<double, 6, 1>& posAndAngle,//xyzabc姿态信息
	Matrix<double, 4, 4>& Trans); //输出齐次矩阵Trans

//齐次矩阵Trans转换为xyzabc姿态
extern void trans2PosAngle(const Matrix<double, 4, 4>& Trans,//齐次矩阵Trans
	Matrix<double, 6, 1>& posAndAngle);//输出姿态信息
//!建立动静坐标系
/***************************************************************************************/
//空间圆拟合圆心法确定原点
extern void solveOriginalPtByCircle(const MatrixXd &pt,//空间点构成的矩阵 3*num
	Matrix <double,3,1> &originPt,//输出圆心
	double &r);//输出半径

//空间球拟合球心法确定原点
extern void solveOriginalPtBySphere(const MatrixXd &pt,//空间点构成的矩阵 3*num
	Matrix <double, 3, 1> &originPt,//输出圆心
	double &r);//输出半径

//使用激光跟踪仪测量三点，建立直角坐标右手系o(三点尽量远点)
//激光跟踪仪再测的其他点Q_m可以转换到o系坐标:**Q_o = R_om^(-1)*(Q_m-t_om)**->标定结构参数
extern void creatCoordSysGetRt(const Matrix<double, 3, 1>& O_m,//第一个测量点为新坐标系的原点（**使用中心法或直接测得**）
	const Matrix<double, 3, 1>& X_m, //第二个测量点为新坐标系x轴 **正** 方向上的一点
	const Matrix<double, 3, 1>& XOY_m,//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)
	Matrix<double, 3, 3>&R_om,//o相对于m的旋转矩阵
	Matrix<double, 3, 1>&t_om);//o相对于m的平移矩阵

/******************************************************************************************/

//!运动学反解
extern void inverseSolution(const Matrix<double, 6, 1>& posAndAngle,//动平台相对静平台位姿xyzabc
	Matrix<double, 6, 1>& L_norm,//解得的杆长
	const Matrix<double, 3, 6>& D,//动平台铰链点在动坐标系下坐标,结构参数
	const Matrix<double, 3, 6>& S);//静台铰链点在静平台坐标系下坐标,结构参数

//求F(X,L)位姿杆长约束方程
extern void solveF(const Matrix<double, 6, 1>& posAndAngle,//xyzabc姿态信息
	const Matrix<double, 6, 1>& L_norm,//杆长
	const Matrix<double, 3, 6>& D,//动平台铰链点在动坐标系下坐标,结构参数
	const Matrix<double, 3, 6>& S,//静台铰链点在静平台坐标系下坐标,结构参数
	Matrix<double, 6, 1>& F);//得出的结果F 6*1列向量

//求F(X,L)对X的偏导数,X这里表示位姿
extern void solvedFdx(const Matrix<double, 6, 1>& posAndAngle,//xyzabc姿态信息
	const Matrix<double, 6, 1>& L_norm,//杆长
	const Matrix<double, 3, 6>& D,//动平台铰链点在动坐标系下坐标,结构参数
	const Matrix<double, 3, 6>& S,//静台铰链点在静平台坐标系下坐标,结构参数
	double delta,//求偏导时微小变动量
	Matrix<double, 6, 6>& dFdx);//得出的结果dF/dX 6*6矩阵


//SVD法求解两坐标系中对应点集的变换矩阵,用于求解R_SM,t_SM和R_DM,t_DM
extern void rigidMotionSVDSolution(int n,//三维点集Q中点的个数
	const MatrixXd& Q_o, //点集Q在o自身坐标系中的坐标，3行n列
	const MatrixXd& Q_m, //点集Q在m测量坐标系中的坐标，3行n列
	Matrix<double, 3, 3>&R_om,//o相对于m的旋转矩阵
	Matrix<double, 3, 1>&t_om);//o相对于m的平移矩阵

//求解并联机构的雅克比矩阵
extern void solveJacobi(const Matrix<double, 6, 1>& posAndAngle, //动平台相对静平台位姿xyzabc
	Matrix <double, 6, 6>& jacobi,//解得的雅克比矩阵6*6
	const Matrix<double, 3, 6>& D, //动平台铰链点在动坐标系下坐标, 结构参数
	const Matrix<double, 3, 6>& S);//静台铰链点在静平台坐标系下坐标,结构参数

//!运动学正解
extern void forwardSolution(const Matrix<double, 6, 1>& initPosAndAngle,//动平台初始位姿
	const Matrix<double, 6, 1>& targL_norm,//目标的新杆长
	Matrix<double, 6, 1>& targPosAndAngle,//正解得到的位姿
	const Matrix<double, 3, 6>& D, //动平台铰链点在动坐标系下坐标,结构参数
	const Matrix<double, 3, 6>& S);//静台铰链点在静平台坐标系下坐标,结构参数

//!**每次使用激光跟踪仪需要先找到静坐标系和激光跟踪仪测量坐标系的关系:R_SM、t_SM **
//sixUPSRobotCalculate::rigidMotionSVDSolution(n_S, Q_SS, Q_SM, R_SM, t_SM);


//!标定(求解)初始杆长
extern void calibrateInitLength(int n_D,//动平台靶标点的个数
	Matrix<double, 6, 1>& initL_norm,//求得的初始杆长
	const MatrixXd& Q_DD, //动平台靶标点在自身坐标系下坐标，3行n_D列
	const MatrixXd& Q_DM, //动平台靶标点在测量坐标系中的坐标，3行n_D列
	const Matrix<double, 3, 3>&R_SM,//静平台相对测量系的旋转矩阵,
	const Matrix<double, 3, 1>&t_SM,//平移,
	const Matrix<double, 3, 6>& D, //动平台铰链点在动坐标系下坐标,结构参数
	const Matrix<double, 3, 6>& S);//静台铰链点在静平台坐标系下坐标,结构参数


//!利用激光跟踪仪和靶标点求解实际位姿，若将n_D、Q_DD、Q_DM换为n_MPt、MPt_D、MPt_M即是通过待测点计算实际位姿
extern void solveRealPosAndAngleByLazer(int n_D,//动平台靶标个数
	const Matrix<double, 3, 3>&R_SM,//静平台相对测量系的旋转矩阵,
	const Matrix<double, 3, 1>&t_SM,//平移,
	const MatrixXd &Q_DD,//3*n_D 动平台靶标点在动系下坐标,
	const MatrixXd &Q_DM,//3*n_D 动平台靶标点在测量系下坐标,
	Matrix<double, 6, 1> &realPosAndAngle); //输出实际位姿

/****************************************************************************************/
//!根据并联机器人上装配件的待测点组的坐标(激光跟踪仪测得的)计算需要输入的平台目标位姿
//以下分两个函数写目的是使待测点组合动系的关系更灵活，若发生改变可以重新计算，若未变则只需计算一次
//求待测点组在动系下的坐标
extern void solveMpt_D(int n_MPt,//待测点组中点的个数
	int n_D,//动平台靶标个数
	const MatrixXd &Q_DD,//3*n_D 动平台靶标点在动系下坐标,
	const MatrixXd &Q_DM,//3*n_D 动平台靶标点在测量系系下坐标,
	const MatrixXd &MPt_M,//3*n_MPt 待测点组在测量系系下坐标,
	MatrixXd &MPt_D);//3*n_MPt 输出待测点组在动系下的坐标

//求基于待测点的并联平台目标位姿,即求R_tarDS,t_tarDS后转为tarPosAndAngleByMPt
extern void solveTarPosAndAnglebyMeasuredPt(int n_MPt,//待测点组中点的个数
	const MatrixXd &MPt_D,//待测点组在动系下的坐标
	const MatrixXd &tarMPt_M,//!目标待测点组在测量坐标系下的坐标
	const Matrix<double, 3, 3>&R_SM,//静平台相对测量系的旋转矩阵,
	const Matrix<double, 3, 1>&t_SM,//平移,
	Matrix<double, 6, 1> &tarPosAndAngleByMPt);
/********************************************************************************************************/

//求解补偿位姿(新的要输入的位姿)
//使用位姿误差变的换矩阵计算新的位姿
extern void solveCompensationPosAndAngle(const Matrix<double, 6, 1> &inputPosAndAngle,//面板输入的目标位姿
	const Matrix<double, 6, 1> &realPosAndAngle,//测得的当前实际位姿
	Matrix<double, 6, 1> &posAndAngleAfterComp);//输出补偿计算后新的目标位姿

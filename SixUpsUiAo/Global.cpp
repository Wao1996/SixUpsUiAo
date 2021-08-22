#include "Global.h"
/************************GlobalSta start*************************************************/

QString GlobalSta::skinPath = "./other/qss/flatwhite.css";
//"./other/qss/psblack.css" //黑
//"./other/qss/flatwhite.css"//白
//"./other/qss/lightblue.css"//蓝
 bool GlobalSta::pmacIsConnected = false;
 bool GlobalSta::pmacIsInitialed = false;
 bool GlobalSta::axlesIshome = false;
 bool GlobalSta::upsIsHome = false;

 bool GlobalSta::handWheelIsOpened = false;
 bool GlobalSta::clinometerIsConnected = false;

 bool GlobalSta::lazerTrackerIsConnected = false;
 //线程状态
 bool GlobalSta::tcpServerQthreadIsSarted = false;//tcp for unity线程
 bool GlobalSta::pmacQthreadIsSarted = false;//ppmac线程

 //仿真模式
 bool GlobalSta::isOnlineSim = false;//是在线模式

 /***********************PmacData ************************************************/
 int PmacData::numL = 6; 
 int PmacData::cts2mm = 20480;
 VectorXd PmacData::negLimitState = VectorXd::Zero(6);//负限位开关状态
 VectorXd PmacData::posLimitState = VectorXd::Zero(6);//正限位开关状态
 VectorXd PmacData::originState = VectorXd::Zero(6);//原点开关状态
 VectorXd PmacData::axleHomeCompleteState = VectorXd::Zero(6);//pmac中 各轴回零完成状态
 VectorXd PmacData::curLengthsMM = VectorXd::Zero(6);//杆长
 VectorXd PmacData::pVariable = VectorXd::Zero(20);//P变量
 
  /***********************SingleJogData ************************************************/
 double SingleJogData::jogSpeed = 0.1;//点动速度
 VectorXd SingleJogData::jogInc = VectorXd::Zero(6);//点动距离向量

 /***********************UPSData start************************************************/
//运动参数
 double UPSData::multiSpeed = 1;//多轴联动 进给轴速度 单位mm/s
 double UPSData::multiJogTranslationSpeed = 0.1;//多轴点动 平动运动速度 单位mm/s
 double UPSData::multiJogTranslationStep = 0.1;//多轴点动 平动运动步长 单位mm
 double UPSData::multiJogRotateSpeed = 0.01;//多轴点动 转动运动速度 单位°/s
 double UPSData::multiJogRotateStep = 0.01;//多轴点动 转动运动步长 单位°
 Matrix<double, 6, 1> UPSData::multiJogMoveDirection = MatrixXd::Zero(6, 1);
 
//建立坐标系
 MatrixXd UPSData::circlePt;//3*n(圆心拟合数据点个数) 
 Matrix <double, 3, 1> UPSData::circleC = MatrixXd::Zero(3, 1);//拟合的圆心坐标
 double UPSData::r = 0;						//拟合的半径
 //动平台坐标系
 Matrix <double, 3, 1> UPSData::O_m_D = MatrixXd::Zero(3, 1);//建立的坐标系原点：圆心或跟踪仪测量点
 Matrix <double, 3, 1> UPSData::X_m_D = MatrixXd::Zero(3, 1);//第二个测量点为新坐标系x轴 **正** 方向上的一点
 Matrix <double, 3, 1> UPSData::XOY_m_D = MatrixXd::Zero(3, 1);//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)
 //静平台坐标系
 Matrix <double, 3, 1> UPSData::O_m_S = MatrixXd::Zero(3, 1);//建立的坐标系原点：圆心或跟踪仪测量点
 Matrix <double, 3, 1> UPSData::X_m_S = MatrixXd::Zero(3, 1);//第二个测量点为新坐标系x轴 **正** 方向上的一点
 Matrix <double, 3, 1> UPSData::XOY_m_S = MatrixXd::Zero(3, 1);//第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**)

 Matrix<double, 3, 3> UPSData::R_SM = MatrixXd::Zero(3, 3);	//静平台坐标系相对测量坐标系的关系:旋转矩阵(每次重新放置跟踪仪都会测量计算)
 Matrix<double, 3, 1> UPSData::t_SM = MatrixXd::Zero(3, 1);	//静平台坐标系相对测量坐标系的关系:平移矩阵

 Matrix<double, 3, 3> UPSData::R_DM = MatrixXd::Zero(3, 3);	//动平台坐标系相对测量坐标系的关系:旋转矩阵(仅在结构参数标定时使用，其他情况在对应函数中作为中间变量直接计算得到)
 Matrix<double, 3, 1> UPSData::t_DM = MatrixXd::Zero(3, 1);	//动平台坐标系相对测量坐标系的关系:平移矩阵
 //
 Matrix<double, 3, 1> UPSData::O_set_S = MatrixXd::Zero(3, 1);//设定相对运动原点，在静坐标系下的位置(默认与静坐标系相同)
 Matrix<double, 3, 1> UPSData::X_set_S = MatrixXd::Zero(3, 1);
 Matrix<double, 3, 1> UPSData::XOY_set_S = MatrixXd::Zero(3, 1);
 Matrix<double, 3, 3> UPSData::R_SetS = MatrixXd::Zero(3, 3);//运动坐标系相对静坐标系的关系:旋转矩阵
 Matrix<double, 3, 1> UPSData::t_SetS = MatrixXd::Zero(3, 1);//运动坐标系相对静坐标系的关系:平移矩阵
//测量相关
 int UPSData::n_D;						//测量的动平台靶标点个数
 MatrixXd UPSData::Q_DM;				//3*n_D 动平台靶标点在测量系下坐标
 int UPSData::n_S;						//测量的静平台靶标点个数
 MatrixXd UPSData::Q_SM;				//3*n_S 静平台靶标点在测量系下坐标
 Matrix<double, 3, 6> UPSData::D_M = MatrixXd::Zero(3, 6);		//动平台铰链点在测量坐标系下坐标
 Matrix<double, 3, 6> UPSData::S_M = MatrixXd::Zero(3, 6);		//静平台铰链点在测量坐标系下坐标
 Matrix<double, 3, 1> UPSData::O_set_M = MatrixXd::Zero(3, 1);  //设定相对运动原点，在测量坐标系下坐标
 
 //结构参数与初始杆长
 Matrix<double, 3, 6> UPSData::D = MatrixXd::Zero(3, 6);				//动平台铰链点在动坐标系下坐标
 int UPSData::n_D_struct = 6;										//动平台上靶标点个数
 MatrixXd UPSData::Q_DD = MatrixXd::Zero(3, 6);						//3*n_D_struct 动平台靶标点在动系下坐标
 Matrix<double, 3, 6> UPSData::S = MatrixXd::Zero(3, 6);				//静平台铰链点在静坐标系下坐标
 int UPSData::n_S_struct = 6;										//静平台上靶标点个数
 MatrixXd UPSData::Q_SS = MatrixXd::Zero(3, 6);						//3*n_S_struct 静平台靶标点在静系下坐标
 Matrix<double, 6, 1> UPSData::initL_norm = MatrixXd::Zero(6, 1);	//初始杆长

 //当前位姿杆长数据
 
 Matrix<double, 6, 1> UPSData::homePosAndAngle = MatrixXd::Zero(6, 1);		//并联机构 平台零位位姿  单位 mm °
 Matrix<double, 6, 1> UPSData::tarPosAndAngle = MatrixXd::Zero(6, 1);		//目标位姿 xyzabc（按照该位姿执行运动） 单位 mm °
 Matrix<double, 6, 1> UPSData::tarL_norm = MatrixXd::Zero(6, 1);			//有目标位姿反解得到的目标杆长 单位mm
 Matrix<double, 6, 1> UPSData::tarLengths = MatrixXd::Zero(6, 1);           //得到目标杆长后 每个轴相对自身零位所需要的移动的距离 单位mm
 Matrix<double, 6, 1> UPSData::tarLengthsLast = MatrixXd::Zero(6, 1);
 Matrix<double, 6, 1> UPSData::curL_norm = MatrixXd::Zero(6, 1);			//由PMAC值换算得到的实时杆长 单位mm
 Matrix<double, 6, 1> UPSData::initPosAndAngle = MatrixXd::Zero(6, 1);		//正解初始位姿
 Matrix<double, 6, 1> UPSData::curPosAndAngle = MatrixXd::Zero(6, 1);		//正解实时位姿
 Matrix<double, 6, 1> UPSData::realPosAndAngleByQD = MatrixXd::Zero(6, 1);	//激光跟踪仪测量靶标点计算得到的实际位姿
 Matrix<double, 6, 1> UPSData::realPosAndAngleByMPt = MatrixXd::Zero(6, 1);	//激光跟踪仪测量待测点计算得到的实际位姿
 Matrix<double, 6, 1> UPSData::compTarPosAndAngleByQD = MatrixXd::Zero(6, 1); //通过靶标点实际位姿补偿后得到的新目标位姿
 Matrix<double, 6, 1> UPSData::compTarPosAndAngleByMPt = MatrixXd::Zero(6, 1);//通过待测点实际位姿补偿后得到的新目标位姿

 //待测点组相关
 int UPSData::n_MPt;					//待测点组点的个数
 MatrixXd UPSData::MPt_M;				//3*n_MPt 待测点组在测量系系下坐标
 MatrixXd UPSData::MPt_D;				//3*n_MPt待测点组在动系下的坐标(较为稳定)
 MatrixXd UPSData::tarMPt_M;			//目标待测点组在测量坐标系下的坐标
 Matrix<double, 6, 1> UPSData::tarPosAndAngleByMPt = MatrixXd::Zero(6, 1);//基于待测点的并联平台目标位姿

 /***********************UPSData end************************************************/
#include "AlgorithmFor6UPS.h"

void posAngleToRt(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 3, 3>& R, Matrix<double, 3, 1>& t)
{
	//这里也可用Eigen函数toRotationMatrix
	//动平台位置，姿态参数(相对于静系)，a,b,c为绕静止坐标系x,y,z旋转的角度(°)
	double x = posAndAngle(0);
	double y = posAndAngle(1);
	double z = posAndAngle(2);
	double a = posAndAngle(3) / 180 * M_PI;
	double b = posAndAngle(4) / 180 * M_PI;
	double c = posAndAngle(5) / 180 * M_PI;
	R << cos(c)*cos(b), cos(c)*sin(b)*sin(a) - sin(c)*cos(a), cos(c)*sin(b)*cos(a) + sin(c)*sin(a),
		sin(c)*cos(b), sin(c)*sin(b)*sin(a) + cos(a)*cos(c), sin(c)*sin(b)*cos(a) - cos(c)*sin(a),
		-sin(b), cos(b)*sin(a), cos(b)*cos(a);
	t << x, y, z;
}

void RtToPosAngle(const Matrix<double, 3, 3>& R, const Matrix<double, 3, 1>& t, Matrix<double, 6, 1>& posAndAngle)
{
	//注：关于绕定坐标系的xyz的rpy角和绕动系的zyx欧拉角
	//由于变换矩阵相乘时绕定系为左乘，绕动系为右乘，因此上述两变换矩阵实际上是相同的均为R(a)*R(b)*R(c)
	Matrix <double, 3, 1> eulerAngle_ZYX = R.eulerAngles(2, 1, 0);
	Matrix <double, 3, 1> Angle_XYZ;
	Angle_XYZ << eulerAngle_ZYX(2), eulerAngle_ZYX(1), eulerAngle_ZYX(0);
	posAndAngle << t, Angle_XYZ / M_PI * 180;
}

void posAngle2Trans(const Matrix<double, 6, 1>& posAndAngle,/*xyzabc姿态信息 */ Matrix<double, 4, 4>& Trans)
{
	Matrix<double, 3, 3> R;
	Matrix<double, 3, 1> t;
	posAngleToRt(posAndAngle, R, t);
	Trans.block<3, 3>(0, 0) = R;
	Trans.block<3, 1>(0, 3) = t;
	Trans.block<1, 3>(3, 0) = MatrixXd::Zero(1, 3);
	Trans(3, 3) = 1;
}

void trans2PosAngle(const Matrix<double, 4, 4>& Trans, Matrix<double, 6, 1>& posAndAngle)
{
	Matrix<double, 3, 3> R;
	Matrix<double, 3, 1> t;
	R = Trans.block<3, 3>(0, 0);
	t = Trans.block<3, 1>(0, 3);
	RtToPosAngle(R, t, posAndAngle);
}

void solveOriginalPtByCircle(const MatrixXd &pt, Matrix<double, 3, 1> &originPt, double &r)
{
	MatrixXd M = pt.transpose();
	Matrix<double, 3, 1> A;
	int num = 0;
	num = M.rows();
	MatrixXd L1 = MatrixXd::Ones(num, 1);
	A = (M.transpose()*M).inverse()*M.transpose()*L1;//求平面法向量A
	MatrixXd B = MatrixXd::Zero(num - 1, 3);
	for (int i=0;i<=num-2;i++)
	{
		B.row(i) = M.row(i + 1) - M.row(i);
	}
	MatrixXd L2 = MatrixXd::Zero(num-1, 1);
	for (int i = 0; i <= num - 2; i++)
	{
		L2(i,0) = (M(i + 1, 0) * M(i + 1, 0) + M(i + 1, 1) * M(i + 1, 1) + M(i + 1, 2)* M(i + 1, 2)- M(i, 0)*M(i, 0)- M(i, 1)*M(i, 1)- M(i, 2)*M(i, 2))/2;
	}
	Matrix<double,4,3> D;
	D.block<3, 3>(0, 0) = B.transpose()*B;
	D.block<1, 3>(3, 0) = A.transpose();
	Matrix<double,4,1> L3;
	L3.block<3, 1>(0, 0) = B.transpose()*L2; 
	L3(3, 0) = 1;
	originPt = (D.transpose()*D).inverse()*D.transpose()*L3;
	r = 0;
	for (int i =0;i<=num-1;i++)
	{
		Matrix <double, 1, 3>tmp;
		tmp = M.row(i) - originPt.transpose();
		r = r + sqrt(tmp(0,0)*tmp(0,0) + tmp(0,1)*tmp(0,1) + tmp(0,2)*tmp(0,2));
	}
	r = r / num;
}

void solveOriginalPtBySphere(const MatrixXd & pt, Matrix<double, 3, 1>& originPt, double & r)
{
	MatrixXd M = pt.transpose();
	r = 0;
	int num = 0;
	num = M.rows();
	MatrixXd X = M.col(0);
	MatrixXd Y = M.col(1);
	MatrixXd Z = M.col(2);
	MatrixXd A(num, 4);
	A.col(0) = -2 * X;
	A.col(1) = -2 * Y;
	A.col(2) = -2 * Z;
	A.col(3) = MatrixXd::Ones(num, 1);
	MatrixXd b = -(X.array().square() + Y.array().square() + Z.array().square());
	MatrixXd P;
	P = (A.transpose()*A).inverse()* A.transpose()*b;
	originPt = P.block<3, 1>(0, 0);
	r = sqrt(pow(P(0, 0), 2) + pow(P(1, 0), 2) + pow(P(2, 0), 2) - P(3, 0));
}

void creatCoordSysGetRt(const Matrix<double, 3, 1>& O_m,/*第一个测量点为新坐标系的原点 */ const Matrix<double, 3, 1>& X_m, /*第二个测量点为新坐标系x轴 **正** 方向上的一点 */ const Matrix<double, 3, 1>& XOY_m,/*第三个测量点为新坐标系XOY平面上的一点(**在y轴正半轴空间**) */ Matrix<double, 3, 3>&R_om,/*o相对于m的旋转矩阵 */ Matrix<double, 3, 1>&t_om)
{
	Matrix<double, 3, 1> OX_unit = (X_m - O_m).normalized();//X轴单位向量
	Matrix<double, 3, 1> OZ_unit = (X_m - O_m).cross(XOY_m-X_m).normalized();//Z轴单位向量
	Matrix<double, 3, 1> OY_unit = OZ_unit.cross(OX_unit);//Y轴单位向量

	R_om << OX_unit, OY_unit, OZ_unit;
	t_om = O_m;
}

void inverseSolution(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 6, 1>& L_norm, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{
	Matrix<double, 3, 6> L;//某姿态支链向量,3行6列，每列为一条支链的向量
	Matrix<double, 3, 3> R;//动平台姿旋转矩阵(相对于静平台坐标系)
	Matrix<double, 3, 1> t;//动平台姿态平移矩阵
	posAngleToRt(posAndAngle, R, t);//由位姿得到旋转矩阵和平移矩阵
	for (int i = 0; i < 6; i++)
	{
		//R * D0 + t为D0在静平台坐标系下的坐标，计算某姿态的支链向量
		L.col(i) = R * D.col(i) + t - S.col(i);
		//计算某姿态下的支链长度
		L_norm(i) = L.col(i).norm();
	}
}

void solveF(const Matrix<double, 6, 1>& posAndAngle, const Matrix<double, 6, 1>& L_norm, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S, Matrix<double, 6, 1>& F)
{
	Matrix<double, 6, 1> tmpL_norm;
	inverseSolution(posAndAngle, tmpL_norm, D, S);
	F = tmpL_norm.cwiseAbs2() - L_norm.cwiseAbs2();//向量分量绝对值的平方
}

void solvedFdx(const Matrix<double, 6, 1>& posAndAngle, const Matrix<double, 6, 1>& L_norm, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S, double delta, Matrix<double, 6, 6>& dFdx)
{
	MatrixXd e = MatrixXd::Identity(6, 6);
	for (int i = 0; i < 6; i++)
	{
		Matrix<double, 6, 1> delta_Fi;
		solveF(posAndAngle + delta * e.col(i), L_norm, D, S, delta_Fi);
		Matrix<double, 6, 1> Fi;
		solveF(posAndAngle, L_norm, D, S, Fi);
		dFdx.col(i) = (delta_Fi - Fi) / delta;
	}
}



void rigidMotionSVDSolution(int n, const MatrixXd & Q_o, const MatrixXd & Q_m, Matrix<double, 3, 3>& R_om, Matrix<double, 3, 1>& t_om)
{
	//求Q点集的均值
	Matrix<double, 3, 1> Q_o_avg = Q_o.rowwise().sum() / n;
	Matrix<double, 3, 1> Q_m_avg = Q_m.rowwise().sum() / n;
	//Q与Q_avg作差
	MatrixXd Q_o_dif = Q_o - Q_o_avg.replicate(1, n);
	MatrixXd Q_m_dif = Q_m - Q_m_avg.replicate(1, n);
	//计算协方差矩阵S0
	Matrix<double, 3, 3> S0 = Q_o_dif * Q_m_dif.transpose();
	//将S0进行SVD分解
	JacobiSVD<MatrixXd> svd(S0, ComputeThinU | ComputeThinV);
	//求旋转矩阵通式
	Matrix<double, 3, 3> M = MatrixXd::Identity(3, 3);//3*3单位阵
	M(2, 2) = (svd.matrixV() * svd.matrixU().transpose()).determinant();
	R_om = svd.matrixV() * M * svd.matrixU().transpose();
	t_om = Q_m_avg - R_om * Q_o_avg;
}

void solveJacobi(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 6, 6>& jacobi, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{
	Matrix<double, 3, 6> L;//某姿态支链向量,3行6列，每列为一条支链的向量
	Matrix<double, 3, 6> L_unit;//支链向量单位化
	Matrix<double, 3, 3> R;//动平台姿旋转矩阵(相对于静平台坐标系)
	Matrix<double, 3, 1> t;//动平台姿态平移矩阵
	Matrix<double, 6, 3> j13;//雅克比矩阵的1-3列
	Matrix<double, 6, 3> j46;//雅克比矩阵的4-6列
	posAngleToRt(posAndAngle, R, t);
	for (int i = 0; i < 6; i++)
	{
		//R * D0 + t为D0在静平台坐标系下的坐标，计算某姿态的支链向量
		L.col(i) = R * D.col(i) + t - S.col(i);
		//计算支链的单位向量
		L_unit.col(i) = L.col(i).normalized();
		//向量叉乘
		j46.row(i) = (R * D.col(i).cross(L_unit.col(i))).transpose();
	}
	j13 = L_unit.transpose();
	jacobi << j13, j46;
}

void forwardSolution(const Matrix<double, 6, 1>& initPosAndAngle, const Matrix<double, 6, 1>& targL_norm, Matrix<double, 6, 1>& targPosAndAngle, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{

	//求解初始位姿下的杆长
	Matrix<double, 6, 1> initL_norm;
	inverseSolution(initPosAndAngle, initL_norm, D, S);
	//使用的参数
	double delta_t = 1e-3; //t是0~1的数,表示杆长变化的步长百分比
	int k = int(1 / delta_t);//杆长变化次数
	Matrix<double, 6, 1> delta_L = (targL_norm - initL_norm)*delta_t;
	double delta = 1e-5; //求偏导时的微小变动量
	double err = 1e-5; //牛顿迭代的阈值
	bool newtonFialed = false;//牛顿迭代成功
	//赋初值
	Matrix<double, 6, 1> nPosAndAngle = initPosAndAngle;//nPosAndAngle为n次迭代的位姿
	//杆长每次增加一个步长
	for (int n = 1; n <= k; n++)
	{
		Matrix<double, 6, 1> Ln_norm = initL_norm + delta_L * n;
		Matrix<double, 6, 6> jacobi_n;
		//求雅克比矩阵
		solveJacobi(nPosAndAngle, jacobi_n, D, S);
		//欧拉法
		nPosAndAngle = nPosAndAngle + jacobi_n.inverse()*delta_L;
		//根据F(X,L)=0方程求误差Ferr
		Matrix<double, 6, 1> tempL_norm;
		inverseSolution(nPosAndAngle, tempL_norm, D, S);
		double Ferr = (tempL_norm.cwiseAbs2() - Ln_norm.cwiseAbs2()).norm();//误差Ferr
		//牛顿迭代逼近
		int newtonNum = 0; //牛顿迭代次数
		while (Ferr > err)
		{
			Matrix<double, 6, 6> dFdx;
			//求F(X, L)对X的偏导数, X这里表示位姿
			solvedFdx(nPosAndAngle, Ln_norm, D, S, delta, dFdx);
			Matrix<double, 6, 1> tmpF;
			//求F(X, L)位姿杆长约束方程
			solveF(nPosAndAngle, Ln_norm, D, S, tmpF);
			nPosAndAngle = nPosAndAngle - dFdx.inverse() * tmpF;
			solveF(nPosAndAngle, Ln_norm, D, S, tmpF);
			Ferr = tmpF.norm();
			newtonNum = newtonNum + 1;
			if (newtonNum > 30)
			{
				cout << "牛顿迭代不收敛！" << endl;
				newtonFialed = true;
				break;//跳出while
			}
		}
	}
	if (!newtonFialed)
	{
		targPosAndAngle = nPosAndAngle;
	}
	else
	{
		targPosAndAngle = initPosAndAngle;
	}


}



void calibrateInitLength(int n_D,/*动平台靶标点的个数 */ Matrix<double, 6, 1>& initL_norm,/*求得的初始杆长 */ const MatrixXd& Q_DD, /*动平台靶标点在自身坐标系下坐标，3行n_D列 */ const MatrixXd& Q_DM, /*动平台靶标点在测量坐标系中的坐标，3行n_D列 */ const Matrix<double, 3, 3>&R_SM,/*静平台相对测量系的旋转矩阵, */ const Matrix<double, 3, 1>&t_SM,/*平移, */ const Matrix<double, 3, 6>& D, /*动平台铰链点在动坐标系下坐标,结构参数 */ const Matrix<double, 3, 6>& S)
{

	Matrix<double, 3, 3> R_DS;//动平台相对静平台的旋转矩阵
	Matrix<double, 3, 1> t_DS;//平移矩阵
	//求动平台靶标点在静坐标系下的坐标
	MatrixXd Q_DS = MatrixXd::Zero(3, n_D);
	Q_DS = R_SM.inverse()*(Q_DM - t_SM.replicate(1, n_D));
	//SVD法求解标定状态下动平台相对静平台的位姿
	rigidMotionSVDSolution(n_D, Q_DD, Q_DS, R_DS, t_DS);
	Matrix <double, 6, 1> posAngle_calibrate;
	RtToPosAngle(R_DS, t_DS, posAngle_calibrate);//得到标定状态下初始的位姿xyzabc信息存在posAngle_calibrate中
	inverseSolution(posAngle_calibrate, initL_norm, D, S);
}

void solveRealPosAndAngleByLazer(int n_D, const Matrix<double, 3, 3>& R_SM, const Matrix<double, 3, 1>& t_SM, const MatrixXd & Q_DD, const MatrixXd & Q_DM, Matrix<double, 6, 1>& realPosAndAngle)
{
	MatrixXd Q_DS = MatrixXd::Zero(3, n_D);
	Matrix<double, 3, 3> R_DS;//动平台相对静平台的旋转矩阵
	Matrix<double, 3, 1> t_DS;

	//求解动平台靶标点在静系中的坐标Q_DS
	Q_DS = R_SM.inverse()*(Q_DM - t_SM.replicate(1, n_D));

	//求解实际位姿(动相对于静的变换关系)，利用动平台靶标点在动系和静系下的坐标
	rigidMotionSVDSolution(n_D,//动平台靶标点个数,
		Q_DD, //动平台靶标点在动系下坐标,
		Q_DS, //动平台靶标点在静系下坐标,
		R_DS,//输出动坐标系相对于静坐标系的旋转矩阵
		t_DS);//输出平移
	RtToPosAngle(R_DS, t_DS, realPosAndAngle);
}

void solveMpt_D(int n_MPt,/*待测点组中点的个数 */ int n_D,/*动平台靶标个数 */ const MatrixXd &Q_DD,/*3*n_D 动平台靶标点在动系下坐标, */ const MatrixXd &Q_DM, const MatrixXd &MPt_M, MatrixXd &MPt_D)
{
	Matrix<double, 3, 3> R_DM;//动平台相对测量坐标系的变换
	Matrix<double, 3, 1> t_DM;
	//求解动平台相对测量坐标系的变换，R_DM,t_DM
	rigidMotionSVDSolution(n_D,//动平台靶标点个数,
		Q_DD, //动平台靶标点在动系下坐标,
		Q_DM, //动平台靶标点在测量系下坐标,
		R_DM,//输出动坐标系相对于测量坐标系的旋转矩阵
		t_DM);//输出平移
	MPt_D = R_DM.inverse()*(MPt_M - t_DM.replicate(1, n_MPt));
}

void solveTarPosAndAnglebyMeasuredPt(int n_MPt, const MatrixXd & MPt_D, const MatrixXd & tarMPt_M, const Matrix<double, 3, 3>& R_SM, const Matrix<double, 3, 1>& t_SM, Matrix<double, 6, 1>& tarPosAndAngleByMPt)
{
	Matrix<double, 3, 3> R_tarDS;//目标动平台相对于静系的旋转矩阵
	Matrix<double, 3, 1> t_tarDS;//平移
	//求目标待测点在静系下的坐标
	MatrixXd tarMPt_S = MatrixXd::Zero(3, n_MPt);
	tarMPt_S = R_SM.inverse()*(tarMPt_M - t_SM.replicate(1, n_MPt));
	//求根据待测点的平台目标位姿
	rigidMotionSVDSolution(n_MPt, MPt_D, tarMPt_S, R_tarDS, t_tarDS);
	RtToPosAngle(R_tarDS, t_tarDS, tarPosAndAngleByMPt);
}

void solveCompensationPosAndAngle(const Matrix<double, 6, 1> &inputPosAndAngle,/*面板输入的目标位姿 */ const Matrix<double, 6, 1> &realPosAndAngle,/*测得的当前实际位姿 */ Matrix<double, 6, 1> &posAndAngleAfterComp)
{
	//角度化为齐次矩阵
	Matrix<double, 4, 4> inputTrans;
	posAngle2Trans(inputPosAndAngle, inputTrans);
	Matrix<double, 4, 4> realTrans;
	posAngle2Trans(inputPosAndAngle, realTrans);
	//计算补偿后齐次矩阵
	Matrix<double, 4, 4> transAfterComp;
	transAfterComp = inputTrans * realTrans.inverse()*inputTrans;
	//补偿后齐次矩阵化为角度
	trans2PosAngle(transAfterComp, posAndAngleAfterComp);
}


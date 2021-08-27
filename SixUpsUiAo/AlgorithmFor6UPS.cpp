#include "AlgorithmFor6UPS.h"

MatrixXd matrix2Homogeneous(const MatrixXd & mat)
{
	MatrixXd mat_hom = MatrixXd::Ones(mat.rows() + 1, mat.cols());
	mat_hom.topRows(mat.rows()) = mat;
	return mat_hom;
}

MatrixXd homogeneous2Matrix(const MatrixXd & mat_hom)
{
	return mat_hom.topRows(mat_hom.rows()-1);
}

void posAngle2Rt(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 3, 3>& R, Matrix<double, 3, 1>& t)
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


extern Matrix4d posAngle2Trans(const Matrix<double, 6, 1>& posAndAngle)
{
	Matrix4d Trans = Matrix4d::Identity();
	Matrix<double, 3, 3> R;
	Matrix<double, 3, 1> t;
	posAngle2Rt(posAndAngle, R, t);
	Trans.block<3, 3>(0, 0) = R;
	Trans.block<3, 1>(0, 3) = t;
	return Trans;
}

Matrix<double, 6, 1> Rt2PosAngle(const Matrix<double, 3, 3>& R, const Matrix<double, 3, 1>& t)
{
	//注：关于绕定坐标系的xyz的rpy角和绕动系的zyx欧拉角
	//由于变换矩阵相乘时绕定系为左乘，绕动系为右乘，因此上述两变换矩阵实际上是相同的均为R(a)*R(b)*R(c)
	Matrix <double, 3, 1> eulerAngle_ZYX = R.eulerAngles(2, 1, 0);
	Matrix <double, 3, 1> Angle_XYZ;
	Angle_XYZ << eulerAngle_ZYX(2), eulerAngle_ZYX(1), eulerAngle_ZYX(0);
	Matrix<double, 6, 1> posAndAngle;
	posAndAngle << t, Angle_XYZ / M_PI * 180;
	return posAndAngle;
}


extern Matrix4d Rt2Trans(const Matrix<double, 3, 3>& R,/*旋转矩阵 */ const Matrix<double, 3, 1>& t)
{
	Matrix4d Trans = Matrix4d::Identity();
	Trans.block<3, 3>(0, 0) = R;
	Trans.block<3, 1>(0, 3) = t;
	return Trans;
}

Matrix<double, 6, 1> trans2PosAngle(const Matrix<double, 4, 4>& Trans)
{
	Matrix<double, 3, 3> R;
	Matrix<double, 3, 1> t;
	R = Trans.block<3, 3>(0, 0);
	t = Trans.block<3, 1>(0, 3);
	Matrix<double, 6, 1> posAndAngle = Rt2PosAngle(R, t);
	return posAndAngle;
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

Matrix4d creatCoordSysGetRt(const Matrix<double, 3, 1>& O_m, const Matrix<double, 3, 1>& X_m, const Matrix<double, 3, 1>& XOY_m)
{
	Matrix<double, 3, 1> OX_unit = (X_m - O_m).normalized();//X轴单位向量
	Matrix<double, 3, 1> OZ_unit = (X_m - O_m).cross(XOY_m - X_m).normalized();//Z轴单位向量
	Matrix<double, 3, 1> OY_unit = OZ_unit.cross(OX_unit);//Y轴单位向量
	Matrix<double, 3, 3> R_om;
	Matrix<double, 3, 1> t_om;
	R_om << OX_unit, OY_unit, OZ_unit;
	t_om = O_m;
	return Rt2Trans(R_om, t_om);
}


void inverseSolution(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 6, 1>& L_norm, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{
	Matrix<double, 3, 6> L;//某姿态支链向量,3行6列，每列为一条支链的向量
	Matrix<double, 3, 3> R;//动平台姿旋转矩阵(相对于静平台坐标系)
	Matrix<double, 3, 1> t;//动平台姿态平移矩阵
	posAngle2Rt(posAndAngle, R, t);//由位姿得到旋转矩阵和平移矩阵
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

Matrix4d rigidMotionSVDSolution(const MatrixXd & Q_o, const MatrixXd & Q_m)
{
	int n = Q_o.cols();
	int p = Q_m.cols();
	if (n!=p)
	{
		cout << "rigidMotionSVDSolution ERROR! " << endl;
		return Matrix4d::Zero();
	}
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
	Matrix<double, 3, 3> R_om = svd.matrixV() * M * svd.matrixU().transpose();
	Matrix<double, 3, 1> t_om = Q_m_avg - R_om * Q_o_avg;
	return Rt2Trans(R_om, t_om);
}


void solveJacobi(const Matrix<double, 6, 1>& posAndAngle, Matrix<double, 6, 6>& jacobi, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{
	Matrix<double, 3, 6> L;//某姿态支链向量,3行6列，每列为一条支链的向量
	Matrix<double, 3, 6> L_unit;//支链向量单位化
	Matrix<double, 3, 3> R;//动平台姿旋转矩阵(相对于静平台坐标系)
	Matrix<double, 3, 1> t;//动平台姿态平移矩阵
	Matrix<double, 6, 3> j13;//雅克比矩阵的1-3列
	Matrix<double, 6, 3> j46;//雅克比矩阵的4-6列
	posAngle2Rt(posAndAngle, R, t);
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
	double delta_t = 1e-1; //t是0~1的数,表示杆长变化的步长百分比
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
				cout << "forwardSolution error 牛顿迭代不收敛！" << endl;
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

Matrix4d calibrateHingePoint(const Matrix<double, 3, 6>& point_theoretical, const Matrix<double, 3, 6>& point_measure, Matrix<double, 3, 6>& point_fact)
{
	Matrix4d TransSVD = Matrix4d::Zero();
	TransSVD = rigidMotionSVDSolution(point_theoretical, point_measure);
	MatrixXd  point_fact_homogeneous;
	point_fact_homogeneous = TransSVD.inverse()*matrix2Homogeneous(point_measure);
	point_fact = homogeneous2Matrix(point_fact_homogeneous);
	return TransSVD;
}


void calibrateTargetPoint(const MatrixXd & point_measure, const Matrix4d & Trans, MatrixXd & point_fact)
{
	MatrixXd  point_fact_homogeneous;
	point_fact_homogeneous = Trans.inverse()*matrix2Homogeneous(point_measure);
	point_fact = homogeneous2Matrix(point_fact_homogeneous);
}

void calibrateInitLength(Matrix<double, 6, 1>& initL_norm, const MatrixXd & Q_DD, const MatrixXd & Q_DM, const Matrix<double, 3, 3>& Trans_SM, const Matrix<double, 3, 6>& D, const Matrix<double, 3, 6>& S)
{
	int n_D = Q_DD.cols();
	//求动平台靶标点在静坐标系下的坐标
	MatrixXd Q_DS_hom = Trans_SM.inverse()*matrix2Homogeneous(Q_DM);
	MatrixXd Q_DS = homogeneous2Matrix(Q_DS_hom);
	//SVD法求解标定状态下动平台相对静平台的位姿
	Matrix4d Trans_DS_calibrate = rigidMotionSVDSolution(Q_DD, Q_DS);
	Matrix <double, 6, 1> posAngle_calibrate = trans2PosAngle(Trans_DS_calibrate);//得到标定状态下初始的位姿xyzabc信息存在posAngle_calibrate中
	inverseSolution(posAngle_calibrate, initL_norm, D, S);
}

Matrix<double, 6, 1> solveRealPosAndAngleByLazer(const Matrix4d & Trans_SM, const MatrixXd & Q_DD, const MatrixXd & Q_DM)
{
	int n_D = Q_DD.cols();
	MatrixXd Q_DD_hom = matrix2Homogeneous(Q_DD);
	//求动平台靶标点在静坐标系下的坐标
	MatrixXd Q_DS_hom = Trans_SM.inverse()*Q_DD_hom;
	MatrixXd Q_DS = homogeneous2Matrix(Q_DS_hom);
	//求解实际位姿(动相对于静的变换关系)，利用动平台靶标点在动系和静系下的坐标
	Matrix4d Trans_DS = rigidMotionSVDSolution(Q_DD, Q_DS);
	Matrix<double, 6, 1> realPosAndAngle;
	realPosAndAngle = trans2PosAngle(Trans_DS);
	return realPosAndAngle;
}


MatrixXd solveMpt_D(const MatrixXd & Q_DD, const MatrixXd & Q_DM, const MatrixXd & MPt_M)
{
	//求解动平台相对测量坐标系的齐次变换矩阵
	Matrix4d Trans_DM = rigidMotionSVDSolution(Q_DD, Q_DM);
	MatrixXd MPt_D_hom = Trans_DM.inverse()* matrix2Homogeneous(MPt_M);
	MatrixXd MPt_D = homogeneous2Matrix(MPt_D_hom);
	return MPt_D;
}

Matrix<double, 6, 1> solveTarPosAndAnglebyMeasuredPt(const MatrixXd & MPt_D, const MatrixXd & tarMPt_M, const Matrix4d & Trans_DM, const Matrix4d &Trans_DS)
{
	//求目标待测点在动系下的齐次坐标
	MatrixXd tarMPt_D_hom = Trans_DM.inverse()*matrix2Homogeneous(tarMPt_M);
	//求目标待测点 和 待测点组 在静系下齐次坐标
	MatrixXd tarMPt_S_hom = Trans_DS.inverse()*tarMPt_D_hom;
	MatrixXd MPt_S_hom = Trans_DS.inverse()*matrix2Homogeneous(MPt_D);
	MatrixXd tarMPt_S = homogeneous2Matrix(tarMPt_S_hom);
	MatrixXd MPt_S = homogeneous2Matrix(MPt_S_hom);
	//求根据待测点的平台目标位姿
	Matrix4d Trans_tarDS = rigidMotionSVDSolution(MPt_S, tarMPt_S);
	return trans2PosAngle(Trans_tarDS);
}


void solveCompensationPosAndAngle(const Matrix<double, 6, 1> &inputPosAndAngle,/*面板输入的目标位姿 */ const Matrix<double, 6, 1> &realPosAndAngle,/*测得的当前实际位姿 */ Matrix<double, 6, 1> &posAndAngleAfterComp)
{
	//角度化为齐次矩阵
	Matrix4d inputTrans;
	inputTrans = posAngle2Trans(inputPosAndAngle);
	Matrix4d realTrans;
	realTrans = posAngle2Trans(inputPosAndAngle);
	//计算补偿后齐次矩阵
	Matrix4d transAfterComp;
	transAfterComp = inputTrans * realTrans.inverse()*inputTrans;
	//补偿后齐次矩阵化为角度
	posAndAngleAfterComp = trans2PosAngle(transAfterComp);
}


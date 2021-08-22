#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	qDebug() << "SixUpsUiAo 构造";

	initIcon();
	initUIList();
	initStructPara();
	initConnect();

	/*Pmac数据采集定时器*/
	dataGatherTimer = new QTimer(this);
	dataGatherTimer->setInterval(200);
	dataGatherTimer->setTimerType(Qt::CoarseTimer);

	/*UI更新定时器*/
	updateUiDataTimer = new QTimer(this);
	updateUiDataTimer->setInterval(200);
	updateUiDataTimer->setTimerType(Qt::CoarseTimer);
	connect(updateUiDataTimer, &QTimer::timeout, this, &SixUpsUiAo::on_updateUiDataTimer);

	/*并联机构回零判断定时器*/
	upsHomeCompleteTimer = new QTimer(this);
	upsHomeCompleteTimer->setInterval(200);
	upsHomeCompleteTimer->setTimerType(Qt::CoarseTimer);
	connect(upsHomeCompleteTimer, &QTimer::timeout, this, &SixUpsUiAo::on_upsHomeCompleteTimer);

	/*并联机构计算定时器*/
	upsCalculateTimer = new QTimer(this);
	upsCalculateTimer->setInterval(250);
	upsCalculateTimer->setTimerType(Qt::CoarseTimer);

	/*并联机构长按点动定时器*/
	upsJogTimer = new QTimer(this);
	upsJogTimer->setInterval(200);
	upsJogTimer->setTimerType(Qt::PreciseTimer);
	connect(upsJogTimer, &QTimer::timeout, this, &SixUpsUiAo::on_upsJogTimer);

	/*计算线程*/
	calculateQthread = new QThread(this);
	myUpsCalculateThread = new UpsCalculateThread();
	myUpsCalculateThread->moveToThread(calculateQthread);
	//connect(calculateQthread, &QThread::started, myPmacThread, &QPmacThread::startPmac);
	connect(calculateQthread, &QThread::finished, myUpsCalculateThread, &QObject::deleteLater);
	connect(calculateQthread, &QThread::finished, calculateQthread, &QObject::deleteLater);
	connect(upsCalculateTimer, &QTimer::timeout, myUpsCalculateThread, &UpsCalculateThread::on_upsCalculateTimer);
	calculateQthread->start();

}

SixUpsUiAo::~SixUpsUiAo()
{
	qDebug() << "SixUpsUiAo 析构";
	//TODO pmac连接时直接关闭界面

	if (GlobalSta::pmacIsConnected)
	{
		delete myPmac;
		GlobalSta::pmacIsConnected = false;
	}
	qDebug() << "stop pamcQthread";
	GlobalSta::pmacQthreadIsSarted = false;

	calculateQthread->quit();
	calculateQthread->wait();
}


void SixUpsUiAo::initIcon()
{
	//setWindowIcon(QIcon(":/SixUPSCtrUI/icon/6UPS85Gray256.ico"));
	//offlineSimIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/offlinesim.png"), QSize(), QIcon::Normal);
	//realtimeSimIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/realtimesim.png"), QSize(), QIcon::Normal);
	//startSimIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/start.png"), QSize(), QIcon::Normal);
	//stopSimIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/stop.png"), QSize(), QIcon::Normal);
	onIcon = QPixmap(QString::fromUtf8(":/SixUpsUiAo/icon/on.png"));
	offIcon = QPixmap(QString::fromUtf8(":/SixUpsUiAo/icon/off.png"));
	loadingIcon = QPixmap(QString::fromUtf8(":/SixUpsUiAo/icon/loading.png"));
	//warningIcon = QPixmap(QString::fromUtf8(":/SixUPSCtrUI/icon/waring.png"));
	//runingIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/running.png"), QSize(), QIcon::Normal);
	//isStopedIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/isstopped.png"), QSize(), QIcon::Normal);
}

void SixUpsUiAo::initUIList()
{
	/***************赋初值****************/
	//多轴点动运动模块
	ui.led_multiAxisJogTranslationSpeed->setValue(UPSData::multiJogTranslationSpeed);
	ui.led_multiAxisJogTranslationStep->setValue(UPSData::multiJogTranslationStep);
	ui.led_multiAxisJogRotateSpeed->setValue(UPSData::multiJogRotateSpeed);
	ui.led_multiAxisJogRotateStep->setValue(UPSData::multiJogRotateStep);
	//单轴运动模块
	ui.led_jogSpeed->setValue(SingleJogData::jogSpeed);

	/*********UI控件指针列表***************/
	btnGroupMultiAxisDirection= new QButtonGroup(this);
	btnGroupMultiAxisDirection->addButton(ui.xRBtn, 1);
	btnGroupMultiAxisDirection->addButton(ui.yRBtn, 2);
	btnGroupMultiAxisDirection->addButton(ui.zRBtn, 3);
	btnGroupMultiAxisDirection->addButton(ui.aRBtn, 4);
	btnGroupMultiAxisDirection->addButton(ui.bRBtn, 5);
	btnGroupMultiAxisDirection->addButton(ui.cRBtn, 6);


	qlabNegLimit_group.append(ui.staNegLimit1);
	qlabNegLimit_group.append(ui.staNegLimit2);
	qlabNegLimit_group.append(ui.staNegLimit3);
	qlabNegLimit_group.append(ui.staNegLimit4);
	qlabNegLimit_group.append(ui.staNegLimit5);
	qlabNegLimit_group.append(ui.staNegLimit6);

	qlabPosLimit_group.append(ui.staPosLimit1);
	qlabPosLimit_group.append(ui.staPosLimit2);
	qlabPosLimit_group.append(ui.staPosLimit3);
	qlabPosLimit_group.append(ui.staPosLimit4);
	qlabPosLimit_group.append(ui.staPosLimit5);
	qlabPosLimit_group.append(ui.staPosLimit6);

	qlabOrigin_group.append(ui.staOrigin1);
	qlabOrigin_group.append(ui.staOrigin2);
	qlabOrigin_group.append(ui.staOrigin3);
	qlabOrigin_group.append(ui.staOrigin4);
	qlabOrigin_group.append(ui.staOrigin5);
	qlabOrigin_group.append(ui.staOrigin6);

	realTimeLengths_group.append(ui.led_realTimeLength1);
	realTimeLengths_group.append(ui.led_realTimeLength2);
	realTimeLengths_group.append(ui.led_realTimeLength3);
	realTimeLengths_group.append(ui.led_realTimeLength4);
	realTimeLengths_group.append(ui.led_realTimeLength5);
	realTimeLengths_group.append(ui.led_realTimeLength6);

	realTimePos_group.append(ui.led_realTimeX);
	realTimePos_group.append(ui.led_realTimeY);
	realTimePos_group.append(ui.led_realTimeZ);
	realTimePos_group.append(ui.led_realTimeRdeg);
	realTimePos_group.append(ui.led_realTimePdeg);
	realTimePos_group.append(ui.led_realTimeYdeg);

	jogInc_group.append(ui.led_jogInc1);
	jogInc_group.append(ui.led_jogInc2);
	jogInc_group.append(ui.led_jogInc3);
	jogInc_group.append(ui.led_jogInc4);
	jogInc_group.append(ui.led_jogInc5);
	jogInc_group.append(ui.led_jogInc6);

	dipJog_group.append(ui.dipJog1);
	dipJog_group.append(ui.dipJog2);
	dipJog_group.append(ui.dipJog3);
	dipJog_group.append(ui.dipJog4);
	dipJog_group.append(ui.dipJog5);
	dipJog_group.append(ui.dipJog6);

	prsJogPos_group.append(ui.prsJogPos1);
	prsJogPos_group.append(ui.prsJogPos2);
	prsJogPos_group.append(ui.prsJogPos3);
	prsJogPos_group.append(ui.prsJogPos4);
	prsJogPos_group.append(ui.prsJogPos5);
	prsJogPos_group.append(ui.prsJogPos6);

	prsJogNeg_group.append(ui.prsJogNeg1);
	prsJogNeg_group.append(ui.prsJogNeg2);
	prsJogNeg_group.append(ui.prsJogNeg3);
	prsJogNeg_group.append(ui.prsJogNeg4);
	prsJogNeg_group.append(ui.prsJogNeg5);
	prsJogNeg_group.append(ui.prsJogNeg6);

	AbsTarPos_group.append(ui.xAbsTarPos1);
	AbsTarPos_group.append(ui.yAbsTarPos2);
	AbsTarPos_group.append(ui.zAbsTarPos3);
	AbsTarPos_group.append(ui.aAbsTarPos4);
	AbsTarPos_group.append(ui.bAbsTarPos5);
	AbsTarPos_group.append(ui.cAbsTarPos6);
}

void SixUpsUiAo::initStructPara()
{
	//导入动静平台结构参数
	csvToMatrixXd("./Data/D.csv", UPSData::D);
	csvToMatrixXd("./Data/S.csv", UPSData::S);
	csvToMatrixXd("./Data/initL.csv", UPSData::initL_norm);
	csvToMatrixXd("./Data/homePosAndAngle.csv", UPSData::homePosAndAngle);
	UPSData::initPosAndAngle = UPSData::homePosAndAngle;
}

void SixUpsUiAo::initConnect()
{
	//平台回零按钮
	connect(ui.upsHomeBtn, &QPushButton::clicked, this, &SixUpsUiAo::upsHome_slot);
	connect(this, &SixUpsUiAo::upsHome_signal, this, &SixUpsUiAo::upsHome_slot);//平台回零
	for (int i = 0; i < PmacData::numL; i++)
	{
		/**************************多轴运动**************************/
		connect(AbsTarPos_group[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SixUpsUiAo::absTarPos_group_valueChanged);
		/**************************单轴运动**************************/
		//距离点动 输入框 信号槽
		connect(jogInc_group[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SixUpsUiAo::led_jogInc_valueChanged);
		//距离电动 按钮点击 信号槽
		connect(dipJog_group[i], &QToolButton::clicked, this, &SixUpsUiAo::dipJog_Clicked);
		//长按点动 正方向按钮按下 信号槽
		connect(prsJogPos_group[i], &QToolButton::pressed, this, &SixUpsUiAo::prsJogPos_pressed);
		//长按点动 正方向按钮松开 信号槽
		connect(prsJogPos_group[i], &QToolButton::released, this, &SixUpsUiAo::prsJogPos_released);
		//长按点动 负方向按钮按下 信号槽
		connect(prsJogNeg_group[i], &QToolButton::pressed, this, &SixUpsUiAo::prsJogNeg_pressed);
		//长按点动 正方向按钮松开 信号槽
		connect(prsJogNeg_group[i], &QToolButton::released, this, &SixUpsUiAo::prsJogNeg_released);
	}

	connect(this, &SixUpsUiAo::platformDHome_signal, this, &SixUpsUiAo::platformDHome_slot);
}

void SixUpsUiAo::switchPmacThread()
{
	if (GlobalSta::pmacQthreadIsSarted)
	{
		//关闭pmac0并delete指针
		if (GlobalSta::pmacIsConnected)
		{
			delete myPmac;
			GlobalSta::pmacIsConnected = false;
		}
		qDebug() << "stop pamcQthread";
		GlobalSta::pmacQthreadIsSarted = false;
	}
	else
	{
		//PMAC 读取操作不能在子线程中
		myPmac = new QPmac(this);
		GlobalSta::pmacIsConnected = myPmac->creatPmacSelect();
		GlobalSta::pmacQthreadIsSarted = true;

		connect(dataGatherTimer, &QTimer::timeout, myPmac, &QPmac::on_dataGatherTimer);
		qDebug() << "start pmacQthread";
	}
}

bool SixUpsUiAo::QMesBoxWhetherHome()
{
	int questionResult = QMessageBox::question(NULL, "提示", "是否恢复零位", QMessageBox::Yes | QMessageBox::No);
	if (questionResult == QMessageBox::Yes)
	{
		qDebug() << "平台归零 yes";
		return true;
	}
	else
	{
		qDebug() << "平台以当前位姿运动 no";
		return false;
	}
}

void SixUpsUiAo::upsHome_slot()
{
	qDebug() << "upsHome_slot ";
	/******弹出等待窗口*****/
	WaitWindowUI = new WaitWindow();
	WaitWindowUI->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
	WaitWindowUI->show();
	/******各轴开始回零*****/
	myPmac->setHomeCompleteZero();
	myPmac->axlesHomeMove();
	/******检测各轴是否归零计时器开始计时*****/
	upsHomeCompleteTimer->start();

}


void SixUpsUiAo::platformDHome_slot()
{
	inverseSolution(UPSData::homePosAndAngle, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarLengths = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsHomeMove(UPSData::tarLengths, UPSData::multiSpeed);
}

void SixUpsUiAo::on_paraCailbrate_triggered()
{
	qDebug() << "on_paraCailbrate_triggered";
	//显示子窗体，必须用new关键字创建子窗体对象，否则此函数执行完毕后对象会被销毁。
	ParameterCalibrateUI = new ParameterCalibrate();
	ParameterCalibrateUI->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
	ParameterCalibrateUI->show();
}


void SixUpsUiAo::on_updateUiDataTimer()
{
	//qDebug() << "on_updateUiDataTimer";
	QString strLength;
	for (int i = 0; i < PmacData::numL; i++)
	{
		/**********杆长***************/
		strLength = QString::number(PmacData::curLengthsMM(i), 'f', 3);
		realTimeLengths_group[i]->setText(strLength);

		/**********开关状态***************/
		//负限位
		if (PmacData::negLimitState(i) == 1)
		{
			qlabNegLimit_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabNegLimit_group[i]->setPixmap(offIcon);
		}

		//正限位
		if (PmacData::posLimitState(i) == 1)
		{
			qlabPosLimit_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabPosLimit_group[i]->setPixmap(offIcon);
		}

		//原点
		if (PmacData::originState(i) == 1)
		{
			qlabOrigin_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabOrigin_group[i]->setPixmap(offIcon);
		}

	}
	/********实时位姿**********/
	QString strPos;
	for (int i = 0; i < 6; i++)
	{
		strPos = QString::number(UPSData::curPosAndAngle(i), 'f', 3);
		realTimePos_group[i]->setText(strPos);
	}

}

void SixUpsUiAo::on_upsHomeCompleteTimer()
{
	//qDebug() << "on_upsHomeCompleteTimer ";

	if (PmacData::axleHomeCompleteState.head(6).sum() == 6 && PmacData::originState.head(6).sum() == 6)
	{
		GlobalSta::axlesIshome = true;//表示所有轴已经回原点
	}
	if (PmacData::pVariable(2 - 1) == 1)
	{
		GlobalSta::upsIsHome = true;//并联机构动平台回零程序完成
	}
	/*********各轴归零完成，平台未归零*******/
	if (GlobalSta::axlesIshome == true && GlobalSta::upsIsHome == false && upsHomeSignalEmited == false)
	{

		emit platformDHome_signal();  // 平台根据当前位姿 让动平台回归初始零位 即二次归零
		upsHomeSignalEmited = true;//这个标志位的作用是保证 platformDHome_signal只发送一次
		qDebug() << "emit platformDHome_signal()  ";

	}
	if (GlobalSta::axlesIshome == true && GlobalSta::upsIsHome == true)
	{
		GlobalSta::upsIsHome = false;
		GlobalSta::axlesIshome = false;
		upsHomeSignalEmited = false;
		WaitWindowUI->close();//关闭等待对话框
		myPmac->setPvariable(2, 0); //将P2置0
		upsHomeCompleteTimer->stop();
		qDebug() << "WaitWindowUI->close() ";
	}
}

void SixUpsUiAo::on_upsJogTimer()
{
	double miniStep;//最小运动步长
	int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//电机了那个方向
	if (MultiAxisDirectionID <= 3)
	{
		//平动
		miniStep = 0.01;//最小运动步长
	}
	else
	{
		//转动
		miniStep = 0.01;//最小运动步长
	}
	
	Matrix<double, 6, 1> delta_Lengths;
	Matrix<double, 6, 1> axlesSpeed;
	UPSData::tarPosAndAngle = UPSData::tarPosAndAngle + miniStep * UPSData::multiJogMoveDirection;
	cout << UPSData::tarPosAndAngle << endl;
	inverseSolution(UPSData::tarPosAndAngle, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarLengths = UPSData::tarL_norm - UPSData::initL_norm;
	delta_Lengths = (UPSData::tarLengthsLast - UPSData::tarLengths);
	axlesSpeed = delta_Lengths / 200 * PmacData::cts2mm;
	myPmac->upsJogJMove(UPSData::tarLengths, axlesSpeed, 1);
	UPSData::tarLengthsLast = UPSData::tarLengths;//更新
}

void SixUpsUiAo::on_connectPmacBtn_clicked()
{
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
	if (!GlobalSta::pmacIsConnected)
	{
		//连接pmac,开启pmac线程
		switchPmacThread();

		if (GlobalSta::pmacIsConnected)
		{
			ui.pmacSta->setPixmap(onIcon);
			ui.connectPmacBtn->setText("断开");
			ui.initPmacBtn->setEnabled(true);
		}
	}
	else
	{
		/*回零相关复位*/
		upsHomeCompleteTimer->stop();
		GlobalSta::upsIsHome = false;
		GlobalSta::axlesIshome = false;
		upsHomeSignalEmited = false;
		myPmac->setPvariable(2, 0); //将P2置0
		/*计时器相关*/
		upsCalculateTimer->stop();//停止计算并联机构实时位姿
		dataGatherTimer->stop();//数据采集停止
		updateUiDataTimer->stop();//停止更新UI

		//让相关状态显示UI初始化
		for (int i = 0; i < PmacData::numL; i++)
		{
			qlabNegLimit_group[i]->setPixmap(offIcon);
			qlabPosLimit_group[i]->setPixmap(offIcon);
			realTimeLengths_group[i]->setText("");
			realTimePos_group[i]->setText("");
		}

		ui.pmacSta->setPixmap(offIcon);
		ui.connectPmacBtn->setText("连接");

		ui.initPmacBtn->setEnabled(false);
		GlobalSta::pmacIsInitialed = false;
		ui.pmacInitSta->setPixmap(offIcon);

		ui.servoOnBtn->setEnabled(false);
		ui.servoOffBtn->setEnabled(false);

		//断开pamc
		switchPmacThread();
	}
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
}

void SixUpsUiAo::on_initPmacBtn_clicked()
{
	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
	ui.pmacInitSta->setPixmap(loadingIcon);
	myPmac->initPmac();//初始化pmac
	bool ifHome = QMesBoxWhetherHome();//平台时候归零
	qDebug() << "on_initPmacBtn_clicked";
	if (GlobalSta::pmacIsInitialed)
	{
		dataGatherTimer->start();//数据采集开始
		upsCalculateTimer->start(); //开始计算并联机构实时位姿
		updateUiDataTimer->start();//开始更新UI
	}

	if (ifHome == true)
	{
		myPmac->jogStop();
		emit upsHome_signal();
		//TODO 
		//1.运行回零程序
		//2.待各轴回零结束后 提示初始化完成
	}
	else
	{
		//TODO
		//1.从文件中读取上一次正常结束程序时的PMAC杆长
		//2.将上一次的PMAC杆长赋值给PMAC
		//3.提示初始化完成

	}

	if (GlobalSta::upsIsHome)
	{
		ui.pmacInitSta->setPixmap(onIcon);
		ui.servoOnBtn->setEnabled(true);
		ui.servoOffBtn->setEnabled(true);
	}

	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
}

void SixUpsUiAo::on_servoOnBtn_clicked()
{
	myPmac->jogStop();
}

void SixUpsUiAo::on_servoOffBtn_clicked()
{
	myPmac->setServoOff();
}


void SixUpsUiAo::on_getRealTimePosBtn_clicked()
{
	QString strPos;
	for (int i = 0; i < 6; i++)
	{
		AbsTarPos_group[i]->setValue(UPSData::curPosAndAngle(i));
	}
	qDebug() << "on_getRealTimePosBtn_clicked";
}

void SixUpsUiAo::on_startMoveBtn_clicked()
{
	inverseSolution(UPSData::tarPosAndAngle, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarLengths = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsAbsMove(UPSData::tarLengths, UPSData::multiSpeed);
}

void SixUpsUiAo::on_stopMoveBtn_clicked()
{
	myPmac->jogStop();
}


void SixUpsUiAo::absTarPos_group_valueChanged(double data)
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "xAbsTarPos";
	int posNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//绝对位姿 x y z a b c的顺序号
	int index = posNum - 1;//轴号减1才是索引号
	UPSData::tarPosAndAngle[index] = data;
	qDebug() << "posNum:" << posNum << " tarPosAndAngle:" << UPSData::tarPosAndAngle[index];
}


void SixUpsUiAo::on_led_multiAxisJogTranslationSpeed_valueChanged(double data)
{
	UPSData::multiJogTranslationSpeed = data;
	qDebug() << "UPSData::multiJogTranslationSpeed =:" << data;
}

void SixUpsUiAo::on_led_multiAxisJogTranslationStep_valueChanged(double data)
{
	UPSData::multiJogTranslationStep = data;
	qDebug() << "UPSData::multiJogTranslationStep = :" << data;
}

void SixUpsUiAo::on_led_multiAxisJogRotateSpeed_valueChanged(double data)
{
	UPSData::multiJogRotateSpeed = data;
	qDebug() << "UPSData::multiJogRotateSpeed =:" << data;
}

void SixUpsUiAo::on_led_multiAxisJogRotateStep_valueChanged(double data)
{
	UPSData::multiJogRotateStep = data;
	qDebug() << "UPSData::multiJogRotateStep =:" << data;
}

void SixUpsUiAo::on_disMultiAxisJog_clicked()
{
	Matrix<double, 6, 1> incPosAndAngle = MatrixXd::Zero(6, 1);
	switch (btnGroupMultiAxisDirection->checkedId())
	{
	case 1:
		incPosAndAngle(0) = UPSData::multiJogTranslationStep;
		qDebug() << "xRBtn";
		break;
	case 2:
		incPosAndAngle(1) = UPSData::multiJogTranslationStep;
		qDebug() << "yRBtn";
		break;
	case 3:
		incPosAndAngle(2) = UPSData::multiJogTranslationStep;
		qDebug() << "zRBtn";
		break;
	case 4:
		incPosAndAngle(3) = UPSData::multiJogTranslationStep;
		qDebug() << "aRBtn";
		break;
	case 5:
		incPosAndAngle(4) = UPSData::multiJogTranslationStep;
		qDebug() << "bRBtn";
		break;
	case 6:
		incPosAndAngle(5) = UPSData::multiJogTranslationStep;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}
	UPSData::tarPosAndAngle = UPSData::curPosAndAngle + incPosAndAngle;
	inverseSolution(UPSData::tarPosAndAngle, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarLengths = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsAbsMove(UPSData::tarLengths, UPSData::multiJogTranslationSpeed);
	
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_pressed()
{
	UPSData::tarPosAndAngle = UPSData::curPosAndAngle;
	UPSData::tarLengthsLast = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	//获取运动方向
	int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//电机了那个方向
	switch (MultiAxisDirectionID)
	{
	case 1:
		moveDirection(0) = -1;
		qDebug() << "xRBtn";
		break;
	case 2:
		moveDirection(1) = -1;
		qDebug() << "yRBtn";
		break;
	case 3:
		moveDirection(2) = -1;
		qDebug() << "zRBtn";
		break;
	case 4:
		moveDirection(3) = -1;
		qDebug() << "aRBtn";
		break;
	case 5:
		moveDirection(4) = -1;
		qDebug() << "bRBtn";
		break;
	case 6:
		moveDirection(5) = -1;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}
	UPSData::multiJogMoveDirection = moveDirection;
	upsJogTimer->start();
	/*int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//电机了那个方向
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	//获取运动方向
	switch (MultiAxisDirectionID)
	{
	case 1:
		moveDirection(0) = -1;
		qDebug() << "xRBtn";
		break;
	case 2:
		moveDirection(1) = -1;
		qDebug() << "yRBtn";
		break;
	case 3:
		moveDirection(2) = -1;
		qDebug() << "zRBtn";
		break;
	case 4:
		moveDirection(3) = -1;
		qDebug() << "aRBtn";
		break;
	case 5:
		moveDirection(4) = -1;
		qDebug() << "bRBtn";
		break;
	case 6:
		moveDirection(5) = -1;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}
	
	if (MultiAxisDirectionID <= 3)
	{
		//平动
		//myPmac->upsJogLinearMove(moveDirection, UPSData::multiJogTranslationSpeed, 1);
		myPmac->upsJogSpline1Move(moveDirection, UPSData::multiJogTranslationSpeed, 1);
	}
	else
	{
		//转动
		//myPmac->upsJogLinearMove(moveDirection, UPSData::multiJogRotateSpeed, 2);
		myPmac->upsJogSpline1Move(moveDirection, UPSData::multiJogTranslationSpeed, 2);
	}*/
	
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_released()
{
	upsJogTimer->stop();
	//myPmac->jogStop();
}

void SixUpsUiAo::on_prsmultiAxisJogPos_pressed()
{
	UPSData::tarPosAndAngle = UPSData::curPosAndAngle;
	UPSData::tarLengthsLast = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	//获取运动方向
	int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//电机了那个方向
	switch (MultiAxisDirectionID)
	{
	case 1:
		moveDirection(0) = 1;
		qDebug() << "xRBtn";
		break;
	case 2:
		moveDirection(1) = 1;
		qDebug() << "yRBtn";
		break;
	case 3:
		moveDirection(2) = 1;
		qDebug() << "zRBtn";
		break;
	case 4:
		moveDirection(3) = 1;
		qDebug() << "aRBtn";
		break;
	case 5:
		moveDirection(4) = 1;
		qDebug() << "bRBtn";
		break;
	case 6:
		moveDirection(5) = 1;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}
	UPSData::multiJogMoveDirection = moveDirection;
	upsJogTimer->start();
	/*int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//电机了那个方向
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	//获取运动方向
	switch (MultiAxisDirectionID)
	{
	case 1:
		moveDirection(0) = 1;
		qDebug() << "xRBtn";
		break;
	case 2:
		moveDirection(1) = 1;
		qDebug() << "yRBtn";
		break;
	case 3:
		moveDirection(2) = 1;
		qDebug() << "zRBtn";
		break;
	case 4:
		moveDirection(3) = 1;
		qDebug() << "aRBtn";
		break;
	case 5:
		moveDirection(4) = 1;
		qDebug() << "bRBtn";
		break;
	case 6:
		moveDirection(5) = 1;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}

	if (MultiAxisDirectionID <= 3)
	{
		//平动
		//myPmac->upsJogLinearMove(moveDirection, UPSData::multiJogTranslationSpeed, 1);
		myPmac->upsJogSpline1Move(moveDirection, UPSData::multiJogTranslationSpeed, 1);
	}
	else
	{
		//转动
		//myPmac->upsJogLinearMove(moveDirection, UPSData::multiJogRotateSpeed, 2);
		myPmac->upsJogSpline1Move(moveDirection, UPSData::multiJogTranslationSpeed, 2);
	}*/
}

void SixUpsUiAo::on_prsmultiAxisJogPos_released()
{
	upsJogTimer->stop();
	//myPmac->jogStop();
}


void SixUpsUiAo::on_led_jogSpeed_valueChanged(double speed)
{
	SingleJogData::jogSpeed = speed;
	qDebug() << "on_led_jogSpeed_valueChanged_valuechanged :" << SingleJogData::jogSpeed;
}

void SixUpsUiAo::on_jogStopBtn_clicked()
{
	myPmac->jogStop();
}


void SixUpsUiAo::led_jogInc_valueChanged(double Inc)
{

	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "led_jogInc";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	int index = axleNum - 1;//轴号减1才是索引号
	SingleJogData::jogInc[index] = Inc;
	qDebug() << "axleNum:" << axleNum << " jogInc:" << SingleJogData::jogInc[index];

}

void SixUpsUiAo::dipJog_Clicked()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "dipJog";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	int index = axleNum - 1;//轴号减1才是索引号

	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//先设置运动速度
	myPmac->jogDisp(axleNum, SingleJogData::jogInc[index]);//再距离点动
}

void SixUpsUiAo::prsJogPos_pressed()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogPos";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	qDebug() << "axleNum:" << axleNum << " prsJogPos_pressed";
	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//先设置运动速度
	myPmac->jogPosContinuously(axleNum);

}

void SixUpsUiAo::prsJogPos_released()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogPos";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	myPmac->jogStop(axleNum);
	qDebug() << "axleNum:" << axleNum << " prsJogPos_released";
}

void SixUpsUiAo::prsJogNeg_pressed()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogNeg";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	qDebug() << "axleNum:" << axleNum << " prsJogNeg_pressed";
	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//先设置运动速度
	myPmac->jogNegContinuously(axleNum);
}

void SixUpsUiAo::prsJogNeg_released()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogNeg";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//轴号
	myPmac->jogStop(axleNum);
	qDebug() << "axleNum:" << axleNum << " prsJogNeg_released";
}


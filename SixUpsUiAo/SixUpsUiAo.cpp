#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	qDebug() << "SixUpsUiAo 构造";

	initIcon();
	initUIList();
	initStructPara();

	
	/*Pmac数据采集定时器*/
	dataGatherTimer = new QTimer(this);
	dataGatherTimer->setInterval(100);
	dataGatherTimer->setTimerType(Qt::CoarseTimer);

	/*UI更新定时器*/
	updateUiDataTimer = new QTimer(this);
	updateUiDataTimer->setInterval(200);
	updateUiDataTimer->setTimerType(Qt::CoarseTimer);
	connect(updateUiDataTimer, &QTimer::timeout, this, &SixUpsUiAo::on_updateUiDataTimer);

	/*正解计算定时器*/
	upsCalculateTimer = new QTimer(this);
	upsCalculateTimer->setInterval(200);
	upsCalculateTimer->setTimerType(Qt::CoarseTimer);

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


}

void SixUpsUiAo::initStructPara()
{
	//导入动静平台结构参数
	csvToMatrixXd("./Data/D.csv", UPSData::D);
	csvToMatrixXd("./Data/S.csv", UPSData::S);
	csvToMatrixXd("./Data/initL.csv", UPSData::initL_norm);
	UPSData::initPosAndAngle.col(0) << 0.0, 0.0, 550.0, 0, 0, 0;
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

void SixUpsUiAo::on_paraCailbrate_triggered()
{
	qDebug() << "mu clicked";
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
		strLength = QString::number(PmacData::curLengths(i), 'f', 3);
		realTimeLengths_group[i]->setText(strLength);

		/**********限位状态***************/
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

	}
	/********实时位姿**********/
	QString strPos;
	for (int i = 0; i < 6; i++)
	{
		strPos = QString::number(UPSData::curPosAndAngle(i), 'f', 3);
		realTimePos_group[i]->setText(strPos);
	}
	
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
		//断开pamc
		switchPmacThread();

		if (!GlobalSta::pmacIsConnected)
		{
			upsCalculateTimer->stop();//停止计算并联机构实时位姿
			dataGatherTimer->stop();//数据采集停止
			updateUiDataTimer->stop();//停止更新UI

			//让相关状态显示UI初始化
			for (int i = 0; i < PmacData::numL ;i++)
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
		}
	}
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
}

void SixUpsUiAo::on_initPmacBtn_clicked()
{
	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
	ui.pmacInitSta->setPixmap(loadingIcon);
	//初始化pmac
	myPmac->initPmac();
	if (GlobalSta::pmacIsInitialed)
	{
		dataGatherTimer->start(100);//数据采集开始
		upsCalculateTimer->start(150); //开始计算并联机构实时位姿
		updateUiDataTimer->start(200);//开始更新UI
		
		ui.pmacInitSta->setPixmap(onIcon);
		
	}
	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
}

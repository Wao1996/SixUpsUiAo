#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	qDebug() << "SixUpsUiAo 构造";

	initIcon();
	initQlab();


	
	/*Pmac数据采集定时器*/
	dataGatherTimer = new QTimer(this);
	dataGatherTimer->setInterval(100);
	dataGatherTimer->setTimerType(Qt::CoarseTimer);

	/*UI更新定时器*/
	updateUiDataTimer = new QTimer(this);
	updateUiDataTimer->setInterval(200);
	updateUiDataTimer->setTimerType(Qt::CoarseTimer);
	connect(updateUiDataTimer, &QTimer::timeout, this, &SixUpsUiAo::on_updateUiDataTimer);
	

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

void SixUpsUiAo::initQlab()
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
	qDebug() << "on_updateUiDataTimer";
	for (int i = 0; i <= 5; i++)
	{
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
			dataGatherTimer->start(100);//数据采集开始
			updateUiDataTimer->start(200);//开始更新UI

			ui.pmacSta->setPixmap(onIcon);
			ui.connectPmacBtn->setText("断开");
		}
	}
	else
	{
		//断开pamc
		switchPmacThread();

		if (!GlobalSta::pmacIsConnected)
		{
			dataGatherTimer->stop();//数据采集停止
			updateUiDataTimer->stop();//停止更新UI
			//让相关状态显示UI初始化
			for (int i = 0; i <=5;i++)
			{
				qlabNegLimit_group[i]->setPixmap(offIcon);
				qlabPosLimit_group[i]->setPixmap(offIcon);
			}

			ui.pmacSta->setPixmap(offIcon);
			ui.connectPmacBtn->setText("连接");

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
		ui.pmacInitSta->setPixmap(onIcon);

	}
	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
}

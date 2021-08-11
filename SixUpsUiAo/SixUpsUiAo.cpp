#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	qDebug() << "SixUpsUiAo 构造";

	initIcon();

}

SixUpsUiAo::~SixUpsUiAo()
{
	qDebug() << "SixUpsUiAo 析构";
	//TODO pmac连接时直接关闭界面
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
	//loadingIcon = QPixmap(QString::fromUtf8(":/SixUPSCtrUI/icon/loading.png"));
	//warningIcon = QPixmap(QString::fromUtf8(":/SixUPSCtrUI/icon/waring.png"));
	//runingIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/running.png"), QSize(), QIcon::Normal);
	//isStopedIcon.addFile(QString::fromUtf8(":/SixUPSCtrUI/icon/isstopped.png"), QSize(), QIcon::Normal);
}

void SixUpsUiAo::switchPmacThread()
{
	if (GlobalSta::pmacQthreadIsSarted)
	{
		//关闭pmac0并delete指针
		if (GlobalSta::pmacIsConnected)
		{
			myPmacThread->Pmac0->Close(myPmacThread->pDeviceNumber);
			qDebug() << "Pmac0->Close";
			GlobalSta::pmacIsConnected = false;
			qDebug() << "GlobalSta::pmacIsConnected :" << GlobalSta::pmacIsConnected;
		}
		delete myPmacThread->Pmac0;//在此处delete而不在myPmacThread的析构中delete是因为析构中delete会卡死
		myPmacThread->Pmac0 = nullptr;//由于Pmac0在主线程创建，经测试发现在子线程中delete主线程的Pmac0会卡死
		//退出pmac线程			   //因此先释放Pmac0然后myPmacThread->deleteLater
		pmacQthread->quit();
		pmacQthread->wait();
		qDebug() << "stop pamcQthread";
		GlobalSta::pmacQthreadIsSarted = false;
		qDebug() << "GlobalSta::pmacQthreadIsSarted:" << GlobalSta::pmacQthreadIsSarted;
	}
	else
	{
		//创建并开始pmac线程
		myPmacThread = new QPmacThread();
		pmacQthread = new QThread(this);
		//先选择，选择设备会弹出界面，子线程中不能进行ui操作，因此在主线程选择设备
		GlobalSta::pmacIsConnected = myPmacThread->creatPmacSelect();
		qDebug() << "GlobalSta::pmacIsConnected :" << GlobalSta::pmacIsConnected;
		myPmacThread->moveToThread(pmacQthread);
		connect(pmacQthread, &QThread::started, myPmacThread, &QPmacThread::startPmac);
		connect(pmacQthread, &QThread::finished, myPmacThread, &QObject::deleteLater);
		connect(pmacQthread, &QThread::finished, pmacQthread, &QObject::deleteLater);

		//测试代码
		connect(this, &SixUpsUiAo::getMotorDisp, myPmacThread, &QPmacThread::onGetMotorDisp);

		pmacQthread->start();
		GlobalSta::pmacQthreadIsSarted = true;
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
		}
	}
	else
	{
		//断开pamc
		switchPmacThread();

		if (!GlobalSta::pmacIsConnected)
		{
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
}

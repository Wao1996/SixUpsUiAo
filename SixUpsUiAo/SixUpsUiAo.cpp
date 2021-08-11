#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	qDebug() << "SixUpsUiAo ����";

	initIcon();

}

SixUpsUiAo::~SixUpsUiAo()
{
	qDebug() << "SixUpsUiAo ����";
	//TODO pmac����ʱֱ�ӹرս���
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
		//�ر�pmac0��deleteָ��
		if (GlobalSta::pmacIsConnected)
		{
			myPmacThread->Pmac0->Close(myPmacThread->pDeviceNumber);
			qDebug() << "Pmac0->Close";
			GlobalSta::pmacIsConnected = false;
			qDebug() << "GlobalSta::pmacIsConnected :" << GlobalSta::pmacIsConnected;
		}
		delete myPmacThread->Pmac0;//�ڴ˴�delete������myPmacThread��������delete����Ϊ������delete�Ῠ��
		myPmacThread->Pmac0 = nullptr;//����Pmac0�����̴߳����������Է��������߳���delete���̵߳�Pmac0�Ῠ��
		//�˳�pmac�߳�			   //������ͷ�Pmac0Ȼ��myPmacThread->deleteLater
		pmacQthread->quit();
		pmacQthread->wait();
		qDebug() << "stop pamcQthread";
		GlobalSta::pmacQthreadIsSarted = false;
		qDebug() << "GlobalSta::pmacQthreadIsSarted:" << GlobalSta::pmacQthreadIsSarted;
	}
	else
	{
		//��������ʼpmac�߳�
		myPmacThread = new QPmacThread();
		pmacQthread = new QThread(this);
		//��ѡ��ѡ���豸�ᵯ�����棬���߳��в��ܽ���ui��������������߳�ѡ���豸
		GlobalSta::pmacIsConnected = myPmacThread->creatPmacSelect();
		qDebug() << "GlobalSta::pmacIsConnected :" << GlobalSta::pmacIsConnected;
		myPmacThread->moveToThread(pmacQthread);
		connect(pmacQthread, &QThread::started, myPmacThread, &QPmacThread::startPmac);
		connect(pmacQthread, &QThread::finished, myPmacThread, &QObject::deleteLater);
		connect(pmacQthread, &QThread::finished, pmacQthread, &QObject::deleteLater);

		//���Դ���
		connect(this, &SixUpsUiAo::getMotorDisp, myPmacThread, &QPmacThread::onGetMotorDisp);

		pmacQthread->start();
		GlobalSta::pmacQthreadIsSarted = true;
		qDebug() << "start pmacQthread";
	}
}

void SixUpsUiAo::on_paraCailbrate_triggered()
{
	qDebug() << "mu clicked";
	//��ʾ�Ӵ��壬������new�ؼ��ִ����Ӵ�����󣬷���˺���ִ����Ϻ����ᱻ���١�
	ParameterCalibrateUI = new ParameterCalibrate();
	ParameterCalibrateUI->setWindowModality(Qt::ApplicationModal);//���ô���ģ̬��Ҫ��ô���û�и��࣬������Ч
	ParameterCalibrateUI->show();
}


void SixUpsUiAo::on_connectPmacBtn_clicked()
{
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
	if (!GlobalSta::pmacIsConnected)
	{
		//����pmac,����pmac�߳�
		switchPmacThread();

		if (GlobalSta::pmacIsConnected)
		{
			ui.pmacSta->setPixmap(onIcon);
			ui.connectPmacBtn->setText("�Ͽ�");
		}
	}
	else
	{
		//�Ͽ�pamc
		switchPmacThread();

		if (!GlobalSta::pmacIsConnected)
		{
			ui.pmacSta->setPixmap(offIcon);
			ui.connectPmacBtn->setText("����");

			GlobalSta::pmacIsInitialed = false;
			ui.pmacInitSta->setPixmap(offIcon);
		}
	}
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
}

void SixUpsUiAo::on_initPmacBtn_clicked()
{
}

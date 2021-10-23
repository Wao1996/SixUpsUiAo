#include "SixUpsUiAo.h"

extern QReadWriteLock rwLock;

SixUpsUiAo::SixUpsUiAo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	qDebug() << GlobalSta::dataFile;

	initTablesStyle();
	initStructPara();
	initIcon();
	initUIList();
	initConnect();
	myWidgetDisnable();
	
	/*Pmac���ݲɼ���ʱ��*/
	dataGatherTimer = new QTimer(this);
	dataGatherTimer->setInterval(100);
	dataGatherTimer->setTimerType(Qt::CoarseTimer);

	/*UI���¶�ʱ��*/
	updateUiDataTimer = new QTimer(this);
	updateUiDataTimer->setInterval(200);
	updateUiDataTimer->setTimerType(Qt::CoarseTimer);
	connect(updateUiDataTimer, &QTimer::timeout, this, &SixUpsUiAo::on_updateUiDataTimer);

	/*�������������ж϶�ʱ��*/
	upsHomeCompleteTimer = new QTimer(this);
	upsHomeCompleteTimer->setInterval(200);
	upsHomeCompleteTimer->setTimerType(Qt::CoarseTimer);
	connect(upsHomeCompleteTimer, &QTimer::timeout, this, &SixUpsUiAo::on_upsHomeCompleteTimer);

	/*�����������㶨ʱ��*/
	upsCalculateTimer = new QTimer(this);
	upsCalculateTimer->setInterval(200);
	upsCalculateTimer->setTimerType(Qt::CoarseTimer);

	/*�������������㶯��ʱ��*/
	upsJogTimer = new QTimer(this);
	upsJogTimer->setInterval(100);
	upsJogTimer->setTimerType(Qt::PreciseTimer);
	connect(upsJogTimer, &QTimer::timeout, this, &SixUpsUiAo::on_upsJogTimer);

	/*�����߳�*/
	calculateQthread = new QThread(this);
	myUpsCalculateThread = new UpsCalculateThread();
	myUpsCalculateThread->moveToThread(calculateQthread);
	connect(calculateQthread, &QThread::finished, myUpsCalculateThread, &QObject::deleteLater);
	connect(calculateQthread, &QThread::finished, calculateQthread, &QObject::deleteLater);
	connect(upsCalculateTimer, &QTimer::timeout, myUpsCalculateThread, &UpsCalculateThread::on_upsCalculateTimer);
	calculateQthread->start();


}

SixUpsUiAo::~SixUpsUiAo()
{
	qDebug() << "SixUpsUiAo ����";

	//pmac����ʱֱ�ӹرս���
	if (GlobalSta::pmacIsConnected)
	{
		//��¼��ǰλ��
		recordCurLengthsMM();
		delete myPmac;
		GlobalSta::pmacIsConnected = false;
	}
	qDebug() << "stop pamcQthread";
	GlobalSta::pmacQthreadIsSarted = false;

	if (calculateQthread->isRunning())
	{
		calculateQthread->quit();
		calculateQthread->wait();
	}

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
	/*����*/
	ui.tabWidget->removeTab(4);
	ui.mainToolBar->setVisible(false);//���ع�����
	/***************����ֵ****************/
	//����㶯�˶�ģ��
	ui.led_multiAxisJogTranslationSpeed->setValue(UPSData::multiJogTranslationSpeed);
	ui.led_multiAxisJogTranslationStep->setValue(UPSData::multiJogTranslationStep);
	ui.led_multiAxisJogRotateSpeed->setValue(UPSData::multiJogRotateSpeed);
	ui.led_multiAxisJogRotateStep->setValue(UPSData::multiJogRotateStep);
	//�����˶�ģ��
	ui.led_jogSpeed->setValue(SingleJogData::jogSpeed);

	/*********UI�ؼ�ָ���б�***************/
	btnGroupMultiAxisDirection= new QButtonGroup(this);
	btnGroupMultiAxisDirection->addButton(ui.xRBtn, 1);
	btnGroupMultiAxisDirection->addButton(ui.yRBtn, 2);
	btnGroupMultiAxisDirection->addButton(ui.zRBtn, 3);
	btnGroupMultiAxisDirection->addButton(ui.aRBtn, 4);
	btnGroupMultiAxisDirection->addButton(ui.bRBtn, 5);
	btnGroupMultiAxisDirection->addButton(ui.cRBtn, 6);

	btnGroupMultiSpeed= new QButtonGroup(this);
	btnGroupMultiSpeed->addButton(ui.lowSpeed_RBtn, 1);
	btnGroupMultiSpeed->addButton(ui.middleSpeed_RBtn, 2);
	btnGroupMultiSpeed->addButton(ui.highSpeed_RBtn, 3);


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

	realTimeForce_group.append(ui.led_realTimeForce1);
	realTimeForce_group.append(ui.led_realTimeForce2);
	realTimeForce_group.append(ui.led_realTimeForce3);
	realTimeForce_group.append(ui.led_realTimeForce4);
	realTimeForce_group.append(ui.led_realTimeForce5);
	realTimeForce_group.append(ui.led_realTimeForce6);

	realTimePose_DS_group.append(ui.led_realTimeX);
	realTimePose_DS_group.append(ui.led_realTimeY);
	realTimePose_DS_group.append(ui.led_realTimeZ);
	realTimePose_DS_group.append(ui.led_realTimeRdeg);
	realTimePose_DS_group.append(ui.led_realTimePdeg);
	realTimePose_DS_group.append(ui.led_realTimeYdeg);

	realTimePose_setS_group.append(ui.led_realTimeX_setS);
	realTimePose_setS_group.append(ui.led_realTimeY_setS);
	realTimePose_setS_group.append(ui.led_realTimeZ_setS);
	realTimePose_setS_group.append(ui.led_realTimeRdeg_setS);
	realTimePose_setS_group.append(ui.led_realTimePdeg_setS);
	realTimePose_setS_group.append(ui.led_realTimeYdeg_setS);

	realTimePose_setD_group.append(ui.led_realTimeX_setD);
	realTimePose_setD_group.append(ui.led_realTimeY_setD);
	realTimePose_setD_group.append(ui.led_realTimeZ_setD);
	realTimePose_setD_group.append(ui.led_realTimeRdeg_setD);
	realTimePose_setD_group.append(ui.led_realTimePdeg_setD);
	realTimePose_setD_group.append(ui.led_realTimeYdeg_setD);



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
	//���붯��ƽ̨�ṹ����
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/D_new.csv", UPSData::D);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/D_theoretical_new.csv", UPSData::D_theoretical);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/S_new.csv", UPSData::S);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/S_theoretical_new.csv", UPSData::S_theoretical);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/Q_DD.csv", UPSData::Q_DD);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/Q_SS.csv", UPSData::Q_SS);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/�ṹ����/initL_norm_new.csv", UPSData::initL_norm);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/ƽ̨����/homePosAndAngle_new.csv", UPSData::homePosAndAngle_DS);
	UPSData::initPosAndAngle_DS = UPSData::homePosAndAngle_DS;
	UPSData::D_set = UPSData::D;
	
}

void SixUpsUiAo::initTablesStyle()
{
	int rowHeight = 30;//�и�
	int colWidth = 80;//�п�
	int scrollBarHeight = 20;//��������
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";

	/*****keyPointTable ����ϲ���******/
	//������
	ui.keyPointTable->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.keyPointTable->setColumnCount(7);
	QStringList keyPointTableHorizontalHeader;
	keyPointTableHorizontalHeader << "�ؼ���" << "X/mm" << "Y/mm" << "Z/mm" << "��X/��" << "��Y/��" << "��Z/��";
	ui.keyPointTable->setHorizontalHeaderLabels(keyPointTableHorizontalHeader);
	ui.keyPointTable->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.keyPointTable->horizontalHeader()->setFont(QFont("����", 10));//��ͷ����
	ui.keyPointTable->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.keyPointTable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//������
	ui.keyPointTable->setRowCount(0);
	ui.keyPointTable->verticalHeader()->setVisible(false);//�кſɼ�
	ui.keyPointTable->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.keyPointTable->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.keyPointTable->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ�еķ�ʽ

	/*********************tableSM*****************/
	
	QStringList tableSMVerticalHeader;
	tableSMVerticalHeader <<"���е����"<< "X����" << "Y����" << "Z����";
	//������
	ui.tableSM->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableSM->setColumnCount(3);
	ui.tableSM->horizontalHeader()->setVisible(false);//�кŲ��ɼ�
	ui.tableSM->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	//������
	ui.tableSM->setRowCount(tableSMVerticalHeader.size());
	ui.tableSM->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableSM->setVerticalHeaderLabels(tableSMVerticalHeader);
	ui.tableSM->verticalHeader()->setFont(QFont("����", 10));
	ui.tableSM->verticalHeader()->setFixedWidth(120);
	ui.tableSM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableSM->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	//��С����
	ui.tableSM->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableSM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableSM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	/*********************tableGM*****************/
	QStringList tableGMVerticalHeader;
	tableGMVerticalHeader << "X����" << "Y����" << "Z����";
	QStringList tableGMHorizontalHeader;
	tableGMHorizontalHeader << "��װ�е�1" << "��װ�е�2" << "��װ�е�3";
	//������
	ui.tableGM->horizontalHeader()->setDefaultSectionSize(100);
	ui.tableGM->setColumnCount(3);
	ui.tableGM->setHorizontalHeaderLabels(tableGMHorizontalHeader);
	ui.tableGM->horizontalHeader()->setVisible(true);//�кſɼ�
	ui.tableGM->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	ui.tableGM->horizontalHeader()->setFont(QFont("����", 10));//��ͷ����
	ui.tableGM->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableGM->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//������
	ui.tableGM->setRowCount(tableGMVerticalHeader.size());
	ui.tableGM->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableGM->setVerticalHeaderLabels(tableGMVerticalHeader);
	ui.tableGM->verticalHeader()->setFont(QFont("����", 10));
	ui.tableGM->verticalHeader()->setFixedWidth(120);
	ui.tableGM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableGM->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	//��С����
	ui.tableGM->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableGM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableGM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*********************tableTarGM*****************/

	QStringList tableTarGMVerticalHeader;
	tableTarGMVerticalHeader << "X����" << "Y����" << "Z����";
	QStringList tableTarGMHorizontalHeader;
	tableTarGMHorizontalHeader << "��װ�е�1" << "��װ�е�2" << "��װ�е�3";
	//������
	ui.tableTarGM->horizontalHeader()->setDefaultSectionSize(100);
	ui.tableTarGM->setColumnCount(3);
	ui.tableTarGM->setHorizontalHeaderLabels(tableTarGMHorizontalHeader);
	ui.tableTarGM->horizontalHeader()->setVisible(true);//�кŲ��ɼ�
	ui.tableTarGM->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	ui.tableTarGM->horizontalHeader()->setFont(QFont("����", 10));//��ͷ����
	ui.tableTarGM->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableTarGM->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//������
	ui.tableTarGM->setRowCount(tableTarGMVerticalHeader.size());
	ui.tableTarGM->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableTarGM->setVerticalHeaderLabels(tableTarGMVerticalHeader);
	ui.tableTarGM->verticalHeader()->setFont(QFont("����", 10));
	ui.tableTarGM->verticalHeader()->setFixedWidth(120);
	ui.tableTarGM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableTarGM->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	//��С����
	ui.tableTarGM->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableTarGM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableTarGM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*tableInputPoints*/
	QStringList tableInputVerticalHeader;
	tableInputVerticalHeader << "X����" << "Y����" << "Z����";
	//������
	ui.tableInputPoints->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableInputPoints->setColumnCount(4);
	ui.tableInputPoints->horizontalHeader()->setVisible(true);//�кŲ��ɼ�
	ui.tableInputPoints->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	//������
	ui.tableInputPoints->setRowCount(tableInputVerticalHeader.size());
	ui.tableInputPoints->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableInputPoints->setVerticalHeaderLabels(tableInputVerticalHeader);
	ui.tableInputPoints->verticalHeader()->setFont(QFont("����", 10));
	ui.tableInputPoints->verticalHeader()->setFixedWidth(80);
	ui.tableInputPoints->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableInputPoints->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableInputPoints->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableInputPoints->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//��С����
	ui.tableInputPoints->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableInputPoints->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableInputPoints->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*tableInputMat4d*/
	
	//������
	ui.tableInputMat4d->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//�п�ƽ��
	ui.tableInputMat4d->setColumnCount(4);
	ui.tableInputMat4d->horizontalHeader()->setVisible(false);//�кŲ��ɼ�
	//ui.tableInputMat4d->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	//������
	ui.tableInputMat4d->setRowCount(4);
	ui.tableInputMat4d->verticalHeader()->setVisible(false);//�кſɼ�
	ui.tableInputMat4d->setVerticalHeaderLabels(tableInputVerticalHeader);
	ui.tableInputMat4d->verticalHeader()->setFont(QFont("����", 10));
	ui.tableInputMat4d->verticalHeader()->setFixedWidth(80);
	ui.tableInputMat4d->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableInputMat4d->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableInputMat4d->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableInputMat4d->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//��С����
	ui.tableInputMat4d->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableInputMat4d->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableInputMat4d->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void SixUpsUiAo::initConnect()
{
	connect(this, &SixUpsUiAo::upsHome_signal, this, &SixUpsUiAo::upsHome_slot);//ƽ̨����
	for (int i = 0; i < PmacData::numL; i++)
	{
		/**************************�����˶�**************************/
		connect(AbsTarPos_group[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SixUpsUiAo::absTarPos_group_valueChanged);
		/**************************�����˶�**************************/
		//����㶯 ����� �źŲ�
		connect(jogInc_group[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SixUpsUiAo::led_jogInc_valueChanged);
		//����綯 ��ť��� �źŲ�
		connect(dipJog_group[i], &QToolButton::clicked, this, &SixUpsUiAo::dipJog_Clicked);
		//�����㶯 ������ť���� �źŲ�
		connect(prsJogPos_group[i], &QToolButton::pressed, this, &SixUpsUiAo::prsJogPos_pressed);
		//�����㶯 ������ť�ɿ� �źŲ�
		connect(prsJogPos_group[i], &QToolButton::released, this, &SixUpsUiAo::prsJogPos_released);
		//�����㶯 ������ť���� �źŲ�
		connect(prsJogNeg_group[i], &QToolButton::pressed, this, &SixUpsUiAo::prsJogNeg_pressed);
		//�����㶯 ������ť�ɿ� �źŲ�
		connect(prsJogNeg_group[i], &QToolButton::released, this, &SixUpsUiAo::prsJogNeg_released);
	}

	connect(this, &SixUpsUiAo::platformDHome_signal, this, &SixUpsUiAo::platformDHome_slot);
	//�����˶�ѡ���˶�����
	connect(btnGroupMultiAxisDirection, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SixUpsUiAo::btnGroupMultiAxisDirection_clicked);
	//���������˶��ٶ�
	connect(btnGroupMultiSpeed, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SixUpsUiAo::btnGroupMultiSpeed_clicked);
	//��¼�ؼ������Ҽ�
	ui.keyPointTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.keyPointTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuKeyPoint()));
	//����׼���Ҽ������
	ui.tableSM->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableSM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuAddCoils()));
	ui.tableGM->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableGM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuAddCoils()));
	ui.tableTarGM->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableTarGM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuClearContent()));
}

void SixUpsUiAo::myWidgetEnable()
{
	ui.servoBox->setEnabled(true);
	ui.HMIBox->setEnabled(true);

	ui.tabWidget->setEnabled(true);
	ui.tabWidget_input->setEnabled(true);
	ui.tabWidget_2->setEnabled(true);

	ui.pmacInitSta->setPixmap(onIcon);
}

void SixUpsUiAo::myWidgetDisnable()
{
	/*�����״̬��ʾUI��ʼ��*/
	for (int i = 0; i < PmacData::numL; i++)
	{
		qlabNegLimit_group[i]->setPixmap(offIcon);
		qlabPosLimit_group[i]->setPixmap(offIcon);
		qlabOrigin_group[i]->setPixmap(offIcon);
		realTimeLengths_group[i]->setText("");
		realTimeForce_group[i]->setText("");
		realTimePose_DS_group[i]->setText("");
		realTimePose_setS_group[i]->setText("");
		realTimePose_setD_group[i]->setText("");
	}

	ui.pmacSta->setPixmap(offIcon);
	ui.pmacInitSta->setPixmap(offIcon);
	ui.connectPmacBtn->setText("����");
	ui.initPmacBtn->setEnabled(false);
	ui.servoBox->setEnabled(false);
	ui.HMIBox->setEnabled(false);

	//ui.tabWidget->setEnabled(false);
	//ui.tabWidget_input->setEnabled(false);
	//ui.tabWidget_2->setEnabled(false);
}

void SixUpsUiAo::switchPmacThread()
{
	if (GlobalSta::pmacQthreadIsSarted)
	{
		//�ر�pmac0��deleteָ��
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
		//PMAC ��ȡ�������������߳���
		myPmac = new QPmac(this);
		GlobalSta::pmacIsConnected = myPmac->creatPmacSelect();
		GlobalSta::pmacQthreadIsSarted = true;

		connect(dataGatherTimer, &QTimer::timeout, myPmac, &QPmac::on_dataGatherTimer);
		qDebug() << "start pmacQthread";
	}
}

bool SixUpsUiAo::QMesBoxWhetherHome()
{
	int questionResult = QMessageBox::question(NULL, "��ʾ", "�Ƿ�ָ���λ", QMessageBox::Yes | QMessageBox::No);
	if (questionResult == QMessageBox::Yes)
	{
		qDebug() << "ƽ̨���� yes";
		return true;
	}
	else
	{
		qDebug() << "ƽ̨�Ե�ǰλ���˶� no";
		return false;
	}
}

bool SixUpsUiAo::inputKeyPoint(const QString & filePath, QTableWidget * tab)
{
	QFile keyPointFile(filePath);
	//��csvȡ������
	if (!keyPointFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "δ���ļ�:" << filePath;
		return false;
	}
	QTextStream textStream(&keyPointFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!textStream.atEnd())
	{
		QString getRowCount = textStream.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	keyPointFile.close();

	//��ȡtable������
	int tabRow = tab->rowCount();
	int tabCol = tab->columnCount();

	//��csv�������ڵ�ǰ�����������չ�������
	if (csvRowCount > tabRow)
	{
		qDebug() << "�Զ���չ����->" << csvColCount;
		for (int i = 0; i < csvRowCount - tabRow; i++)
		{
			tab->insertRow(i + tabRow);
			//QTableWidgetItem * tmpItem = new QTableWidgetItem();
			//for (int j = 0; j < csvColCount; j++)
			//{
			//	tab->setItem(i + tabRow,j, new QTableWidgetItem());//�����Ԫ��
			//}
		}
	}

	//��csv�ļ���ȡ����
	if (!keyPointFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream keyPointStream(&keyPointFile);
	QString line;
	QStringList fields;
	for (int i = 0; i < csvRowCount; i++)
	{
		line = keyPointStream.readLine();
		fields = line.split(',');//����,�ָ�
		for (int j = 0; j < csvColCount; j++)
		{
			QTableWidgetItem *item = new QTableWidgetItem(fields[j]);
			if (j!=0)
			{
				//���ò��ɱ༭
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			}
			tab->setItem(i, j, item);
			//qDebug() << fields[j];
		}
	}
	keyPointFile.close();
	return true;
}

void SixUpsUiAo::recordCurLengthsMM()
{
	
	matrixXdToCsv(PmacData::curLengthsMM, GlobalSta::dataFile + "/Data/ƽ̨����/lastCurLengthsMM.csv");
}

bool SixUpsUiAo::loadLastLengthsMM()
{
	Matrix<double, 6, 1> lastCurLengthsMM = MatrixXd::Zero(6, 1);
	bool flag = csvToMatrixXd(GlobalSta::dataFile+"/Data/ƽ̨����/lastCurLengthsMM.csv", lastCurLengthsMM);
	if (flag)
	{
		// pmac��ʼʱ�˳�����
		myPmac->setCurLengths(lastCurLengthsMM);
		DeleteFileOrFolder(GlobalSta::dataFile+"/Data/ƽ̨����/lastCurLengthsMM.csv");
		return true;
	} 
	else
	{
		return false;
	}
}

bool SixUpsUiAo::DeleteFileOrFolder(const QString & strPath)
{
	if (strPath.isEmpty() || !QDir().exists(strPath))//�Ƿ����˿յ�·��||·���Ƿ����
		return false;

	QFileInfo FileInfo(strPath);

	if (FileInfo.isFile())//������ļ�
		QFile::remove(strPath);
	else if (FileInfo.isDir())//������ļ���
	{
		QDir qDir(strPath);
		qDir.removeRecursively();
	}
	return true;
}

void SixUpsUiAo::upsHome_slot()
{
	qDebug() << "upsHome_slot ";
	/******�����ȴ�����*****/
	WaitWindowUI = new WaitWindow();
	WaitWindowUI->setWindowModality(Qt::ApplicationModal);//���ô���ģ̬��Ҫ��ô���û�и��࣬������Ч
	WaitWindowUI->show();
	/******���Ὺʼ����*****/
	myPmac->setHomeCompleteZero();//����������λ����
	myPmac->axlesHomeMove();
	/******�������Ƿ�����ʱ����ʼ��ʱ*****/
	upsHomeCompleteTimer->start();

}


void SixUpsUiAo::platformDHome_slot()
{
	inverseSolution(UPSData::homePosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsHomeMove(UPSData::tarAxlesL_norm, UPSData::multiSpeed);
}

void SixUpsUiAo::on_paraCailbrate_triggered()
{
	qDebug() << "on_paraCailbrate_triggered";
	//��ʾ�Ӵ��壬������new�ؼ��ִ����Ӵ�����󣬷���˺���ִ����Ϻ����ᱻ���١�
	ParameterCalibrateUI = new ParameterCalibrate();
	ParameterCalibrateUI->setWindowModality(Qt::ApplicationModal);//���ô���ģ̬��Ҫ��ô���û�и��࣬������Ч
	ParameterCalibrateUI->show();
}


void SixUpsUiAo::on_updateUiDataTimer()
{
	//qDebug() << "on_updateUiDataTimer";
	QString strLength;
	for (int i = 0; i < PmacData::numL; i++)
	{
		/**********�˳�***************/
		strLength = QString::number(PmacData::curLengthsMM(i), 'f', 3);
		realTimeLengths_group[i]->setText(strLength);

		strLength = QString::number(PmacData::curForce(i), 'f', 3); 
		realTimeForce_group[i]->setText(strLength);
		/**********����״̬***************/
		//����λ
		if (PmacData::negLimitState(i) == 1)
		{
			qlabNegLimit_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabNegLimit_group[i]->setPixmap(offIcon);
		}

		//����λ
		if (PmacData::posLimitState(i) == 1)
		{
			qlabPosLimit_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabPosLimit_group[i]->setPixmap(offIcon);
		}

		//ԭ��
		if (PmacData::originState(i) == 1)
		{
			qlabOrigin_group[i]->setPixmap(onIcon);
		}
		else
		{
			qlabOrigin_group[i]->setPixmap(offIcon);
		}

	}
	/********ʵʱλ��**********/
	QString strPos;
	for (int i = 0; i < 6; i++)
	{
		//��ƽ̨��Ծ�ƽ̨
		strPos = QString::number(UPSData::curPosAndAngle_DS(i), 'f', 3);
		realTimePose_DS_group[i]->setText(strPos);
		//�˶�����ϵ��Ծ�ƽ̨
		strPos = QString::number(UPSData::curPosAndAngle_setS(i), 'f', 3);
		realTimePose_setS_group[i]->setText(strPos);
		//��ƽ̨����˶�����ϵ
		strPos = QString::number(UPSData::curPosAndAngle_setD(i), 'f', 3);
		realTimePose_setD_group[i]->setText(strPos);
	}

	
}

void SixUpsUiAo::on_upsHomeCompleteTimer()
{
	//qDebug() << "on_upsHomeCompleteTimer ";

	if (PmacData::axleHomeCompleteState.head(6).sum() == 6 && PmacData::originState.head(6).sum() == 6)
	{
		GlobalSta::axlesIshome = true;//��ʾ�������Ѿ���ԭ��
	}
	if (PmacData::pVariable(2 - 1) == 1)
	{
		qDebug() << "P2=1";
		GlobalSta::upsIsHome = true;//����������ƽ̨����������
	}
	/*********���������ɣ�ƽ̨δ����*******/
	if (GlobalSta::axlesIshome == true && GlobalSta::upsIsHome == false && upsHomeSignalEmited == false)
	{

		emit platformDHome_signal();  // ƽ̨���ݵ�ǰλ�� �ö�ƽ̨�ع��ʼ��λ �����ι���
		upsHomeSignalEmited = true;//�����־λ�������Ǳ�֤ platformDHome_signalֻ����һ��
		qDebug() << "emit platformDHome_signal()  ";

	}
	if (GlobalSta::axlesIshome == true && GlobalSta::upsIsHome == true)
	{
		GlobalSta::axlesIshome = false;
		GlobalSta::upsIsHome = false;
		upsHomeSignalEmited = false;
		WaitWindowUI->close();//�رյȴ��Ի���
		myPmac->setPvariable(2, 0); //��P2��0
		myWidgetEnable();
		upsHomeCompleteTimer->stop();
		qDebug() << "WaitWindowUI->close() ";
	}
}

void SixUpsUiAo::on_upsJogTimer()
{

	/***********��������������ʽ***********/
	Matrix<double, 6, 1> delta_Lengths;//�����˶�ÿ������˶����� mm
	Matrix<double, 6, 1> axlesSpeed;//ÿ������˶��ٶ� mm/s
	Matrix<double, 6, 1> tarL_norm;//Ŀ��µ�˳�
	//�˶�����ϵ��Ծ�ƽ̨ ��һ����λ������
	UPSData::prsPosAndAngle_setS = UPSData::prsPosAndAngle_setS + UPSData::multiJogMoveStep * UPSData::multiJogMoveDirection;
	//�˶�ѧ���ⷴ��
	inverseSolution(UPSData::prsPosAndAngle_setS, tarL_norm, UPSData::D_set, UPSData::S);
	//��PMAC�˳�
	UPSData::tarAxlesL_norm = tarL_norm - UPSData::initL_norm;
	//������һ�����˽µ㳤������һ�����˽µ㳤�ȵĲ�ֵ �õ� ÿ���˵ı仯��
	delta_Lengths = UPSData::tarAxlesL_norm - UPSData::lastAxlesL_norm;
	//��ÿ���˵ı仯��������˵��˶��ٶ� ��֤ÿ����ͬһʱ�䵽��Ŀ��λ��
	axlesSpeed = delta_Lengths.cwiseAbs() * PmacData::cts2mm / upsJogTimer->interval();
	/*�ж� ����и�����λλ��ʱ������λ�����˶� ����ֹ�˶�*/
	for (int i = 0; i < 6; i++)
	{
		if ((delta_Lengths(i) < 0 && PmacData::negLimitState(i) == 1) || (delta_Lengths(i) > 0 && PmacData::posLimitState(i) == 1))
		{
			qDebug() << "mutiAxlesJogNeg error:�����˶��������� ��λ��";
			upsJogTimer->stop();
			return;
		}
	}
	myPmac->upsJogJMove(UPSData::tarAxlesL_norm, axlesSpeed);
	UPSData::lastAxlesL_norm = UPSData::tarAxlesL_norm;//����

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
			ui.initPmacBtn->setEnabled(true);
		}
	}
	else
	{
		myPmac->jogStop();
		myPmac->setServoOff();
		//��¼��ǰ�˳�
		recordCurLengthsMM();
		/*������ظ�λ*/
		upsHomeCompleteTimer->stop();
		GlobalSta::upsIsHome = false;
		GlobalSta::axlesIshome = false;
		upsHomeSignalEmited = false;
		myPmac->setPvariable(2, 0); //��P2��0
		/*��ʱ�����*/
		upsCalculateTimer->stop();//ֹͣ���㲢������ʵʱλ��
		dataGatherTimer->stop();//���ݲɼ�ֹͣ
		updateUiDataTimer->stop();//ֹͣ����UI
		ui.closeHMIBtn->click();
		GlobalSta::pmacIsInitialed = false;
		//�����״̬��ʾUI��ʼ��
		myWidgetDisnable();
		//�Ͽ�pamc
		switchPmacThread();
	}
	qDebug() << "pmacIsConnected = " << GlobalSta::pmacIsConnected;
}

void SixUpsUiAo::on_initPmacBtn_clicked()
{
	qDebug() << "pmacIsInitialed = " << GlobalSta::pmacIsInitialed;
	ui.pmacInitSta->setPixmap(loadingIcon);
	myPmac->initPmac();//��ʼ��pmac
	bool ifHome = QMesBoxWhetherHome();//ѯ���Ƿ�ʱ�����
	qDebug() << "on_initPmacBtn_clicked";
	if (GlobalSta::pmacIsInitialed)
	{
		dataGatherTimer->start();//���ݲɼ���ʼ
		upsCalculateTimer->start(); //��ʼ���㲢������ʵʱλ��
		updateUiDataTimer->start();//��ʼ����UI
	}

	myPmac->jogStop();
	if (ifHome == true)
	{
		/*ƽ̨����*/
		//1.���л������
		//2.�������������� ��ʾ��ʼ�����
		emit upsHome_signal();
	}
	else
	{
		/*ƽ̨�Ե�ǰλ���˶�*/
		//1.���ļ��ж�ȡ��һ��������������ʱ��PMAC�˳�
		//2.����һ�ε�PMAC�˳���ֵ��PMAC
		//3.��ʾ��ʼ�����

		//�����ϴθ˳�����
		bool flagloadLastLengthsMM = loadLastLengthsMM();
		//�����ʼ��ʧ��
		if (!flagloadLastLengthsMM)
		{
			int questionResult = QMessageBox::question(NULL, "��ʾ", "������һ��λ��ʧ��,�轫ƽ̨���㡣ԭ�����Ϊ��һ�γ���δ�����˳����Ƿ���㣿", QMessageBox::Yes | QMessageBox::No);
			if (questionResult == QMessageBox::Yes)
			{
				emit upsHome_signal();
			}
			else
			{
				myWidgetEnable();
			}
	
		}
		else
		{
			GlobalSta::upsIsHome = true;
			if (GlobalSta::upsIsHome)
			{
				GlobalSta::upsIsHome = false;
				myWidgetEnable();
			}
		}
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

void SixUpsUiAo::on_axlesHomeBtn_clicked()
{
	myPmac->jogStop();
	myPmac->axlesHomeMove();
}

void SixUpsUiAo::on_upsHomeBtn_clicked()
{
	myPmac->jogStop();
	inverseSolution(UPSData::homePosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsAbsMove(UPSData::tarAxlesL_norm, UPSData::multiSpeed);
}

void SixUpsUiAo::on_openHMIBtn_clicked()
{

	/*HMI*/
	HMIQthread = new QThread();
	myHMIcommunication = new HMIcommunication();
	myHMIcommunication->moveToThread(HMIQthread);
	connect(HMIQthread, &QThread::started, myHMIcommunication, &HMIcommunication::startHMI);
	connect(HMIQthread, &QThread::finished, myHMIcommunication, &QObject::deleteLater);
	connect(HMIQthread, &QThread::finished, HMIQthread, &QObject::deleteLater);
	connect(myHMIcommunication, &HMIcommunication::wirteRegisters, this, &SixUpsUiAo::wirteRegisters_slot);
	connect(myHMIcommunication, &HMIcommunication::writeCoils, this, &SixUpsUiAo::writeCoils_slot);
	HMIQthread->start();

	ui.closeHMIBtn->setEnabled(true);
	ui.openHMIBtn->setEnabled(false);
	ui.HMISta->setPixmap(onIcon);
}

void SixUpsUiAo::on_closeHMIBtn_clicked()
{
	
	if (HMIQthread->isRunning())
	{
		HMIQthread->requestInterruption();
		HMIQthread->quit();
		HMIQthread->wait();
		delete HMIQthread;

	}

	ui.openHMIBtn->setEnabled(true);
	ui.closeHMIBtn->setEnabled(false);
	ui.HMISta->setPixmap(offIcon);
}




void SixUpsUiAo::on_calibrateSMBtn_clicked()
{
	int numCol = ui.tableSM->columnCount();
	MatrixXd Q_SM_temp = MatrixXd::Zero(3, numCol);
	MatrixXd Q_SS_temp = MatrixXd::Zero(3, numCol);
	//�жϱ�����Ƿ��п�ֵ
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (ui.tableSM->item(i, j) == NULL || ui.tableSM->item(i, j)->text()=="")
			{
				QMessageBox::information(NULL, "��ʾ", "����д��ڿ�ֵ��");
				return;
			}
		}
	}
	for (int j=0;j< numCol;j++)
	{
		int p = ui.tableSM->item(0, j)->text().toInt();
		Q_SS_temp.col(j) = UPSData::Q_SS.col(p - 1);
		for (int i = 0;i<3;i++)
		{
			if (!ui.tableSM->item(i + 1, j)->text().isEmpty())
			{
				Q_SM_temp(i, j) = ui.tableSM->item(i + 1, j)->text().toDouble();
			}
		}
	}
	UPSData::Trans_SM = rigidMotionSVDSolution(Q_SS_temp, Q_SM_temp);
	QMessageBox::information(NULL, "��ʾ", "�궨��ƽ̨�б��ɹ���");
	flag_calibrateSM = true;
}

void SixUpsUiAo::on_calibrateGMBtn_clicked()
{
	int numCol = ui.tableGM->columnCount();
	//�жϱ�����Ƿ��п�ֵ
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (ui.tableGM->item(i, j) == NULL || ui.tableGM->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "��ʾ", "����д��ڿ�ֵ��");
				return;
			}
		}
	}

	//��װ�б���ڶ�����ϵ�µ�����
	vector<int> index;
	MatrixXd Q_GM;
	bool flag = tableToMatrixXd(ui.tableGM, Q_GM, index);
	if (flag)
	{
		MatrixXd  Q_GD_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_GM);
		UPSData::Q_GD = homogeneous2Matrix(Q_GD_homogeneous);
		cout << UPSData::Q_GD;
		QMessageBox::information(NULL, "��ʾ", "�궨��װ�б��ɹ���");
		flag_calibrateGM = true;
	} 
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�궨��װ�б��ʧ�ܣ�");
	}
	
}

void SixUpsUiAo::addTableWidgetMenuAddCoils()
{
	//��ȡ�źŷ�����
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());

	/************���ݱ�����趨��ͬ�б���**************/
	QString curTabName = senderTableWidget->objectName();//��ȡ�����źŵı����
	QStringList tabNameList;//���ֱ����
	tabNameList << "tableSM" << "tableGM" << "tableTarGM";
	QString colTitle;//�б���
	switch (tabNameList.indexOf(curTabName))
	{
	case 0:
		colTitle = "���";
		break;
	case 1:
		colTitle = "��װ�е�";
		break;
	case 2:
		colTitle = "���е�";
		break;
	default:
		break;
	}
	/*******************************************/
	//���ò˵�ѡ��
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *addColumn = new QAction("�����", senderTableWidget);
	QAction *deleteColumn = new QAction("ɾ����", senderTableWidget);
	QAction *clearContents = new QAction("�����������", senderTableWidget);
	connect(addColumn, &QAction::triggered, this, [=]() {
		qDebug() << "addCol";
		int rows = senderTableWidget->rowCount();//��ȡ������
		int cols = senderTableWidget->columnCount();
		senderTableWidget->insertColumn(cols);//������
		QTableWidgetItem * tmpItem = new QTableWidgetItem();
		senderTableWidget->setHorizontalHeaderItem(cols, tmpItem);//�����б���
		QTableWidgetItem * colItem = senderTableWidget->horizontalHeaderItem(cols);
		colItem->setText(colTitle + QString::number(cols + 1));
		for (int i = 0; i < rows; i++)
		{
			senderTableWidget->setItem(i, cols, new QTableWidgetItem());//�����Ԫ��
		}
		senderTableWidget->selectColumn(cols);//ѡ������

		if (tabNameList.indexOf(curTabName) == 1)
		{
			ui.tableTarGM->insertColumn(cols);//������
			tmpItem = new QTableWidgetItem();
			ui.tableTarGM->setHorizontalHeaderItem(cols, tmpItem);//�����б���
			colItem = ui.tableTarGM->horizontalHeaderItem(cols);
			colItem->setText(colTitle + QString::number(cols + 1));
			for (int i = 0; i < rows; i++)
			{
				ui.tableTarGM->setItem(i, cols, new QTableWidgetItem());//�����Ԫ��
			}
			ui.tableTarGM->selectColumn(cols);//ѡ������
			flag_calibrateGM = false;
		}
		else
		{
			flag_calibrateSM = false;
		}

	});
	connect(deleteColumn, &QAction::triggered, this, [=]() {
		qDebug() << "deleteCol";
		QTableWidgetItem * tmpItem = senderTableWidget->currentItem();
		if (tmpItem == Q_NULLPTR)
		{
			return;
		}
		else
		{
			int colNum = senderTableWidget->columnCount();
			if (colNum<=3)
			{
				return;
			}

			if (tabNameList.indexOf(curTabName) == 1)
			{
				
				senderTableWidget->removeColumn(colNum - 1);
				ui.tableTarGM->removeColumn(colNum - 1);//ɾ����
				flag_calibrateGM = false;
			}
			else
			{
				senderTableWidget->removeColumn(tmpItem->column());//ɾ����
				flag_calibrateSM = false;
			}
		}
	});
	connect(clearContents, &QAction::triggered, this, [=]() {
		qDebug() << "clearContents";
		QTableWidgetItem * tmpItem = senderTableWidget->currentItem();
		if (tmpItem == Q_NULLPTR)
		{
			return;
		}
		else
		{
			senderTableWidget->clearContents();
		}
	});
	tableWidgetMenu->addAction(addColumn);
	tableWidgetMenu->addAction(deleteColumn);
	tableWidgetMenu->addAction(clearContents);
	tableWidgetMenu->move(cursor().pos());
	tableWidgetMenu->show();
}

void SixUpsUiAo::addTableWidgetMenuClearContent()
{
	//��ȡ�źŷ�����
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());

	/*******************************************/
	//���ò˵�ѡ��
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *clearContents = new QAction("�����������", senderTableWidget);
	connect(clearContents, &QAction::triggered, this, [=]() {
		qDebug() << "clearContents";
		QTableWidgetItem * tmpItem = senderTableWidget->currentItem();
		if (tmpItem == Q_NULLPTR)
		{
			return;
		}
		else
		{
			senderTableWidget->clearContents();
		}
	});
	tableWidgetMenu->addAction(clearContents);
	tableWidgetMenu->move(cursor().pos());
	tableWidgetMenu->show();
}

void SixUpsUiAo::on_transTarPoseBtn_clicked()
{
	if (flag_calibrateSM == false)
	{
		QMessageBox::information(NULL, "��ʾ", "��δ�궨��ƽ̨�б���ڲ�������ϵ��λ��,��궨�����ԡ�");
		return;
	}

	if (flag_calibrateGM == false)
	{
		QMessageBox::information(NULL, "��ʾ", "��δ�궨��װ�б���ڶ�ϵ������,��궨�����ԡ�");
		return;
	}

	int numCol = ui.tableTarGM->columnCount();
	//�жϱ�����Ƿ��п�ֵ
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (ui.tableTarGM->item(i, j) == NULL || ui.tableTarGM->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "��ʾ", "����д��ڿ�ֵ��");
				return;
			}
		}
	}

	//1.Ŀ��λ�ðб���ڲ�������ϵ�µ�����
	MatrixXd Q_tarGM_temp = MatrixXd::Zero(3, numCol);
	//2.������Ŷ�Ӧ�Ĺ�װ�б���ڶ�ƽ̨�µ�����
	MatrixXd Q_GD_temp = MatrixXd::Zero(3, numCol);
	Q_GD_temp = UPSData::Q_GD;
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!ui.tableTarGM->item(i , j)->text().isEmpty())
			{
				Q_tarGM_temp(i, j) = ui.tableTarGM->item(i, j)->text().toDouble();
			}
		}
	}
	//3.��Ŀ��λ�ðб���ڶ�ƽ̨�µĵ��������
	MatrixXd Q_tarGD_temp_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_tarGM_temp);
	//Ŀ�깤װ�е��ڶ�ƽ̨�µ�����
	MatrixXd Q_tarGD_temp = homogeneous2Matrix(Q_tarGD_temp_homogeneous);
	//4.��Ŀ�깤װ��Ե�ǰ��ƽ̨����α任����
	MatrixXd Trans_tarGD = rigidMotionSVDSolution(Q_GD_temp, Q_tarGD_temp);
	//5.��Ŀ�깤װλ����Ե���ϵ����α任����
	MatrixXd Trans_tarGS = UPSData::Trans_DS*Trans_tarGD;
	//6.��Ŀ�깤װ�ھ�ϵ�е�λ��()
	Matrix<double, 6, 1> PosAndAngle_tarGS = trans2PosAngle(Trans_tarGS);
	//7.��Ŀ�깤װ�����˶�����ϵ�ھ�ϵ�е�λ��
	Matrix<double, 6, 1> PosAndAngle_tarGsetS = posAndAngleDS2setS(PosAndAngle_tarGS, UPSData::Trans_setD);
	for (int i = 0; i < 6; i++)
	{
		AbsTarPos_group[i]->setValue(PosAndAngle_tarGsetS(i));
	}
	ui.tabWidget->setCurrentIndex(2);
}


void SixUpsUiAo::on_setOriginBtn_clicked()
{
	qDebug() << "on_creatDCrdSys_clicked";

	UPSData::XYZ_setD(0) = ui.led_X_setD->value();
	UPSData::XYZ_setD(1) = ui.led_Y_setD->value();
	UPSData::XYZ_setD(2) = ui.led_Z_setD->value();
	
	
	//�޸Ķ�ƽ̨�ṹ����
	Matrix<double, 3, 6> XYZ_setD_Mat= MatrixXd::Zero(3, 6);
	for (int i=0;i<6;i++)
	{
		XYZ_setD_Mat.col(i) = UPSData::XYZ_setD;
	}
	UPSData::D_set = UPSData::D - XYZ_setD_Mat;

	//�޸� �˶�����ϵ��Զ�ƽ̨����α任����
	UPSData::Trans_setD(0, 3) = UPSData::XYZ_setD(0);
	UPSData::Trans_setD(1, 3) = UPSData::XYZ_setD(1);
	UPSData::Trans_setD(2, 3) = UPSData::XYZ_setD(2);
	UPSData::curPosAndAngle_setD = trans2PosAngle(UPSData::Trans_setD);
}


void SixUpsUiAo::on_recordKeyPointBtn_clicked()
{
	int tabRow = ui.keyPointTable->rowCount();
	int tabCol = ui.keyPointTable->columnCount();
	ui.keyPointTable->insertRow(tabRow);
	qDebug() << tabRow;
	//�ؼ������
	ui.keyPointTable->setItem(tabRow, 0, new QTableWidgetItem(QString::number(tabRow + 1)));
	//����ǰ��ƽ̨��Ծ�ƽ̨λ�˼�¼
	for (int i = 0; i < 6; i++)
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(UPSData::curPosAndAngle_DS(i)));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		ui.keyPointTable->setItem(tabRow, i + 1, item);
	}
}

void SixUpsUiAo::on_inputKeyPointBtn_clicked()
{
	QString tempKeyPointsFile = QFileDialog::getOpenFileName(this, "ѡ���ļ�", "D:/", "�ı��ļ�(*.txt;*.csv;)");
	if (tempKeyPointsFile != "")
	{
		keyPointsFile = tempKeyPointsFile;
		ui.keyPointTable->clearContents();
		inputKeyPoint(keyPointsFile, ui.keyPointTable);	
	}
	ui.led_keyPointsFile->setText(keyPointsFile);
}

void SixUpsUiAo::on_saveKeyPointBtn_clicked()
{
	if (keyPointsFile == "")
	{
		int questionResult = QMessageBox::question(NULL, "��ʾ", "δ����ؼ����ļ����Ƿ��½��ؼ����ļ�?", QMessageBox::Yes | QMessageBox::No);
		if (questionResult == QMessageBox::Yes)
		{
			keyPointsFile = QFileDialog::getSaveFileName(this, "ѡ���ļ�", "D:/", "����ļ�(*.csv);;�ı��ļ�(*.txt)");
		}
		else
		{
			return;
		}
	}
	bool flag = tableToCsv(ui.keyPointTable, keyPointsFile);
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "����ؼ������ݳɹ���");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "����ؼ�������ʧ�ܣ�");
	}
	ui.led_keyPointsFile->setText(keyPointsFile);
}


void SixUpsUiAo::on_saveAsKeyPointBtn_clicked()
{
	QString tempKeyPointsFile = QFileDialog::getSaveFileName(this, "ѡ���ļ�", "D:/", "����ļ�(*.csv);;�ı��ļ�(*.txt)");
	if (tempKeyPointsFile != "")
	{
		keyPointsFile = tempKeyPointsFile;
		bool flag = tableToCsv(ui.keyPointTable, keyPointsFile);
		if (flag)
		{
			QMessageBox::information(NULL, "��ʾ", "���ؼ������ݳɹ���");
		}
		else
		{
			QMessageBox::information(NULL, "��ʾ", "���ؼ�������ʧ�ܣ�");
		}
	} 
	ui.led_keyPointsFile->setText(keyPointsFile);
}

void SixUpsUiAo::addTableWidgetMenuKeyPoint()
{
	//��ȡ�źŷ�����
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());
	/*******************************************/
	//���ò˵�ѡ��
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *transPos = new QAction("���ؼ���ת����Ŀ��λ��", senderTableWidget);
	QAction *deleteRow = new QAction("ɾ����ǰ�ؼ�������", senderTableWidget);
	connect(transPos, &QAction::triggered, this, [=]() {
		qDebug() << "transPos";
		QTableWidgetItem * tmpItem = senderTableWidget->currentItem();
		if (tmpItem == Q_NULLPTR)
		{
			return;
		}
		else
		{
			int curRow = tmpItem->row();
			Matrix<double, 6, 1> keyPointPosAndAngle_DS = MatrixXd::Zero(6, 1);
			Matrix<double, 6, 1> keyPointPosAndAngle_setS = MatrixXd::Zero(6, 1);
			for (int i = 0; i < 6; i++)
			{
				keyPointPosAndAngle_DS(i) = senderTableWidget->item(curRow, i+1 )->text().toDouble();
			}
			keyPointPosAndAngle_setS = posAndAngleDS2setS(keyPointPosAndAngle_DS, UPSData::Trans_setD);
			for (int i = 0; i < 6; i++)
			{
				AbsTarPos_group[i]->setValue(keyPointPosAndAngle_setS(i));
			}
			ui.tabWidget->setCurrentIndex(2);
		}

	});
	connect(deleteRow, &QAction::triggered, this, [=]() {
		qDebug() << "deleteRow";
		QTableWidgetItem * tmpItem = senderTableWidget->currentItem();
		if (tmpItem == Q_NULLPTR)
		{
			return;
		}
		else
		{
			senderTableWidget->removeRow(tmpItem->row());//ɾ����
		}
	});
	tableWidgetMenu->addAction(transPos);
	tableWidgetMenu->addAction(deleteRow);
	tableWidgetMenu->move(cursor().pos());
	tableWidgetMenu->show();
}

void SixUpsUiAo::btnGroupMultiSpeed_clicked(int id)
{
	switch (id)
	{
	case 1:
		UPSData::multiSpeed = 1;
		break;
	case 2:
		UPSData::multiSpeed = 2.5;
		break;
	case 3:
		UPSData::multiSpeed = 4;
		break;
	default:
		break;
	}
}

void SixUpsUiAo::on_getRealTimePosBtn_clicked()
{
	for (int i = 0; i < 6; i++)
	{
		AbsTarPos_group[i]->setValue(UPSData::curPosAndAngle_setS(i));
	}
	qDebug() << "on_getRealTimePosBtn_clicked";
}

void SixUpsUiAo::on_startMoveBtn_clicked()
{
	for (int i = 0; i < 6; i++)
	{
		UPSData::tarPosAndAngle_setS(i) = AbsTarPos_group[i]->value();
	}
	//������˳�
	inverseSolution(UPSData::tarPosAndAngle_setS, UPSData::tarL_norm, UPSData::D_set, UPSData::S);
	//�˳�����ʼ�˳��õ�PMAC�˳�
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsAbsMove(UPSData::tarAxlesL_norm, UPSData::multiSpeed);
}

void SixUpsUiAo::on_stopMoveBtn_clicked()
{
	myPmac->jogStop();
}


void SixUpsUiAo::absTarPos_group_valueChanged(double data)
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "xAbsTarPos";
	int posNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//����λ�� x y z a b c��˳���
	int index = posNum - 1;//��ż�1����������
	UPSData::tarPosAndAngle_Dset[index] = data;
	qDebug() << "posNum:" << posNum << " tarPosAndAngle_Dset:" << UPSData::tarPosAndAngle_Dset[index];
}


void SixUpsUiAo::btnGroupMultiAxisDirection_clicked(int id)
{
	UPSData::multiJogMoveDirectionID = id;
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
	switch (UPSData::multiJogMoveDirectionID)
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
		incPosAndAngle(3) = UPSData::multiJogRotateStep;
		qDebug() << "aRBtn";
		break;
	case 5:
		incPosAndAngle(4) = UPSData::multiJogRotateStep;
		qDebug() << "bRBtn";
		break;
	case 6:
		incPosAndAngle(5) = UPSData::multiJogRotateStep;
		qDebug() << "cRBtn";
		break;
	default:
		qDebug() << "on_disMultiAxisJog_clicked ERROR!";
		break;
	}
	//�˶�����ϵ��Ծ�ƽ̨��Ŀ��λ��
	UPSData::tarPosAndAngle_setS = UPSData::curPosAndAngle_setS + incPosAndAngle;
	//����
	inverseSolution(UPSData::tarPosAndAngle_setS, UPSData::tarL_norm, UPSData::D_set, UPSData::S);
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;

	double moveTime = 30;
	if (UPSData::multiJogMoveDirectionID<=3)
	{
		//ƽ��
		moveTime = abs(UPSData::multiJogTranslationStep / UPSData::multiJogTranslationSpeed);
	} 
	else
	{
		//ת��
		moveTime = abs(UPSData::multiJogRotateStep / UPSData::multiJogRotateSpeed);
	}
	double vel = (UPSData::tarL_norm - UPSData::curL_norm).norm() / moveTime;
	myPmac->upsAbsMove(UPSData::tarAxlesL_norm, vel);
	
}

void SixUpsUiAo::on_disMultiAxisJog_stop_clicked()
{
	myPmac->jogStop();
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_pressed()
{

	UPSData::prsPosAndAngle_setS = UPSData::curPosAndAngle_setS;
	UPSData::lastAxlesL_norm = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//�˶���������
	/*�����˶�����*/
	switch (UPSData::multiJogMoveDirectionID)
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
	/*�����˶�������*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (UPSData::multiJogMoveDirectionID <= 3)
	{
		//ƽ��
		UPSData::multiJogMoveStep = UPSData::multiJogTranslationSpeed * upsJogTimer->interval() / 1000.0; //��С�˶�����
	}
	else
	{
		//ת��
		UPSData::multiJogMoveStep = UPSData::multiJogRotateSpeed * upsJogTimer->interval() / 1000.0;//��С�˶�����
	}
	upsJogTimer->start();	
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_released()
{

	upsJogTimer->stop();
}

void SixUpsUiAo::on_prsmultiAxisJogPos_pressed()
{
	UPSData::prsPosAndAngle_setS = UPSData::curPosAndAngle_setS;
	UPSData::lastAxlesL_norm = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//�˶���������
	/*�����˶�����*/
	switch (UPSData::multiJogMoveDirectionID)
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
	/*�����˶�������*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (UPSData::multiJogMoveDirectionID <= 3)
	{
		//ƽ��
		UPSData::multiJogMoveStep = UPSData::multiJogTranslationSpeed * upsJogTimer->interval() / 1000.0;//�˶�����
	}
	else
	{
		//ת��
		UPSData::multiJogMoveStep = UPSData::multiJogRotateSpeed * upsJogTimer->interval() / 1000.0;//�˶�����
	}
	upsJogTimer->start();
}

void SixUpsUiAo::on_prsmultiAxisJogPos_released()
{

	upsJogTimer->stop();
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
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	int index = axleNum - 1;//��ż�1����������
	SingleJogData::jogInc[index] = Inc;
	qDebug() << "axleNum:" << axleNum << " jogInc:" << SingleJogData::jogInc[index];

}

void SixUpsUiAo::dipJog_Clicked()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "dipJog";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	int index = axleNum - 1;//��ż�1����������

	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//�������˶��ٶ�
	myPmac->jogDisp(axleNum, SingleJogData::jogInc[index]);//�پ���㶯
}

void SixUpsUiAo::prsJogPos_pressed()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogPos";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	qDebug() << "axleNum:" << axleNum << " prsJogPos_pressed";
	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//�������˶��ٶ�
	myPmac->jogPosContinuously(axleNum);

}

void SixUpsUiAo::prsJogPos_released()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogPos";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	myPmac->jogStop(axleNum);
	qDebug() << "axleNum:" << axleNum << " prsJogPos_released";
}

void SixUpsUiAo::prsJogNeg_pressed()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogNeg";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	qDebug() << "axleNum:" << axleNum << " prsJogNeg_pressed";
	myPmac->setJogSpeed(axleNum, SingleJogData::jogSpeed);//�������˶��ٶ�
	myPmac->jogNegContinuously(axleNum);
}

void SixUpsUiAo::prsJogNeg_released()
{
	QString btnName = QObject::sender()->objectName();
	QString btnNamePrefix = "prsJogNeg";
	int axleNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//���
	myPmac->jogStop(axleNum);
	qDebug() << "axleNum:" << axleNum << " prsJogNeg_released";
}

void SixUpsUiAo::wirteRegisters_slot(const int & address, const double & data)
{
	switch (address)
	{
	case 1:
		ui.led_multiAxisJogTranslationSpeed->setValue(data);
		break;
	case 3:
		ui.led_multiAxisJogTranslationStep->setValue(data);
		break;
	case 5:
		ui.led_multiAxisJogRotateSpeed->setValue(data);
		break;
	case 7:
		ui.led_multiAxisJogRotateStep->setValue(data);
		break;
	case 9:
	{
		int tempID = data;
		btnGroupMultiAxisDirection->button(tempID)->click();
		break;
	}
	default:
		break;
	}
}

void SixUpsUiAo::writeCoils_slot(const int & address, const bool & flag)
{
	switch (address)
	{
	case 1:
		if (flag)
		{
			myPmac->jogStop();
			qDebug() << "move stop";
		}
		break;
	case 2:
		if (flag)
		{
			ui.disMultiAxisJog->click();
			qDebug() << "distance move";
		}
		break;
	case 3:
		if (flag)
		{
			ui.prsmultiAxisJogPos->pressed();
			qDebug() << "+press move start";
		}
		else
		{
			ui.prsmultiAxisJogPos->released();
			qDebug() << "+press move stop";
		}
		break;
	case 4:
		if (flag)
		{
			ui.prsMultiAxisJogNeg->pressed();
			qDebug() << "-press move start";
		}
		else
		{
			ui.prsMultiAxisJogNeg->released();
			qDebug() << "-press move stop";
		}
		break;
	case 5:
		if (flag)
		{
			ui.recordKeyPointBtn->click();
			qDebug() << "ui.recordKeyPointBtn->click();";
		}
		break;
	default:
		break;
	}
}

void SixUpsUiAo::on_inputPointsBtn_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "ѡ���ļ�", "D:/", "�ı��ļ�(*.txt;*.csv;)");
	Vector2d csvSize = getCsvSize(strFile);
	
	if (strFile != "")
	{
		if (csvSize(0) > 3 )
		{
			QMessageBox::information(NULL, "��ʾ", "�������ݵ��ļ�������ӦС�ڵ���3");
		}
		else
		{
			csvToTableAdapt(strFile, ui.tableInputPoints);
		}
	}
}

void SixUpsUiAo::on_inputMatBtn_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "ѡ���ļ�", "D:/", "�ı��ļ�(*.txt;*.csv;)");
	Vector2d csvSize = getCsvSize(strFile);
	if (strFile!="")
	{
		if (csvSize(0) != 4 || csvSize(1) != 4)
		{
			QMessageBox::information(NULL, "��ʾ", "��������ά�ȴ���ӦΪ4��4����");
		}
		else
		{
			csvToTable(strFile, ui.tableInputMat4d);
		}
	} 

}

void SixUpsUiAo::on_Mat2PosBtn_clicked()
{
	Matrix4d mat = Matrix4d::Identity();
	//�жϱ�����Ƿ��п�ֵ
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (ui.tableInputMat4d->item(i, j) == NULL || ui.tableInputMat4d->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "��ʾ", "����д��ڿ�ֵ��");
				return;
			}
		}
	}

	bool flag = tableToMatrixXd(ui.tableInputMat4d , mat);

	if (flag)
	{
		//��Ŀ���ھ�ϵ�е�λ��
		Matrix<double, 6, 1> PosAndAngle_matS = trans2PosAngle(mat);

		//��Ŀ�����˶�����ϵ�ھ�ϵ�е�λ��
		//Matrix<double, 6, 1> PosAndAngle_matsetS = posAndAngleDS2setS(PosAndAngle_matS, UPSData::Trans_setD);
		for (int i = 0; i < 6; i++)
		{
			AbsTarPos_group[i]->setValue(PosAndAngle_matS(i));
		}
		
		QMessageBox::information(NULL, "��ʾ", "ת����Ŀ��λ�˳ɹ���");
		ui.tabWidget->setCurrentIndex(2);
	} 
	else
	{
		QMessageBox::information(NULL, "��ʾ", "ת����Ŀ��λ��ʧ�ܡ�");
	}

}

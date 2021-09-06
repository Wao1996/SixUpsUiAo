#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	qDebug() << "SixUpsUiAo ����";

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
	//connect(calculateQthread, &QThread::started, myPmacThread, &QPmacThread::startPmac);
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

	realTimePose_Dset_group.append(ui.led_realTimeX_Dset);
	realTimePose_Dset_group.append(ui.led_realTimeY_Dset);
	realTimePose_Dset_group.append(ui.led_realTimeZ_Dset);
	realTimePose_Dset_group.append(ui.led_realTimeRdeg_Dset);
	realTimePose_Dset_group.append(ui.led_realTimePdeg_Dset);
	realTimePose_Dset_group.append(ui.led_realTimeYdeg_Dset);



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
	csvToMatrixXd("./Data/�ṹ����/D.csv", UPSData::D);
	csvToMatrixXd("./Data/�ṹ����/D_theoretical.csv", UPSData::D_theoretical);
	csvToMatrixXd("./Data/�ṹ����/S.csv", UPSData::S);
	csvToMatrixXd("./Data/�ṹ����/S_theoretical.csv", UPSData::S_theoretical);
	csvToMatrixXd("./Data/�ṹ����/Q_DD.csv", UPSData::Q_DD);
	csvToMatrixXd("./Data/�ṹ����/Q_SS.csv", UPSData::Q_SS);
	csvToMatrixXd("./Data/�ṹ����/initL_norm.csv", UPSData::initL_norm);
	csvToMatrixXd("./Data/ƽ̨����/homePosAndAngle.csv", UPSData::homePosAndAngle_DS);
	UPSData::initPosAndAngle_DS = UPSData::homePosAndAngle_DS;

	//����ؼ���
	inputKeyPoint("./Data/ƽ̨����/keyPoint.csv", ui.keyPointTable);
	
}

void SixUpsUiAo::initTablesStyle()
{
	int rowHeight = 30;//�и�
	int colWidth = 80;//�п�
	int scrollBarHeight = 20;//��������
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableSetOrigin ԭ������******/
	//������
	ui.tableSetOrigin->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableSetOrigin->setColumnCount(3);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "O" << "X" << "XoY" ;
	ui.tableSetOrigin->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);

	//������
	ui.tableSetOrigin->setRowCount(3);
	ui.tableSetOrigin->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableSetOrigin->setVerticalHeaderLabels(VerticalHeaderXYZ);
	ui.tableSetOrigin->verticalHeader()->setFont(QFont("����", 12));
	ui.tableSetOrigin->verticalHeader()->setFixedWidth(25);
	ui.tableSetOrigin->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableSetOrigin->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableSetOrigin->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableSetOrigin->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableSetOrigin->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableSetOrigin->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableSetOrigin->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableSetOrigin->setFixedHeight(4 * rowHeight);

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
	tableSMVerticalHeader <<"���µ����"<< "X����" << "Y����" << "Z����";
	//������
	ui.tableSM->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableSM->setColumnCount(3);
	ui.tableSM->horizontalHeader()->setVisible(false);//�кſɼ�
	ui.tableSM->horizontalHeader()->setStretchLastSection(false);//��ͷ����Ӧ���
	//������
	ui.tableSM->setRowCount(tableSMVerticalHeader.size());
	ui.tableSM->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableSM->setVerticalHeaderLabels(tableSMVerticalHeader);
	ui.tableSM->verticalHeader()->setFont(QFont("����", 10));
	ui.tableSM->verticalHeader()->setFixedWidth(100);
	ui.tableSM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableSM->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	//��С����
	ui.tableSM->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableSM->setFixedHeight(4 * rowHeight + scrollBarHeight);
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
	//��¼�ؼ������Ҽ�
	ui.keyPointTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.keyPointTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuKeyPoint()));
}

void SixUpsUiAo::myWidgetEnable()
{
	ui.servoOnBtn->setEnabled(true);
	ui.servoOffBtn->setEnabled(true);
	ui.axlesHomeBtn->setEnabled(true);
	ui.upsHomeBtn->setEnabled(true);
	ui.tabWidget->setEnabled(true);
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
		realTimePose_DS_group[i]->setText("");
		realTimePose_setS_group[i]->setText("");
		realTimePose_Dset_group[i]->setText("");
	}

	ui.pmacSta->setPixmap(offIcon);
	ui.pmacInitSta->setPixmap(offIcon);
	ui.connectPmacBtn->setText("����");
	ui.initPmacBtn->setEnabled(false);
	ui.servoOnBtn->setEnabled(false);
	ui.servoOffBtn->setEnabled(false);
	ui.axlesHomeBtn->setEnabled(false);
	ui.upsHomeBtn->setEnabled(false);
	ui.tabWidget->setEnabled(false);
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
	
	matrixXdToCsv(PmacData::curLengthsMM, "./Data/ƽ̨����/lastCurLengthsMM.csv");
}

bool SixUpsUiAo::loadLastLengthsMM()
{
	Matrix<double, 6, 1> lastCurLengthsMM = MatrixXd::Zero(6, 1);
	bool flag = csvToMatrixXd("./Data/ƽ̨����/lastCurLengthsMM.csv", lastCurLengthsMM);
	if (flag)
	{
		// pmac��ʼʱ�˳�����
		myPmac->setCurLengths(lastCurLengthsMM);
		DeleteFileOrFolder("./Data/ƽ̨����/lastCurLengthsMM.csv");
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
		strPos = QString::number(UPSData::curPosAndAngle_Dset(i), 'f', 3);
		realTimePose_Dset_group[i]->setText(strPos);
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
	Matrix<double, 6, 1> tarL_norm;//Ŀ��˳�
	//��ƽ̨Ŀ��λ�� ��� �˶�����ϵ��λ������
	UPSData::prsPosAndAngle_Dset = UPSData::prsPosAndAngle_Dset + UPSData::multiJogMoveStep * UPSData::multiJogMoveDirection;
	//��ƽ̨Ŀ��λ�� ��� ������ϵ��λ������
	Matrix<double, 6, 1> tarPosAndAngle_DS = posAndAngleDset2DS(UPSData::prsPosAndAngle_Dset, UPSData::Trans_setS);
	//�˶�ѧ���ⷴ��
	inverseSolution(tarPosAndAngle_DS, tarL_norm, UPSData::D, UPSData::S);
	//��PMAC�˳�
	UPSData::tarAxlesL_norm = tarL_norm - UPSData::initL_norm;
	//������һ�θ��˳�������һ�θ��˳��ȵĲ�ֵ
	delta_Lengths = UPSData::tarAxlesL_norm - UPSData::lastAxlesL_norm;
	//�ø��˳���ֵ������˵��˶��ٶ� ��֤ÿ����ͬһʱ�䵽��Ŀ��λ��
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
	//myPmac->enablePLC(1);
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
	emit upsHome_signal();
}

void SixUpsUiAo::on_calibrateSMBtn_clicked()
{
	int numCol = ui.tableSM->columnCount();
	MatrixXd Q_SM_temp = MatrixXd::Zero(3, numCol);
	MatrixXd Q_SS_temp = MatrixXd::Zero(3, numCol);
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
}


void SixUpsUiAo::on_setOriginBtn_clicked()
{
	qDebug() << "on_creatDCrdSys_clicked";
	MatrixXd CrdSysD = Matrix3d::Zero();
	vector<int> index;
	tableToMatrixXd(ui.tableSetOrigin, CrdSysD, index);
	UPSData::O_set_M = CrdSysD.col(0);
	UPSData::X_set_M = CrdSysD.col(1);
	UPSData::XOY_set_M = CrdSysD.col(2);
	//�˶�����ϵ�ڲ�������ϵ�е��������
	Matrix4d Trans_set_M = creatCoordSysGetRt(UPSData::O_set_M, UPSData::X_set_M, UPSData::XOY_set_M);
	//���˶�����ϵת����������ϵ������
	UPSData::Trans_setS = UPSData::Trans_SM.inverse()*Trans_set_M;
	UPSData::curPosAndAngle_setS = trans2PosAngle(UPSData::Trans_setS);
	
}

void SixUpsUiAo::on_setSPosOriginBtn_clicked()
{
	UPSData::Trans_setS = Matrix4d::Identity();
	UPSData::curPosAndAngle_setS = trans2PosAngle(UPSData::Trans_setS);
}

void SixUpsUiAo::on_setCurPosOriginBtn_clicked()
{
	UPSData::Trans_setS = UPSData::Trans_DS;
	UPSData::curPosAndAngle_setS = trans2PosAngle(UPSData::Trans_setS);

}

void SixUpsUiAo::on_recordKeyPointBtn_clicked()
{
	int tabRow = ui.keyPointTable->rowCount();
	int tabCol = ui.keyPointTable->columnCount();
	ui.keyPointTable->insertRow(tabRow);
	qDebug() << tabRow;
	//�ؼ������
	ui.keyPointTable->setItem(tabRow , 0, new QTableWidgetItem(QString::number(tabRow + 1)));
	//����ǰ��ƽ̨��Ծ�ƽ̨λ�˼�¼
	for (int i = 0; i < 6; i++ )
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(UPSData::curPosAndAngle_DS(i)));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		ui.keyPointTable->setItem(tabRow , i + 1, item);
	}
	
}

void SixUpsUiAo::on_saveKeyPointBtn_clicked()
{
	bool flag = tableToCsv(ui.keyPointTable, "./Data/ƽ̨����/keyPoint.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "����ؼ������ݳɹ���");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "����ؼ�������ʧ�ܣ�");
	}
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
		int curRow = tmpItem->row();
		Matrix<double, 6, 1> keyPointPosAndAngle_DS = MatrixXd::Zero(6, 1);
		Matrix<double, 6, 1> keyPointPosAndAngle_Dset = MatrixXd::Zero(6, 1);
		for (int i = 0; i < 6; i++)
		{
			keyPointPosAndAngle_DS(i) = senderTableWidget->item(curRow, i+1 )->text().toDouble();
		}
		keyPointPosAndAngle_Dset = posAndAngleDS2Dset(keyPointPosAndAngle_DS, UPSData::Trans_setS);
		for (int i = 0; i < 6; i++)
		{
			AbsTarPos_group[i]->setValue(keyPointPosAndAngle_Dset(i));
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

void SixUpsUiAo::on_getRealTimePosBtn_clicked()
{
	for (int i = 0; i < 6; i++)
	{
		AbsTarPos_group[i]->setValue(UPSData::curPosAndAngle_Dset(i));
	}
	qDebug() << "on_getRealTimePosBtn_clicked";
}

void SixUpsUiAo::on_startMoveBtn_clicked()
{
	for (int i = 0; i < 6; i++)
	{
		UPSData::tarPosAndAngle_Dset(i) = AbsTarPos_group[i]->value();
	}
	Matrix<double, 6, 1> tarPosAndAngle_DS = posAndAngleDset2DS(UPSData::tarPosAndAngle_Dset, UPSData::Trans_setS);
	//������˳�
	inverseSolution(tarPosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
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
	qDebug() << "Step:" ;
	cout << incPosAndAngle << endl;
	//��ƽ̨Ŀ��λ������˶�����ϵ��λ������
	UPSData::tarPosAndAngle_Dset = UPSData::curPosAndAngle_Dset + incPosAndAngle;
	qDebug() << "tarPosAndAngle_Dset:";
	cout << UPSData::tarPosAndAngle_Dset << endl;
	//��ƽ̨Ŀ��λ��Ծ�����ϵ��λ������
	Matrix<double, 6, 1> tarPosAndAngle_DS = posAndAngleDset2DS(UPSData::tarPosAndAngle_Dset, UPSData::Trans_setS);
	qDebug() << "tarPosAndAngle_DS:";
	cout << tarPosAndAngle_DS << endl;
	//����
	inverseSolution(tarPosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;
	myPmac->upsAbsMove(UPSData::tarAxlesL_norm, UPSData::multiJogTranslationSpeed);
	
}

void SixUpsUiAo::on_disMultiAxisJog_stop_clicked()
{
	myPmac->jogStop();
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_pressed()
{

	UPSData::prsPosAndAngle_Dset = UPSData::curPosAndAngle_Dset;
	UPSData::lastAxlesL_norm = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//�˶���������
	/*�����˶�����*/
	int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//������Ǹ�����
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
	/*�����˶�������*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (MultiAxisDirectionID <= 3)
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

	UPSData::prsPosAndAngle_Dset = UPSData::curPosAndAngle_Dset;
	UPSData::lastAxlesL_norm = UPSData::curL_norm - UPSData::initL_norm;
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//�˶���������
	/*�����˶�����*/
	int MultiAxisDirectionID = btnGroupMultiAxisDirection->checkedId();//������Ǹ�����
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
	/*�����˶�������*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (MultiAxisDirectionID <= 3)
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


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
	
	/*Pmac数据采集定时器*/
	dataGatherTimer = new QTimer(this);
	dataGatherTimer->setInterval(100);
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
	upsCalculateTimer->setInterval(200);
	upsCalculateTimer->setTimerType(Qt::CoarseTimer);

	/*并联机构长按点动定时器*/
	upsJogTimer = new QTimer(this);
	upsJogTimer->setInterval(100);
	upsJogTimer->setTimerType(Qt::PreciseTimer);
	connect(upsJogTimer, &QTimer::timeout, this, &SixUpsUiAo::on_upsJogTimer);

	/*计算线程*/
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
	qDebug() << "SixUpsUiAo 析构";

	//pmac连接时直接关闭界面
	if (GlobalSta::pmacIsConnected)
	{
		//记录当前位姿
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
	/*隐藏*/
	ui.tabWidget->removeTab(4);
	ui.mainToolBar->setVisible(false);//隐藏工具栏
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
	//导入动静平台结构参数
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/D_new.csv", UPSData::D);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/D_theoretical_new.csv", UPSData::D_theoretical);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/S_new.csv", UPSData::S);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/S_theoretical_new.csv", UPSData::S_theoretical);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/Q_DD.csv", UPSData::Q_DD);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/Q_SS.csv", UPSData::Q_SS);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/结构参数/initL_norm_new.csv", UPSData::initL_norm);
	csvToMatrixXd(GlobalSta::dataFile + "/Data/平台参数/homePosAndAngle_new.csv", UPSData::homePosAndAngle_DS);
	UPSData::initPosAndAngle_DS = UPSData::homePosAndAngle_DS;
	UPSData::D_set = UPSData::D;
	
}

void SixUpsUiAo::initTablesStyle()
{
	int rowHeight = 30;//行高
	int colWidth = 80;//列宽
	int scrollBarHeight = 20;//滚动条高
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";

	/*****keyPointTable 待拟合参数******/
	//列设置
	ui.keyPointTable->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.keyPointTable->setColumnCount(7);
	QStringList keyPointTableHorizontalHeader;
	keyPointTableHorizontalHeader << "关键点" << "X/mm" << "Y/mm" << "Z/mm" << "绕X/°" << "绕Y/°" << "绕Z/°";
	ui.keyPointTable->setHorizontalHeaderLabels(keyPointTableHorizontalHeader);
	ui.keyPointTable->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.keyPointTable->horizontalHeader()->setFont(QFont("黑体", 10));//表头字体
	ui.keyPointTable->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.keyPointTable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//行设置
	ui.keyPointTable->setRowCount(0);
	ui.keyPointTable->verticalHeader()->setVisible(false);//行号可见
	ui.keyPointTable->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.keyPointTable->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.keyPointTable->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式

	/*********************tableSM*****************/
	
	QStringList tableSMVerticalHeader;
	tableSMVerticalHeader <<"静靶点序号"<< "X坐标" << "Y坐标" << "Z坐标";
	//列设置
	ui.tableSM->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableSM->setColumnCount(3);
	ui.tableSM->horizontalHeader()->setVisible(false);//列号不可见
	ui.tableSM->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	//行设置
	ui.tableSM->setRowCount(tableSMVerticalHeader.size());
	ui.tableSM->verticalHeader()->setVisible(true);//行号可见
	ui.tableSM->setVerticalHeaderLabels(tableSMVerticalHeader);
	ui.tableSM->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableSM->verticalHeader()->setFixedWidth(120);
	ui.tableSM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableSM->setAlternatingRowColors(true);//行奇偶颜色不同
	//大小设置
	ui.tableSM->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableSM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableSM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	/*********************tableGM*****************/
	QStringList tableGMVerticalHeader;
	tableGMVerticalHeader << "X坐标" << "Y坐标" << "Z坐标";
	QStringList tableGMHorizontalHeader;
	tableGMHorizontalHeader << "工装靶点1" << "工装靶点2" << "工装靶点3";
	//列设置
	ui.tableGM->horizontalHeader()->setDefaultSectionSize(100);
	ui.tableGM->setColumnCount(3);
	ui.tableGM->setHorizontalHeaderLabels(tableGMHorizontalHeader);
	ui.tableGM->horizontalHeader()->setVisible(true);//列号可见
	ui.tableGM->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	ui.tableGM->horizontalHeader()->setFont(QFont("黑体", 10));//表头字体
	ui.tableGM->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableGM->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//行设置
	ui.tableGM->setRowCount(tableGMVerticalHeader.size());
	ui.tableGM->verticalHeader()->setVisible(true);//行号可见
	ui.tableGM->setVerticalHeaderLabels(tableGMVerticalHeader);
	ui.tableGM->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableGM->verticalHeader()->setFixedWidth(120);
	ui.tableGM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableGM->setAlternatingRowColors(true);//行奇偶颜色不同
	//大小设置
	ui.tableGM->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableGM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableGM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*********************tableTarGM*****************/

	QStringList tableTarGMVerticalHeader;
	tableTarGMVerticalHeader << "X坐标" << "Y坐标" << "Z坐标";
	QStringList tableTarGMHorizontalHeader;
	tableTarGMHorizontalHeader << "工装靶点1" << "工装靶点2" << "工装靶点3";
	//列设置
	ui.tableTarGM->horizontalHeader()->setDefaultSectionSize(100);
	ui.tableTarGM->setColumnCount(3);
	ui.tableTarGM->setHorizontalHeaderLabels(tableTarGMHorizontalHeader);
	ui.tableTarGM->horizontalHeader()->setVisible(true);//列号不可见
	ui.tableTarGM->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	ui.tableTarGM->horizontalHeader()->setFont(QFont("黑体", 10));//表头字体
	ui.tableTarGM->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableTarGM->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//行设置
	ui.tableTarGM->setRowCount(tableTarGMVerticalHeader.size());
	ui.tableTarGM->verticalHeader()->setVisible(true);//行号可见
	ui.tableTarGM->setVerticalHeaderLabels(tableTarGMVerticalHeader);
	ui.tableTarGM->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableTarGM->verticalHeader()->setFixedWidth(120);
	ui.tableTarGM->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableTarGM->setAlternatingRowColors(true);//行奇偶颜色不同
	//大小设置
	ui.tableTarGM->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableTarGM->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableTarGM->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*tableInputPoints*/
	QStringList tableInputVerticalHeader;
	tableInputVerticalHeader << "X坐标" << "Y坐标" << "Z坐标";
	//列设置
	ui.tableInputPoints->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableInputPoints->setColumnCount(4);
	ui.tableInputPoints->horizontalHeader()->setVisible(true);//列号不可见
	ui.tableInputPoints->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	//行设置
	ui.tableInputPoints->setRowCount(tableInputVerticalHeader.size());
	ui.tableInputPoints->verticalHeader()->setVisible(true);//行号可见
	ui.tableInputPoints->setVerticalHeaderLabels(tableInputVerticalHeader);
	ui.tableInputPoints->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableInputPoints->verticalHeader()->setFixedWidth(80);
	ui.tableInputPoints->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableInputPoints->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableInputPoints->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableInputPoints->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//大小设置
	ui.tableInputPoints->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableInputPoints->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableInputPoints->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	/*tableInputMat4d*/
	
	//列设置
	ui.tableInputMat4d->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽平均
	ui.tableInputMat4d->setColumnCount(4);
	ui.tableInputMat4d->horizontalHeader()->setVisible(false);//列号不可见
	//ui.tableInputMat4d->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	//行设置
	ui.tableInputMat4d->setRowCount(4);
	ui.tableInputMat4d->verticalHeader()->setVisible(false);//行号可见
	ui.tableInputMat4d->setVerticalHeaderLabels(tableInputVerticalHeader);
	ui.tableInputMat4d->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableInputMat4d->verticalHeader()->setFixedWidth(80);
	ui.tableInputMat4d->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableInputMat4d->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableInputMat4d->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableInputMat4d->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//大小设置
	ui.tableInputMat4d->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableInputMat4d->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableInputMat4d->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void SixUpsUiAo::initConnect()
{
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
	//多轴运动选择运动方向
	connect(btnGroupMultiAxisDirection, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SixUpsUiAo::btnGroupMultiAxisDirection_clicked);
	//多轴联动运动速度
	connect(btnGroupMultiSpeed, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SixUpsUiAo::btnGroupMultiSpeed_clicked);
	//记录关键点表格右键
	ui.keyPointTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.keyPointTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuKeyPoint()));
	//测量准备右键添加列
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
	/*让相关状态显示UI初始化*/
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
	ui.connectPmacBtn->setText("连接");
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

bool SixUpsUiAo::inputKeyPoint(const QString & filePath, QTableWidget * tab)
{
	QFile keyPointFile(filePath);
	//获csv取行列数
	if (!keyPointFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
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

	//获取table行列数
	int tabRow = tab->rowCount();
	int tabCol = tab->columnCount();

	//若csv行数大于当前表格列数则扩展表格列数
	if (csvRowCount > tabRow)
	{
		qDebug() << "自动扩展行数->" << csvColCount;
		for (int i = 0; i < csvRowCount - tabRow; i++)
		{
			tab->insertRow(i + tabRow);
			//QTableWidgetItem * tmpItem = new QTableWidgetItem();
			//for (int j = 0; j < csvColCount; j++)
			//{
			//	tab->setItem(i + tabRow,j, new QTableWidgetItem());//添加新元素
			//}
		}
	}

	//从csv文件读取数据
	if (!keyPointFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream keyPointStream(&keyPointFile);
	QString line;
	QStringList fields;
	for (int i = 0; i < csvRowCount; i++)
	{
		line = keyPointStream.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			QTableWidgetItem *item = new QTableWidgetItem(fields[j]);
			if (j!=0)
			{
				//设置不可编辑
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
	
	matrixXdToCsv(PmacData::curLengthsMM, GlobalSta::dataFile + "/Data/平台参数/lastCurLengthsMM.csv");
}

bool SixUpsUiAo::loadLastLengthsMM()
{
	Matrix<double, 6, 1> lastCurLengthsMM = MatrixXd::Zero(6, 1);
	bool flag = csvToMatrixXd(GlobalSta::dataFile+"/Data/平台参数/lastCurLengthsMM.csv", lastCurLengthsMM);
	if (flag)
	{
		// pmac初始时杆长设置
		myPmac->setCurLengths(lastCurLengthsMM);
		DeleteFileOrFolder(GlobalSta::dataFile+"/Data/平台参数/lastCurLengthsMM.csv");
		return true;
	} 
	else
	{
		return false;
	}
}

bool SixUpsUiAo::DeleteFileOrFolder(const QString & strPath)
{
	if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
		return false;

	QFileInfo FileInfo(strPath);

	if (FileInfo.isFile())//如果是文件
		QFile::remove(strPath);
	else if (FileInfo.isDir())//如果是文件夹
	{
		QDir qDir(strPath);
		qDir.removeRecursively();
	}
	return true;
}

void SixUpsUiAo::upsHome_slot()
{
	qDebug() << "upsHome_slot ";
	/******弹出等待窗口*****/
	WaitWindowUI = new WaitWindow();
	WaitWindowUI->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
	WaitWindowUI->show();
	/******各轴开始回零*****/
	myPmac->setHomeCompleteZero();//各轴回零完成位清零
	myPmac->axlesHomeMove();
	/******检测各轴是否归零计时器开始计时*****/
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

		strLength = QString::number(PmacData::curForce(i), 'f', 3); 
		realTimeForce_group[i]->setText(strLength);
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
		//动平台相对静平台
		strPos = QString::number(UPSData::curPosAndAngle_DS(i), 'f', 3);
		realTimePose_DS_group[i]->setText(strPos);
		//运动坐标系相对静平台
		strPos = QString::number(UPSData::curPosAndAngle_setS(i), 'f', 3);
		realTimePose_setS_group[i]->setText(strPos);
		//动平台相对运动坐标系
		strPos = QString::number(UPSData::curPosAndAngle_setD(i), 'f', 3);
		realTimePose_setD_group[i]->setText(strPos);
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
		qDebug() << "P2=1";
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
		GlobalSta::axlesIshome = false;
		GlobalSta::upsIsHome = false;
		upsHomeSignalEmited = false;
		WaitWindowUI->close();//关闭等待对话框
		myPmac->setPvariable(2, 0); //将P2置0
		myWidgetEnable();
		upsHomeCompleteTimer->stop();
		qDebug() << "WaitWindowUI->close() ";
	}
}

void SixUpsUiAo::on_upsJogTimer()
{

	/***********采用在线命令形式***********/
	Matrix<double, 6, 1> delta_Lengths;//本次运动每个轴的运动增量 mm
	Matrix<double, 6, 1> axlesSpeed;//每个轴的运动速度 mm/s
	Matrix<double, 6, 1> tarL_norm;//目标铰点杆长
	//运动坐标系相对静平台 下一步的位姿向量
	UPSData::prsPosAndAngle_setS = UPSData::prsPosAndAngle_setS + UPSData::multiJogMoveStep * UPSData::multiJogMoveDirection;
	//运动学反解反解
	inverseSolution(UPSData::prsPosAndAngle_setS, tarL_norm, UPSData::D_set, UPSData::S);
	//求PMAC杆长
	UPSData::tarAxlesL_norm = tarL_norm - UPSData::initL_norm;
	//计算这一步各杆铰点长度与上一步各杆铰点长度的差值 得到 每根杆的变化量
	delta_Lengths = UPSData::tarAxlesL_norm - UPSData::lastAxlesL_norm;
	//用每根杆的变化量计算各杆的运动速度 保证每根杆同一时间到达目标位置
	axlesSpeed = delta_Lengths.cwiseAbs() * PmacData::cts2mm / upsJogTimer->interval();
	/*判断 如果有杆在限位位置时还想限位方向运动 则终止运动*/
	for (int i = 0; i < 6; i++)
	{
		if ((delta_Lengths(i) < 0 && PmacData::negLimitState(i) == 1) || (delta_Lengths(i) > 0 && PmacData::posLimitState(i) == 1))
		{
			qDebug() << "mutiAxlesJogNeg error:多轴运动长按遭遇 限位！";
			upsJogTimer->stop();
			return;
		}
	}
	myPmac->upsJogJMove(UPSData::tarAxlesL_norm, axlesSpeed);
	UPSData::lastAxlesL_norm = UPSData::tarAxlesL_norm;//更新

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
		myPmac->jogStop();
		myPmac->setServoOff();
		//记录当前杆长
		recordCurLengthsMM();
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
		ui.closeHMIBtn->click();
		GlobalSta::pmacIsInitialed = false;
		//让相关状态显示UI初始化
		myWidgetDisnable();
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
	bool ifHome = QMesBoxWhetherHome();//询问是否时候归零
	qDebug() << "on_initPmacBtn_clicked";
	if (GlobalSta::pmacIsInitialed)
	{
		dataGatherTimer->start();//数据采集开始
		upsCalculateTimer->start(); //开始计算并联机构实时位姿
		updateUiDataTimer->start();//开始更新UI
	}

	myPmac->jogStop();
	if (ifHome == true)
	{
		/*平台归零*/
		//1.运行回零程序
		//2.待各轴回零结束后 提示初始化完成
		emit upsHome_signal();
	}
	else
	{
		/*平台以当前位姿运动*/
		//1.从文件中读取上一次正常结束程序时的PMAC杆长
		//2.将上一次的PMAC杆长赋值给PMAC
		//3.提示初始化完成

		//导入上次杆长数据
		bool flagloadLastLengthsMM = loadLastLengthsMM();
		//如果初始化失败
		if (!flagloadLastLengthsMM)
		{
			int questionResult = QMessageBox::question(NULL, "提示", "加载上一次位姿失败,需将平台归零。原因可能为上一次程序未正常退出。是否归零？", QMessageBox::Yes | QMessageBox::No);
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
	//判断表格中是否有空值
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (ui.tableSM->item(i, j) == NULL || ui.tableSM->item(i, j)->text()=="")
			{
				QMessageBox::information(NULL, "提示", "表格中存在空值。");
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
	QMessageBox::information(NULL, "提示", "标定静平台靶标点成功！");
	flag_calibrateSM = true;
}

void SixUpsUiAo::on_calibrateGMBtn_clicked()
{
	int numCol = ui.tableGM->columnCount();
	//判断表格中是否有空值
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (ui.tableGM->item(i, j) == NULL || ui.tableGM->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "提示", "表格中存在空值。");
				return;
			}
		}
	}

	//求工装靶标点在动坐标系下的坐标
	vector<int> index;
	MatrixXd Q_GM;
	bool flag = tableToMatrixXd(ui.tableGM, Q_GM, index);
	if (flag)
	{
		MatrixXd  Q_GD_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_GM);
		UPSData::Q_GD = homogeneous2Matrix(Q_GD_homogeneous);
		cout << UPSData::Q_GD;
		QMessageBox::information(NULL, "提示", "标定工装靶标点成功！");
		flag_calibrateGM = true;
	} 
	else
	{
		QMessageBox::information(NULL, "提示", "标定工装靶标点失败！");
	}
	
}

void SixUpsUiAo::addTableWidgetMenuAddCoils()
{
	//获取信号发送者
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());

	/************根据表格名设定不同列标题**************/
	QString curTabName = senderTableWidget->objectName();//获取发送信号的表格名
	QStringList tabNameList;//各种表格名
	tabNameList << "tableSM" << "tableGM" << "tableTarGM";
	QString colTitle;//列标题
	switch (tabNameList.indexOf(curTabName))
	{
	case 0:
		colTitle = "测点";
		break;
	case 1:
		colTitle = "工装靶点";
		break;
	case 2:
		colTitle = "动靶点";
		break;
	default:
		break;
	}
	/*******************************************/
	//设置菜单选项
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *addColumn = new QAction("添加列", senderTableWidget);
	QAction *deleteColumn = new QAction("删除列", senderTableWidget);
	QAction *clearContents = new QAction("清空所有内容", senderTableWidget);
	connect(addColumn, &QAction::triggered, this, [=]() {
		qDebug() << "addCol";
		int rows = senderTableWidget->rowCount();//获取行列数
		int cols = senderTableWidget->columnCount();
		senderTableWidget->insertColumn(cols);//插入列
		QTableWidgetItem * tmpItem = new QTableWidgetItem();
		senderTableWidget->setHorizontalHeaderItem(cols, tmpItem);//设置列标题
		QTableWidgetItem * colItem = senderTableWidget->horizontalHeaderItem(cols);
		colItem->setText(colTitle + QString::number(cols + 1));
		for (int i = 0; i < rows; i++)
		{
			senderTableWidget->setItem(i, cols, new QTableWidgetItem());//添加新元素
		}
		senderTableWidget->selectColumn(cols);//选中新列

		if (tabNameList.indexOf(curTabName) == 1)
		{
			ui.tableTarGM->insertColumn(cols);//插入列
			tmpItem = new QTableWidgetItem();
			ui.tableTarGM->setHorizontalHeaderItem(cols, tmpItem);//设置列标题
			colItem = ui.tableTarGM->horizontalHeaderItem(cols);
			colItem->setText(colTitle + QString::number(cols + 1));
			for (int i = 0; i < rows; i++)
			{
				ui.tableTarGM->setItem(i, cols, new QTableWidgetItem());//添加新元素
			}
			ui.tableTarGM->selectColumn(cols);//选中新列
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
				ui.tableTarGM->removeColumn(colNum - 1);//删除列
				flag_calibrateGM = false;
			}
			else
			{
				senderTableWidget->removeColumn(tmpItem->column());//删除列
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
	//获取信号发送者
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());

	/*******************************************/
	//设置菜单选项
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *clearContents = new QAction("清空所有内容", senderTableWidget);
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
		QMessageBox::information(NULL, "提示", "尚未标定静平台靶标点在测量坐标系下位置,请标定后重试。");
		return;
	}

	if (flag_calibrateGM == false)
	{
		QMessageBox::information(NULL, "提示", "尚未标定工装靶标点在动系下坐标,请标定后重试。");
		return;
	}

	int numCol = ui.tableTarGM->columnCount();
	//判断表格中是否有空值
	for (int j = 0; j < numCol; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (ui.tableTarGM->item(i, j) == NULL || ui.tableTarGM->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "提示", "表格中存在空值。");
				return;
			}
		}
	}

	//1.目标位置靶标点在测量坐标系下的坐标
	MatrixXd Q_tarGM_temp = MatrixXd::Zero(3, numCol);
	//2.与其序号对应的工装靶标点在动平台下的坐标
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
	//3.求目标位置靶标点在动平台下的的齐次坐标
	MatrixXd Q_tarGD_temp_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_tarGM_temp);
	//目标工装靶点在动平台下的坐标
	MatrixXd Q_tarGD_temp = homogeneous2Matrix(Q_tarGD_temp_homogeneous);
	//4.求目标工装相对当前动平台的齐次变换矩阵
	MatrixXd Trans_tarGD = rigidMotionSVDSolution(Q_GD_temp, Q_tarGD_temp);
	//5.求目标工装位姿相对当静系的齐次变换矩阵
	MatrixXd Trans_tarGS = UPSData::Trans_DS*Trans_tarGD;
	//6.求目标工装在静系中的位姿()
	Matrix<double, 6, 1> PosAndAngle_tarGS = trans2PosAngle(Trans_tarGS);
	//7.求目标工装所在运动坐标系在静系中的位姿
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
	
	
	//修改动平台结构参数
	Matrix<double, 3, 6> XYZ_setD_Mat= MatrixXd::Zero(3, 6);
	for (int i=0;i<6;i++)
	{
		XYZ_setD_Mat.col(i) = UPSData::XYZ_setD;
	}
	UPSData::D_set = UPSData::D - XYZ_setD_Mat;

	//修改 运动坐标系相对动平台的齐次变换矩阵
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
	//关键点序号
	ui.keyPointTable->setItem(tabRow, 0, new QTableWidgetItem(QString::number(tabRow + 1)));
	//将当前动平台相对静平台位姿记录
	for (int i = 0; i < 6; i++)
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(UPSData::curPosAndAngle_DS(i)));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		ui.keyPointTable->setItem(tabRow, i + 1, item);
	}
}

void SixUpsUiAo::on_inputKeyPointBtn_clicked()
{
	QString tempKeyPointsFile = QFileDialog::getOpenFileName(this, "选择文件", "D:/", "文本文件(*.txt;*.csv;)");
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
		int questionResult = QMessageBox::question(NULL, "提示", "未导入关键点文件，是否新建关键点文件?", QMessageBox::Yes | QMessageBox::No);
		if (questionResult == QMessageBox::Yes)
		{
			keyPointsFile = QFileDialog::getSaveFileName(this, "选择文件", "D:/", "表格文件(*.csv);;文本文件(*.txt)");
		}
		else
		{
			return;
		}
	}
	bool flag = tableToCsv(ui.keyPointTable, keyPointsFile);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "保存关键点数据成功。");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "保存关键点数据失败！");
	}
	ui.led_keyPointsFile->setText(keyPointsFile);
}


void SixUpsUiAo::on_saveAsKeyPointBtn_clicked()
{
	QString tempKeyPointsFile = QFileDialog::getSaveFileName(this, "选择文件", "D:/", "表格文件(*.csv);;文本文件(*.txt)");
	if (tempKeyPointsFile != "")
	{
		keyPointsFile = tempKeyPointsFile;
		bool flag = tableToCsv(ui.keyPointTable, keyPointsFile);
		if (flag)
		{
			QMessageBox::information(NULL, "提示", "另存关键点数据成功。");
		}
		else
		{
			QMessageBox::information(NULL, "提示", "另存关键点数据失败！");
		}
	} 
	ui.led_keyPointsFile->setText(keyPointsFile);
}

void SixUpsUiAo::addTableWidgetMenuKeyPoint()
{
	//获取信号发送者
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());
	/*******************************************/
	//设置菜单选项
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *transPos = new QAction("将关键点转换到目标位姿", senderTableWidget);
	QAction *deleteRow = new QAction("删除当前关键点数据", senderTableWidget);
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
			senderTableWidget->removeRow(tmpItem->row());//删除列
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
	//反解求杆长
	inverseSolution(UPSData::tarPosAndAngle_setS, UPSData::tarL_norm, UPSData::D_set, UPSData::S);
	//杆长减初始杆长得到PMAC杆长
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
	int posNum = (btnName.mid(btnNamePrefix.size(), -1)).toInt();//绝对位姿 x y z a b c的顺序号
	int index = posNum - 1;//轴号减1才是索引号
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
	//运动坐标系相对静平台的目标位姿
	UPSData::tarPosAndAngle_setS = UPSData::curPosAndAngle_setS + incPosAndAngle;
	//反解
	inverseSolution(UPSData::tarPosAndAngle_setS, UPSData::tarL_norm, UPSData::D_set, UPSData::S);
	UPSData::tarAxlesL_norm = UPSData::tarL_norm - UPSData::initL_norm;

	double moveTime = 30;
	if (UPSData::multiJogMoveDirectionID<=3)
	{
		//平动
		moveTime = abs(UPSData::multiJogTranslationStep / UPSData::multiJogTranslationSpeed);
	} 
	else
	{
		//转动
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
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	/*设置运动方向*/
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
	/*设置运动步长及*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (UPSData::multiJogMoveDirectionID <= 3)
	{
		//平动
		UPSData::multiJogMoveStep = UPSData::multiJogTranslationSpeed * upsJogTimer->interval() / 1000.0; //最小运动步长
	}
	else
	{
		//转动
		UPSData::multiJogMoveStep = UPSData::multiJogRotateSpeed * upsJogTimer->interval() / 1000.0;//最小运动步长
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
	Matrix<double, 6, 1> moveDirection = MatrixXd::Zero(6, 1);//运动方向向量
	/*设置运动方向*/
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
	/*设置运动步长及*/
	UPSData::multiJogMoveDirection = moveDirection;
	if (UPSData::multiJogMoveDirectionID <= 3)
	{
		//平动
		UPSData::multiJogMoveStep = UPSData::multiJogTranslationSpeed * upsJogTimer->interval() / 1000.0;//运动步长
	}
	else
	{
		//转动
		UPSData::multiJogMoveStep = UPSData::multiJogRotateSpeed * upsJogTimer->interval() / 1000.0;//运动步长
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
	QString strFile = QFileDialog::getOpenFileName(this, "选择文件", "D:/", "文本文件(*.txt;*.csv;)");
	Vector2d csvSize = getCsvSize(strFile);
	
	if (strFile != "")
	{
		if (csvSize(0) > 3 )
		{
			QMessageBox::information(NULL, "提示", "导入数据点文件的行数应小于等于3");
		}
		else
		{
			csvToTableAdapt(strFile, ui.tableInputPoints);
		}
	}
}

void SixUpsUiAo::on_inputMatBtn_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "选择文件", "D:/", "文本文件(*.txt;*.csv;)");
	Vector2d csvSize = getCsvSize(strFile);
	if (strFile!="")
	{
		if (csvSize(0) != 4 || csvSize(1) != 4)
		{
			QMessageBox::information(NULL, "提示", "导入矩阵的维度错误，应为4×4矩阵。");
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
	//判断表格中是否有空值
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (ui.tableInputMat4d->item(i, j) == NULL || ui.tableInputMat4d->item(i, j)->text() == "")
			{
				QMessageBox::information(NULL, "提示", "表格中存在空值。");
				return;
			}
		}
	}

	bool flag = tableToMatrixXd(ui.tableInputMat4d , mat);

	if (flag)
	{
		//求目标在静系中的位姿
		Matrix<double, 6, 1> PosAndAngle_matS = trans2PosAngle(mat);

		//求目标在运动坐标系在静系中的位姿
		//Matrix<double, 6, 1> PosAndAngle_matsetS = posAndAngleDS2setS(PosAndAngle_matS, UPSData::Trans_setD);
		for (int i = 0; i < 6; i++)
		{
			AbsTarPos_group[i]->setValue(PosAndAngle_matS(i));
		}
		
		QMessageBox::information(NULL, "提示", "转换到目标位姿成功！");
		ui.tabWidget->setCurrentIndex(2);
	} 
	else
	{
		QMessageBox::information(NULL, "提示", "转换到目标位姿失败。");
	}

}

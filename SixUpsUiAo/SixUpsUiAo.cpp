#include "SixUpsUiAo.h"


SixUpsUiAo::SixUpsUiAo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	qDebug() << "SixUpsUiAo 构造";

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

	/*************力绘制************************/
	plotTimer = new QTimer(this);
	plotTimer->setInterval(100);//设置定时器的间隔50ms
	plotTimer->setTimerType(Qt::PreciseTimer);//精确定时
	connect(plotTimer, &QTimer::timeout, this, &SixUpsUiAo::on_plotTimer);
	/*力曲线窗口初始化*/
	//1号轴
	widget_Force1 = ui.widget_Force1;
	curves_Force1 = new CurvePlot(widget_Force1);
	curves_Force1->setxAxisRange(0, 210);
	curves_Force1->setyAxisRange(-2400, 2400);
	curves_Force1->setSelectLegend(false);

	widget_Force2 = ui.widget_Force2;
	curves_Force2 = new CurvePlot(widget_Force2);
	curves_Force2->setxAxisRange(0, 210);
	curves_Force2->setyAxisRange(-2400, 2400);
	curves_Force2->setSelectLegend(false);

	widget_Force3 = ui.widget_Force3;
	curves_Force3 = new CurvePlot(widget_Force3);
	curves_Force3->setxAxisRange(0, 210);
	curves_Force3->setyAxisRange(-2400, 2400);
	curves_Force3->setSelectLegend(false);

	widget_Force4 = ui.widget_Force4;
	curves_Force4 = new CurvePlot(widget_Force4);
	curves_Force4->setxAxisRange(0, 210);
	curves_Force4->setyAxisRange(-2400, 2400);
	curves_Force4->setSelectLegend(false);

	widget_Force5 = ui.widget_Force5;
	curves_Force5 = new CurvePlot(widget_Force5);
	curves_Force5->setxAxisRange(0, 210);
	curves_Force5->setyAxisRange(-2400, 2400);
	curves_Force5->setSelectLegend(false);

	widget_Force6 = ui.widget_Force6;
	curves_Force6 = new CurvePlot(widget_Force6);
	curves_Force6->setxAxisRange(0, 210);
	curves_Force6->setyAxisRange(-2400, 2400);
	curves_Force6->setSelectLegend(false);

	widget_Force7 = ui.widget_Force7;
	curves_Force7 = new CurvePlot(widget_Force7);
	curves_Force7->setxAxisRange(0, 210);
	curves_Force7->setyAxisRange(-2400, 2400);
	curves_Force7->setSelectLegend(false);
	/*注册数据类型 跨线程传递*/
	qRegisterMetaType<QList<QList<QVariant>>>("QList<QList<QVariant>>");
	qRegisterMetaType<QList<QList<QVariant>>>("QList<QList<QVariant>>*");
	qRegisterMetaType<CurveType>("CurveType");

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
	//ui.tabWidget->removeTab(3);
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
	//导入动静平台结构参数
	csvToMatrixXd("./Data/结构参数/D.csv", UPSData::D);
	csvToMatrixXd("./Data/结构参数/D_theoretical.csv", UPSData::D_theoretical);
	csvToMatrixXd("./Data/结构参数/S.csv", UPSData::S);
	csvToMatrixXd("./Data/结构参数/S_theoretical.csv", UPSData::S_theoretical);
	csvToMatrixXd("./Data/结构参数/Q_DD.csv", UPSData::Q_DD);
	csvToMatrixXd("./Data/结构参数/Q_SS.csv", UPSData::Q_SS);
	csvToMatrixXd("./Data/结构参数/initL_norm.csv", UPSData::initL_norm);
	csvToMatrixXd("./Data/平台参数/homePosAndAngle.csv", UPSData::homePosAndAngle_DS);
	UPSData::initPosAndAngle_DS = UPSData::homePosAndAngle_DS;

	//导入关键点
	inputKeyPoint("./Data/平台参数/keyPoint.csv", ui.keyPointTable);
	
}

void SixUpsUiAo::initTablesStyle()
{
	int rowHeight = 30;//行高
	int colWidth = 80;//列宽
	int scrollBarHeight = 20;//滚动条高
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableSetOrigin 原点设置******/
	//列设置
	ui.tableSetOrigin->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableSetOrigin->setColumnCount(3);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "O" << "X" << "XoY" ;
	ui.tableSetOrigin->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);

	//行设置
	ui.tableSetOrigin->setRowCount(3);
	ui.tableSetOrigin->verticalHeader()->setVisible(true);//行号可见
	ui.tableSetOrigin->setVerticalHeaderLabels(VerticalHeaderXYZ);
	ui.tableSetOrigin->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableSetOrigin->verticalHeader()->setFixedWidth(25);
	ui.tableSetOrigin->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableSetOrigin->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableSetOrigin->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableSetOrigin->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableSetOrigin->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableSetOrigin->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableSetOrigin->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableSetOrigin->setFixedHeight(4 * rowHeight);

	/*****keyPointTable ******/
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
	tableSMVerticalHeader <<"静铰点序号"<< "X坐标" << "Y坐标" << "Z坐标";
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
	tableGMHorizontalHeader << "靶点1" << "靶点2" << "靶点3";
	//列设置
	ui.tableGM->horizontalHeader()->setDefaultSectionSize(colWidth);
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
	tableTarGMVerticalHeader << "工装靶点序号" << "X坐标" << "Y坐标" << "Z坐标";
	//列设置
	ui.tableTarGM->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableTarGM->setColumnCount(3);
	ui.tableTarGM->horizontalHeader()->setVisible(false);//列号不可见
	ui.tableTarGM->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
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

	/*tableInput*/
	QStringList tableInputVerticalHeader;
	tableInputVerticalHeader << "X坐标" << "Y坐标" << "Z坐标";
	//列设置
	ui.tableInput->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableInput->setColumnCount(4);
	ui.tableInput->horizontalHeader()->setVisible(true);//列号不可见
	ui.tableInput->horizontalHeader()->setStretchLastSection(false);//行头自适应表格
	//行设置
	ui.tableInput->setRowCount(tableInputVerticalHeader.size());
	ui.tableInput->verticalHeader()->setVisible(true);//行号可见
	ui.tableInput->setVerticalHeaderLabels(tableInputVerticalHeader);
	ui.tableInput->verticalHeader()->setFont(QFont("黑体", 10));
	ui.tableInput->verticalHeader()->setFixedWidth(80);
	ui.tableInput->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.tableInput->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableInput->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableInput->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//大小设置
	ui.tableInput->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableInput->setFixedHeight(4 * rowHeight + scrollBarHeight);
	ui.tableInput->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void SixUpsUiAo::initConnect()
{
	connect(this, &SixUpsUiAo::upsHome_signal, this, &SixUpsUiAo::upsHome_slot);//平台回零
	for (int i = 0; i < 6; i++)
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
	connect(ui.tableTarGM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenuAddCoils()));
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
	/*让相关状态显示UI初始化*/
	for (int i = 0; i < 6; i++)
	{
		qlabNegLimit_group[i]->setPixmap(offIcon);
		qlabPosLimit_group[i]->setPixmap(offIcon);
		qlabOrigin_group[i]->setPixmap(offIcon);
		realTimeLengths_group[i]->setText("");
		realTimeForce_group[i]->setText("");
		realTimePose_DS_group[i]->setText("");
		realTimePose_setS_group[i]->setText("");
		realTimePose_Dset_group[i]->setText("");
	}

	ui.pmacSta->setPixmap(offIcon);
	ui.pmacInitSta->setPixmap(offIcon);
	ui.connectPmacBtn->setText("连接");
	ui.initPmacBtn->setEnabled(false);
	ui.servoOnBtn->setEnabled(false);
	ui.servoOffBtn->setEnabled(false);
	ui.axlesHomeBtn->setEnabled(false);
	ui.upsHomeBtn->setEnabled(false);
	//ui.tabWidget->setEnabled(false);
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
	
	matrixXdToCsv(PmacData::curLengthsMM, "./Data/平台参数/lastCurLengthsMM.csv");
}

bool SixUpsUiAo::loadLastLengthsMM()
{
	Matrix<double, 6, 1> lastCurLengthsMM = MatrixXd::Zero(6, 1);
	bool flag = csvToMatrixXd("./Data/平台参数/lastCurLengthsMM.csv", lastCurLengthsMM);
	if (flag)
	{
		// pmac初始时杆长设置
		myPmac->setCurLengths(lastCurLengthsMM);
		DeleteFileOrFolder("./Data/平台参数/lastCurLengthsMM.csv");
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

void SixUpsUiAo::addPoint(CurvePlot * graph, const double & x_temp, const double & y_temp)
{
	graph->x0.append(x_temp);
	graph->y0.append(y_temp);
	QList<QVariant> xy;
	xy.append(x_temp);
	xy.append(y_temp);
	graph->x_y0.append(xy);
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

	QString strLength;
	/*冗余支链相关显示*/
	strLength = QString::number(PmacData::redundantChain(0), 'f', 3);
	ui.led_realTimeLength7->setText(strLength);
	strLength = QString::number(PmacData::redundantChain(1), 'f', 3);
	ui.led_realTimeForce7->setText(strLength);
	for (int i = 0; i < 6; i++)
	{
		/**********杆长***************/
		strLength = QString::number(PmacData::curLengthsMM(i), 'f', 3);
		realTimeLengths_group[i]->setText(strLength);
		/**********支链力*************/
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
		strPos = QString::number(UPSData::curPosAndAngle_Dset(i), 'f', 3);
		realTimePose_Dset_group[i]->setText(strPos);
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
	Matrix<double, 6, 1> tarL_norm;//目标杆长
	//动平台目标位姿 相对 运动坐标系的位姿向量
	UPSData::prsPosAndAngle_Dset = UPSData::prsPosAndAngle_Dset + UPSData::multiJogMoveStep * UPSData::multiJogMoveDirection;
	//动平台目标位姿 相对 静坐标系的位姿向量
	Matrix<double, 6, 1> tarPosAndAngle_DS = posAndAngleDset2DS(UPSData::prsPosAndAngle_Dset, UPSData::Trans_setS);
	//运动学反解反解
	inverseSolution(tarPosAndAngle_DS, tarL_norm, UPSData::D, UPSData::S);
	//求PMAC杆长
	UPSData::tarAxlesL_norm = tarL_norm - UPSData::initL_norm;
	//计算这一次各杆长度与上一次各杆长度的差值
	delta_Lengths = UPSData::tarAxlesL_norm - UPSData::lastAxlesL_norm;
	//用各杆长差值计算各杆的运动速度 保证每根杆同一时间到达目标位置
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
	//myPmac->enablePLC(1);
	myPmac->upsJogJMove(UPSData::tarAxlesL_norm, axlesSpeed);
	UPSData::lastAxlesL_norm = UPSData::tarAxlesL_norm;//更新

}

void SixUpsUiAo::on_plotTimer()
{
	//绘图时间
	endTime = QDateTime::currentDateTime();
	intervalTime = startTime.msecsTo(endTime) / 1000.0;

	//添加点
	//addPoint(curves_Force1, intervalTime, PmacData::curForce(0));
	recordData[0].append(intervalTime);
	for (int i=0;i<6;i++)
	{
		recordData[i+1].append(PmacData::curForce(i));
	}
	recordData[7].append(PmacData::redundantChain(1));
	for (int i = 0; i < 6; i++)
	{
		recordData[i + 8].append(UPSData::curL_norm(i));
	}
	recordData[14].append(PmacData::redundantChain(0));
	//设置数据
	widget_Force1->graph(0)->setData(recordData[0], recordData[1]);
	widget_Force2->graph(0)->setData(recordData[0], recordData[2]);
	widget_Force3->graph(0)->setData(recordData[0], recordData[3]);
	widget_Force4->graph(0)->setData(recordData[0], recordData[4]);
	widget_Force5->graph(0)->setData(recordData[0], recordData[5]);
	widget_Force6->graph(0)->setData(recordData[0], recordData[6]);
	widget_Force7->graph(0)->setData(recordData[0], recordData[7]);
	//重新绘制
	widget_Force1->replot(QCustomPlot::rpQueuedReplot);
	widget_Force2->replot(QCustomPlot::rpQueuedReplot);
	widget_Force3->replot(QCustomPlot::rpQueuedReplot);
	widget_Force4->replot(QCustomPlot::rpQueuedReplot);
	widget_Force5->replot(QCustomPlot::rpQueuedReplot);
	widget_Force6->replot(QCustomPlot::rpQueuedReplot);
	widget_Force7->replot(QCustomPlot::rpQueuedReplot);

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

void SixUpsUiAo::on_calibrateGMBtn_clicked()
{
	//1.求工装靶标点在动坐标系下的坐标
	vector<int> index;
	MatrixXd Q_GM;
	tableToMatrixXd(ui.tableGM, Q_GM, index);
	MatrixXd  Q_GD_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_GM);
	UPSData::Q_GD = homogeneous2Matrix(Q_GD_homogeneous);
	cout << UPSData::Q_GD;
}

void SixUpsUiAo::addTableWidgetMenuAddCoils()
{
	//获取信号发送者
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());
	//qDebug() << senderTableWidget->objectName();

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
		colTitle = "靶点";
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
	QAction *deleteColumn = new QAction("删除当前列", senderTableWidget);
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
			senderTableWidget->removeColumn(tmpItem->column());//删除列
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

void SixUpsUiAo::on_transTarPoseBtn_clicked()
{
	int numCol = ui.tableTarGM->columnCount();
	//1.目标位置工装靶标点在测量坐标系下的坐标
	MatrixXd Q_tarGM_temp = MatrixXd::Zero(3, numCol);
	//2.与其序号对应的工装靶标点在动坐标系下的坐标
	MatrixXd Q_GD_temp = MatrixXd::Zero(3, numCol);
	for (int j = 0; j < numCol; j++)
	{
		int p = ui.tableTarGM->item(0, j)->text().toInt();
		Q_GD_temp.col(j) = UPSData::Q_GD.col(p - 1);
		for (int i = 0; i < 3; i++)
		{
			if (!ui.tableTarGM->item(i + 1, j)->text().isEmpty())
			{
				Q_tarGM_temp(i, j) = ui.tableTarGM->item(i + 1, j)->text().toDouble();
			}
		}
	}
	//3.求目标位置工装靶标点相对当前动系的齐次变换矩阵
	MatrixXd Q_tarGD_temp_homogeneous = UPSData::Trans_DS.inverse()*UPSData::Trans_SM.inverse()*matrix2Homogeneous(Q_tarGM_temp);
	//目标工装靶点在当前动系下的坐标
	MatrixXd Q_tarGD_temp = homogeneous2Matrix(Q_tarGD_temp_homogeneous);
	//4.求目标工装位姿相对当前动系的齐次变换矩阵
	MatrixXd Trans_tarGD = rigidMotionSVDSolution(Q_GD_temp, Q_tarGD_temp);
	//5.求目标工装位姿相对当静系的齐次变换矩阵
	MatrixXd Trans_tarGS = UPSData::Trans_DS*Trans_tarGD;
	//6.求目标工装在静系中的位姿
	Matrix<double, 6, 1> PosAndAngle_tarGS = trans2PosAngle(Trans_tarGS);
	//7.求目标工装在运动坐标系中的位姿
	Matrix<double, 6, 1> PosAndAngle_tarGset = MatrixXd::Zero(6, 1);
	PosAndAngle_tarGset = posAndAngleDS2Dset(PosAndAngle_tarGS, UPSData::Trans_setS);
	for (int i = 0; i < 6; i++)
	{
		AbsTarPos_group[i]->setValue(PosAndAngle_tarGset(i));
	}
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
	//运动坐标系在测量坐标系中的齐次坐标
	Matrix4d Trans_set_M = creatCoordSysGetRt(UPSData::O_set_M, UPSData::X_set_M, UPSData::XOY_set_M);
	//将运动坐标系转换到静坐标系下描述
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
	//关键点序号
	ui.keyPointTable->setItem(tabRow , 0, new QTableWidgetItem(QString::number(tabRow + 1)));
	//将当前动平台相对静平台位姿记录
	for (int i = 0; i < 6; i++ )
	{
		QTableWidgetItem *item = new QTableWidgetItem(QString::number(UPSData::curPosAndAngle_DS(i)));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		ui.keyPointTable->setItem(tabRow , i + 1, item);
	}
	
}

void SixUpsUiAo::on_inputKeyPointBtn_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "选择文件", "./", "文本文件(*.txt;*.csv;)");
	csvToTable(strFile, ui.keyPointTable);
}

void SixUpsUiAo::on_saveKeyPointBtn_clicked()
{
	bool flag = tableToCsv(ui.keyPointTable, "./Data/平台参数/keyPoint.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "保存关键点数据成功。");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "保存关键点数据失败！");
	}
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
		UPSData::multiSpeed = 2;
		break;
	case 3:
		UPSData::multiSpeed = 3;
		break;
	default:
		break;
	}
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
	//反解求杆长
	inverseSolution(tarPosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
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
	qDebug() << "Step:" ;
	cout << incPosAndAngle << endl;
	//动平台目标位姿相对运动坐标系的位姿向量
	UPSData::tarPosAndAngle_Dset = UPSData::curPosAndAngle_Dset + incPosAndAngle;
	qDebug() << "tarPosAndAngle_Dset:";
	cout << UPSData::tarPosAndAngle_Dset << endl;
	//动平台目标位相对静坐标系的位姿向量
	Matrix<double, 6, 1> tarPosAndAngle_DS = posAndAngleDset2DS(UPSData::tarPosAndAngle_Dset, UPSData::Trans_setS);
	qDebug() << "tarPosAndAngle_DS:";
	cout << tarPosAndAngle_DS << endl;
	//反解
	inverseSolution(tarPosAndAngle_DS, UPSData::tarL_norm, UPSData::D, UPSData::S);
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
	//myPmac->upsAbsMove(UPSData::tarAxlesL_norm, UPSData::multiJogTranslationSpeed);
	myPmac->upsAbsMove(UPSData::tarAxlesL_norm, vel);
	
}

void SixUpsUiAo::on_disMultiAxisJog_stop_clicked()
{
	myPmac->jogStop();
}

void SixUpsUiAo::on_prsMultiAxisJogNeg_pressed()
{

	UPSData::prsPosAndAngle_Dset = UPSData::curPosAndAngle_Dset;
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

	UPSData::prsPosAndAngle_Dset = UPSData::curPosAndAngle_Dset;
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

void SixUpsUiAo::on_inputBtn_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "选择文件", "./", "文本文件(*.txt;*.csv;)");
	qDebug() << strFile;
	csvToTableAdapt(strFile, ui.tableInput);
}

void SixUpsUiAo::on_plotForceBtn_toggled(bool checked)
{
	/*开始绘图 仅仅绘图不运动*/
	if (checked)
	{
		qDebug() << "on_plotRealTimeCurve  is checked";
		if (!plotTimer->isActive())
		{
			recordData.clear();
			QVector<double> vectorTemp;
			for (int i = 0; i < 15; i++)
			{
				recordData.append(vectorTemp);
			}
			plotTimer->start();
			startTime = QDateTime::currentDateTime();//获取开始时间
			qDebug() << "plotTimer->start()";
		}
		else
		{
			QMessageBox::information(NULL, "提示", "当前正在绘图！");
		}
	}
	else
	{
		qDebug() << "on_plotRealTimeCurve  is  not checked";
		if (!plotTimer->isActive())
		{
			QMessageBox::information(NULL, "提示", "当前未在绘图！");
		}
		else
		{
			qDebug() << "plotTimer->stop()";
			plotTimer->stop();
		}
	}
}

void SixUpsUiAo::on_clearForceBtn_clicked()
{
	recordData.clear();
	curves_Force1->clearAllCurves();
	curves_Force2->clearAllCurves();
	curves_Force3->clearAllCurves();
	curves_Force4->clearAllCurves();
	curves_Force5->clearAllCurves();
	curves_Force6->clearAllCurves();
	curves_Force7->clearAllCurves();
}

void SixUpsUiAo::on_recordForceBtn_clicked()
{
	QString strFile = QFileDialog::getSaveFileName(this, "另存为", "./", "文本文件(*.csv;)");
	if (!strFile.isEmpty())
	{
		qlistQvectorToCsv(recordData, strFile);
	}

	
}

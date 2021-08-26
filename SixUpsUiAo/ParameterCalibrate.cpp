#include "ParameterCalibrate.h"


ParameterCalibrate::ParameterCalibrate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//关闭时 可以析构
	qDebug() << "ParameterCalibrate 构造";

	initTablesStyle(); 
	initTableWidgetConnect();//添加右键初始化
}

ParameterCalibrate::~ParameterCalibrate()
{
	qDebug() << "ParameterCalibrate 析构";
}

void ParameterCalibrate::initTablesStyle()
{
	int rowHeight = 30;//行高
	int colWidth = 80;//列宽
	int scrollBarHeight = 25;//滚动条高
	/*****tableCirclePt 待拟合参数******/
	setTablesStyle(ui.tableCirclePt);
	//列设置
	ui.tableCirclePt->setColumnCount(4);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "点1" << "点2" << "点3"<< "点4";
	ui.tableCirclePt->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);
	/**********tableCircleC  拟合后数据*****************/
	setTablesStyle(ui.tableCircleC);
	//列设置
	ui.tableCircleC->setColumnCount(1);
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "圆心" ;
	ui.tableCircleC->setHorizontalHeaderLabels(tableCircleCHorizontalHeader);

	QStringList tableD_HorizontalHeader;
	tableD_HorizontalHeader << "动铰点1" << "动铰点2" << "动铰点3" << "动铰点4" << "动铰点5" << "动铰点6";
	/*****tableD_M ******/
	setTablesStyle(ui.tableD_M);
	//列设置
	ui.tableD_M->setColumnCount(6);
	ui.tableD_M->setHorizontalHeaderLabels(tableD_HorizontalHeader);
	/*****tableD ******/
	setTablesStyle(ui.tableD);
	//列设置
	ui.tableD->setColumnCount(6);
	ui.tableD->setHorizontalHeaderLabels(tableD_HorizontalHeader);

	QStringList tableS_HorizontalHeader;
	tableS_HorizontalHeader << "静铰点1" << "静铰点2" << "静铰点3" << "静铰点4" << "静铰点5" << "静铰点6";
	/*****tableS_M ******/
	setTablesStyle(ui.tableS_M);
	//列设置
	ui.tableS_M->setColumnCount(6);
	ui.tableS_M->setHorizontalHeaderLabels(tableS_HorizontalHeader);
	/*****tableS ******/
	setTablesStyle(ui.tableS);
	//列设置
	ui.tableS->setColumnCount(6);
	ui.tableS->setHorizontalHeaderLabels(tableS_HorizontalHeader);

	QStringList tableQ_D_StructHorizontalHeader;
	tableQ_D_StructHorizontalHeader << "动靶标1" << "动靶标2" << "动靶标3" << "动靶标4" << "动靶标5" << "动靶标6";
	/*****tableQ_DM ******/
	setTablesStyle(ui.tableQ_DM);
	//列设置
	ui.tableQ_DM->setColumnCount(6);
	ui.tableQ_DM->setHorizontalHeaderLabels(tableQ_D_StructHorizontalHeader);
	/*****tableQ_DD ******/
	setTablesStyle(ui.tableQ_DD);
	//列设置
	ui.tableQ_DD->setColumnCount(6);
	ui.tableQ_DD->setHorizontalHeaderLabels(tableQ_D_StructHorizontalHeader);

	QStringList tableQ_S_HorizontalHeader;
	tableQ_S_HorizontalHeader << "静靶标1" << "静靶标2" << "静靶标3" << "静靶标4" << "静靶标5" << "静靶标6";
	/*****tableQ_SM ******/
	setTablesStyle(ui.tableQ_SM);
	//列设置
	ui.tableQ_SM->setColumnCount(6);
	ui.tableQ_SM->setHorizontalHeaderLabels(tableQ_S_HorizontalHeader);
	/*****tableQ_SS ******/
	setTablesStyle(ui.tableQ_SS);
	//列设置
	ui.tableQ_SS->setColumnCount(6);
	ui.tableQ_SS->setHorizontalHeaderLabels(tableQ_S_HorizontalHeader);
}

void ParameterCalibrate::setTablesStyle(QTableWidget * table)
{
	int rowHeight = 30;//行高
	int colWidth = 100;//列宽
	int scrollBarHeight = 20;//滚动条高
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	//列设置
	table->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	table->setRowCount(3);
	table->verticalHeader()->setVisible(true);//行号可见
	table->setVerticalHeaderLabels(VerticalHeaderXYZ);
	table->verticalHeader()->setFont(QFont("黑体", 10));
	table->verticalHeader()->setFixedWidth(30);
	table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	table->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	table->horizontalHeader()->setFont(QFont("黑体", 10));//表头字体
	table->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	table->setAlternatingRowColors(true);//行奇偶颜色不同
	table->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	table->setFixedHeight(4 * rowHeight + scrollBarHeight);
}


void ParameterCalibrate::initTableWidgetConnect()
{
	ui.tableCirclePt->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableCirclePt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
}


void ParameterCalibrate::on_calCircleC_clicked()
{
	qDebug() << "on_calCircleC_clicked";
	vector<int> dataIndex;//有数据的列索引
	tableToMatrixXd(ui.tableCirclePt, UPSData::circlePt, dataIndex);//从表中写入
	if (UPSData::circlePt.cols() != 0)
	{
		solveOriginalPtByCircle(UPSData::circlePt, UPSData::circleC, UPSData::r);//拟合圆心
		matrixXdToTable(UPSData::circleC, ui.tableCircleC);
	}
	else
	{
		qDebug() << "on_calCircleC_clicked 无数据";
	}
}
void ParameterCalibrate::on_calCircleS_clicked()
{
	qDebug() << "on_calCircleS_clicked";
	vector<int> dataIndex;//有数据的列索引
	tableToMatrixXd(ui.tableCirclePt, UPSData::circlePt, dataIndex);//从表中写入
	if (UPSData::circlePt.cols() != 0)
	{
		solveOriginalPtBySphere(UPSData::circlePt, UPSData::circleC, UPSData::r);//拟合球心
		matrixXdToTable(UPSData::circleC, ui.tableCircleC);
	}
	else
	{
		qDebug() << "on_calCircleS_clicked 无数据";
	}
}

void ParameterCalibrate::on_dHingeCalbrateBtn_clicked()
{
	//计算动平台坐标系原点在测量坐标系下的位姿
	tableToMatrixXd(ui.tableD_M, UPSData::D_M);
	calibrateHingePoint(UPSData::D_theoretical, UPSData::D_M, UPSData::D, UPSData::O_DM_r, UPSData::O_DM_t);
	bool flag = matrixXdToTable(UPSData::D, ui.tableD);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "动平台铰链点标定成功");
		ui.dTargetCalbrateBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "提示", "动平台铰链点标定失败！");
	}
	
}

void ParameterCalibrate::on_sHingeCalbrateBtn_clicked()
{
	//计算静平台坐标系原点在测量坐标系下的位姿
	tableToMatrixXd(ui.tableS_M, UPSData::S_M);
	calibrateHingePoint(UPSData::S_theoretical, UPSData::S_M, UPSData::S, UPSData::O_SM_r, UPSData::O_SM_t);
	bool flag = matrixXdToTable(UPSData::S, ui.tableS);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "静平台铰链点标定成功");
		ui.sTargetCalbrateBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "提示", "静平台铰链点标定失败！");
	}
}

void ParameterCalibrate::on_dTargetCalbrateBtn_clicked()
{
	vector<int> index;
	tableToMatrixXd(ui.tableQ_DM, UPSData::Q_DM, index);
	calibrateTargetPoint(UPSData::Q_DM, UPSData::O_DM_r, UPSData::O_DM_t, UPSData::Q_DD);
	bool flag = matrixXdToTable(UPSData::Q_DD, ui.tableQ_DD);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "动平台靶标点标定成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "动平台靶标点标定失败！");
	}

}

void ParameterCalibrate::on_sTargetCalbrateBtn_clicked()
{
	vector<int> index;
	tableToMatrixXd(ui.tableQ_SM, UPSData::Q_SM, index);
	calibrateTargetPoint(UPSData::Q_SM, UPSData::O_SM_r, UPSData::O_SM_t, UPSData::Q_SS);
	bool flag = matrixXdToTable(UPSData::Q_SS, ui.tableQ_SS);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "静平台靶标点标定成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "静平台靶标点标定失败！");
	}
}

void ParameterCalibrate::on_inputTestDataBtn_clicked()
{
	csvToTable("./Data/S_M_test.csv", ui.tableS_M);
	csvToTable("./Data/D_M_test.csv", ui.tableD_M);
	csvToTable("./Data/Q_SM_test.csv", ui.tableQ_SM);
	csvToTable("./Data/Q_DM_test.csv", ui.tableQ_DM);
}

void ParameterCalibrate::on_saveMeasureDataBtn_clicked()
{
	bool flag1 = tableToCsv(ui.tableS_M, "./Data/S_M.csv");
	bool flag2 = tableToCsv(ui.tableD_M, "./Data/D_M.csv");
	bool flag3 = tableToCsv(ui.tableQ_SM, "./Data/Q_SM.csv");
	bool flag4 = tableToCsv(ui.tableQ_DM, "./Data/Q_DM.csv");
	if (flag1 && flag2 && flag3 && flag4)
	{ 
		QMessageBox::information(NULL, "提示", "测量数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "测量数据保存失败");
	}
	
}

void ParameterCalibrate::on_saveCalibrateDataBtn_clicked()
{
	bool flag1 = matrixXdToCsv(UPSData::D,"./Data/D.csv");
	bool flag2 = matrixXdToCsv(UPSData::S,"./Data/S.csv");
	bool flag3 = matrixXdToCsv(UPSData::Q_DD, "./Data/Q_DD.csv");
	bool flag4 = matrixXdToCsv(UPSData::Q_SS, "./Data/Q_SS.csv");
	if (flag1 && flag2 && flag3 && flag4)
	{
		QMessageBox::information(NULL, "提示", "标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "标定数据保存失败");
	}

}

void ParameterCalibrate::addTableWidgetMenu()
{
	//获取信号发送者
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());
	//qDebug() << senderTableWidget->objectName();

	/************根据表格名设定不同列标题**************/
	QString curTabName = senderTableWidget->objectName();//获取发送信号的表格名
	QStringList tabNameList;//各种表格名
	tabNameList << "tableCirclePt" << "tableMPt_D" << "tableMPt_M" << "tableTarMPt_M" << "tableMPt_M_Comp";
	QString colTitle;//列标题
	switch (tabNameList.indexOf(curTabName))
	{
	case 0:
		colTitle = "测点";
		break;
	case 1:
		colTitle = "待测点";
		break;
	case 2:
		colTitle = "待测点";
		break;
	case 3:
		colTitle = "目标待测点";
		break;
	case 4:
		colTitle = "待测点";
		break;
	default:
		break;
	}
	/*******************************************/
	//设置菜单选项
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *addColumn = new QAction("添加列", senderTableWidget);
	QAction *deleteColumn = new QAction("删除当前列", senderTableWidget);
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
	tableWidgetMenu->addAction(addColumn);
	tableWidgetMenu->addAction(deleteColumn);
	tableWidgetMenu->move(cursor().pos());
	tableWidgetMenu->show();
}

void ParameterCalibrate::on_loadStructPara_clicked()
{
	qDebug() << "on_loadStructPara_clicked";
	csvToTable("./Data/D.csv", ui.tableD);
	tableToMatrixXd(ui.tableD, UPSData::D);

	csvToTable("./Data/S.csv", ui.tableS);
	tableToMatrixXd(ui.tableS, UPSData::S);

	csvToTable("./Data/Q_DD.csv", ui.tableQ_DD);
	vector<int>Q_DDindex;
	tableToMatrixXd(ui.tableQ_DD, UPSData::Q_DD, Q_DDindex);

	csvToTable("./Data/Q_SS.csv", ui.tableQ_SS);
	vector<int>Q_SSindex;
	tableToMatrixXd(ui.tableQ_SS, UPSData::Q_SS, Q_SSindex);
	
}

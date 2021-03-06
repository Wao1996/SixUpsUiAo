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

	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableCirclePt 待拟合参数******/
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "点1" << "点2" << "点3" << "点4";
	setTablesStyle(ui.tableCirclePt, VerticalHeaderXYZ, tableCirclePtHorizontalHeader);
	/**********tableCircleC  拟合后数据*****************/
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "圆心";
	setTablesStyle(ui.tableCircleC, VerticalHeaderXYZ, tableCircleCHorizontalHeader);
	/*****tableD_M ******/
	QStringList tableD_HorizontalHeader;
	tableD_HorizontalHeader << "动铰点1" << "动铰点2" << "动铰点3" << "动铰点4" << "动铰点5" << "动铰点6";
	setTablesStyle(ui.tableD_M, VerticalHeaderXYZ, tableD_HorizontalHeader);
	/*****tableD ******/
	QStringList tableS_HorizontalHeader;
	tableS_HorizontalHeader << "动铰点1" << "动铰点2" << "动铰点3" << "动铰点4" << "动铰点5" << "动铰点6";
	setTablesStyle(ui.tableD, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableS_M ******/
	setTablesStyle(ui.tableS_M, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableS ******/
	setTablesStyle(ui.tableS, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableQ_DM ******/
	QStringList tableQ_DHorizontalHeader;
	tableQ_DHorizontalHeader << "动靶点1" << "动靶点2" << "动靶点3" << "动靶点4" << "动靶点5" << "动靶点6";
	setTablesStyle(ui.tableQ_DM, VerticalHeaderXYZ, tableQ_DHorizontalHeader);
	/*****tableQ_DD ******/
	QStringList tableQ_S_HorizontalHeader;
	tableQ_S_HorizontalHeader << "静靶点1" << "静靶点2" << "静靶点3" << "静靶点4" << "静靶点5" << "静靶点6";
	setTablesStyle(ui.tableQ_DD, VerticalHeaderXYZ, tableQ_DHorizontalHeader);
	/*****tableQ_SM ******/
	setTablesStyle(ui.tableQ_SM, VerticalHeaderXYZ, tableQ_S_HorizontalHeader);
	/*****tableQ_SS ******/
	setTablesStyle(ui.tableQ_SS, VerticalHeaderXYZ, tableQ_S_HorizontalHeader);
	/*tableQ_SM_initL*/
	setTablesStyle(ui.tableQ_SM_initL, VerticalHeaderXYZ, tableQ_S_HorizontalHeader);
	/*tableQ_DM_initL*/
	setTablesStyle(ui.tableQ_DM_initL, VerticalHeaderXYZ, tableQ_DHorizontalHeader);
	/*tableInitL*/
	QStringList tableInitL_HorizontalHeader;
	tableInitL_HorizontalHeader << "初始杆长mm";
	QStringList tableInitL_VerticalHeader;
	tableInitL_VerticalHeader << "杆1" << "杆2" << "杆3" << "杆4" << "杆5" << "杆6";
	setTablesStyle(ui.tableInitL, tableInitL_VerticalHeader, tableInitL_HorizontalHeader,45);


}

void ParameterCalibrate::setTablesStyle(QTableWidget * table, QStringList VerticalHeader, QStringList HorizontalHeader,int verticalHeaderWidth)
{
	int rowHeight = 30;//行高
	int colWidth = 100;//列宽
	int scrollBarHeight = 20;//滚动条高
	int rowNum = VerticalHeader.size();
	int colNum = HorizontalHeader.size();
	//列设置
	table->setColumnCount(colNum);
	table->horizontalHeader()->setDefaultSectionSize(colWidth);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	table->setHorizontalHeaderLabels(HorizontalHeader);
	//行设置
	table->setRowCount(rowNum);
	table->verticalHeader()->setVisible(true);//行号可见
	table->setVerticalHeaderLabels(VerticalHeader);
	table->verticalHeader()->setFont(QFont("黑体", 10));
	table->verticalHeader()->setFixedWidth(verticalHeaderWidth);
	table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	table->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	table->horizontalHeader()->setFont(QFont("黑体", 10));//表头字体
	table->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	table->setAlternatingRowColors(true);//行奇偶颜色不同
	table->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	table->setFixedHeight((rowNum+1) * rowHeight + scrollBarHeight);
}


void ParameterCalibrate::initTableWidgetConnect()
{
	ui.tableCirclePt->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableCirclePt, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
	ui.tableQ_SM->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableQ_SM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
	ui.tableQ_DM->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableQ_DM, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
	ui.tableQ_SM_initL->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableQ_SM_initL, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
	ui.tableQ_DM_initL->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableQ_DM_initL, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(addTableWidgetMenu()));
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

void ParameterCalibrate::on_inputTestDataBtn_clicked()
{
	//csvToTable("./Data/结构参数/S_M_test1.csv", ui.tableS_M,"静铰点");
	//csvToTable("./Data/结构参数/D_M_test1.csv", ui.tableD_M, "动铰点");
	//csvToTable("./Data/结构参数/Q_SM_test1.csv", ui.tableQ_SM, "静靶点");
	//csvToTable("./Data/结构参数/Q_DM_test1.csv", ui.tableQ_DM, "动靶点");
	//csvToTable("./Data/结构参数/Q_SM_test2.csv", ui.tableQ_SM_initL, "静靶点");
	//csvToTable("./Data/结构参数/Q_DM_test2.csv", ui.tableQ_DM_initL, "动靶点");
}

void ParameterCalibrate::on_inputQ_DMBtn_clicked()
{
	
	csvToTable("./Data/结构参数/Q_DM.csv", ui.tableQ_DM_initL, "动靶点");

}

void ParameterCalibrate::on_inputQ_SMBtn_clicked()
{
	csvToTable("./Data/结构参数/Q_SM.csv", ui.tableQ_SM_initL, "静靶点");
}

void ParameterCalibrate::on_dHingeCalbrateBtn_clicked()
{

	tableToMatrixXd(ui.tableD_M, UPSData::D_M);
	Trans_DM_calibrate = calibrateHingePoint(UPSData::D_theoretical, UPSData::D_M, UPSData::D );
	
	bool flag = matrixXdToTable(UPSData::D, ui.tableD,"动铰点");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "动平台铰链点标定成功，请保存数据。");
		ui.dTargetCalbrateBtn->setEnabled(true);
		ui.saveDBtn->setEnabled(true);
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
	Trans_SM_calibrate = calibrateHingePoint(UPSData::S_theoretical, UPSData::S_M, UPSData::S);
	cout << Trans_SM_calibrate;
	bool flag = matrixXdToTable(UPSData::S, ui.tableS, "静铰点");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "静平台铰链点标定成功，请保存数据。");
		ui.sTargetCalbrateBtn->setEnabled(true);
		ui.saveSBtn->setEnabled(true);
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
	calibrateTargetPoint(UPSData::Q_DM, Trans_DM_calibrate, UPSData::Q_DD);
	bool flag = matrixXdToTable(UPSData::Q_DD, ui.tableQ_DD, "动靶点");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "动平台靶标点标定成功，请保存数据。");
		ui.saveQ_DDBtn->setEnabled(true);
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
	calibrateTargetPoint(UPSData::Q_SM, Trans_SM_calibrate, UPSData::Q_SS);
	bool flag = matrixXdToTable(UPSData::Q_SS, ui.tableQ_SS, "静靶点");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "静平台靶标点标定成功，请保存数据。");
		ui.saveQ_SSBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "提示", "静平台靶标点标定失败！");
	}
}



void ParameterCalibrate::on_saveD_MBtn_clicked()
{
	bool flag = tableToCsv(ui.tableD_M, "./Data/结构参数/D_M.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "测量数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "测量数据保存失败");
	}
}

void ParameterCalibrate::on_saveS_MBtn_clicked()
{
	bool flag = tableToCsv(ui.tableS_M, "./Data/结构参数/S_M.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "测量数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "测量数据保存失败");
	}
}

void ParameterCalibrate::on_saveQ_DMBtn_clicked()
{
	bool flag = tableToCsv(ui.tableQ_DM, "./Data/结构参数/Q_DM.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "测量数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "测量数据保存失败");
	}
}

void ParameterCalibrate::on_saveQ_SMBtn_clicked()
{
	bool flag = tableToCsv(ui.tableQ_SM, "./Data/结构参数/Q_SM.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "测量数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "测量数据保存失败");
	}
}

void ParameterCalibrate::on_saveDBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::D, "./Data/结构参数/D.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "标定数据保存失败");
	}
}

void ParameterCalibrate::on_saveSBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::S, "./Data/结构参数/S.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "标定数据保存失败");
	}
}

void ParameterCalibrate::on_saveQ_DDBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::Q_DD, "./Data/结构参数/Q_DD.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "标定数据保存失败");
	}
}

void ParameterCalibrate::on_saveQ_SSBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::Q_SS, "./Data/结构参数/Q_SS.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "标定数据保存失败");
	}
}

void ParameterCalibrate::on_ininLcalbrateBtn_clicked()
{
	/*将测量系中静平台靶标点数据转换为矩阵*/
	MatrixXd Q_SM_initL;
	vector<int> index_Q_SM_initL;
	tableToMatrixXd(ui.tableQ_SM_initL, Q_SM_initL, index_Q_SM_initL);
	/*将测量系中静平台靶标点数据转换为矩阵*/
	MatrixXd Q_DM_initL;
	vector<int> index_Q_DM_initL;
	tableToMatrixXd(ui.tableQ_DM_initL, Q_DM_initL, index_Q_DM_initL);
	/*计算静平台相对测量坐标系位姿*/
	Matrix4d Trans_SM = rigidMotionSVDSolution(UPSData::Q_SS, Q_SM_initL);
	/*标定初始杆长*/
	calibrateInitLength(UPSData::initL_norm, UPSData::Q_DD, Q_DM_initL, Trans_SM, UPSData::D, UPSData::S);
	bool flag = matrixXdToTable(UPSData::initL_norm, ui.tableInitL);
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "初始杆长标定成功，请保存数据。");
		ui.saveInitLBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "提示", "初始杆长标定失败！");
	}
}


void ParameterCalibrate::on_saveInitLBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::initL_norm, "./Data/结构参数/initL_norm.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "提示", "初始杆长标定数据保存成功");
	}
	else
	{
		QMessageBox::information(NULL, "提示", "初始杆长标定数据保存失败");
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
	tabNameList << "tableCirclePt" << "tableQ_SM" << "tableQ_DM" << "tableQ_SM_initL" << "tableQ_DM_initL";
	QString colTitle;//列标题
	switch (tabNameList.indexOf(curTabName))
	{
	case 0:
		colTitle = "测点";
		break;
	case 1:
		colTitle = "静靶点";
		break;
	case 2:
		colTitle = "动靶点";
		break;
	case 3:
		colTitle = "静靶点";
		break;
	case 4:
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


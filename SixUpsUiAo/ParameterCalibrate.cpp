#include "ParameterCalibrate.h"


ParameterCalibrate::ParameterCalibrate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//关闭时 可以析构
	qDebug() << "ParameterCalibrate 构造";

	initTablesStyle(); 
	initTableWidget();//添加右键初始化
}

ParameterCalibrate::~ParameterCalibrate()
{
	qDebug() << "ParameterCalibrate 析构";
}

void ParameterCalibrate::initTablesStyle()
{
	int rowHeight = 25;//行高
	int colWidth = 80;//列宽
	int scrollBarHeight = 20;//滚动条高
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableCirclePt 待拟合参数******/
	//列设置
	ui.tableCirclePt->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableCirclePt->setColumnCount(4);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "点1" << "点2" << "点3"<< "点4";
	ui.tableCirclePt->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);
	
	//行设置
	ui.tableCirclePt->setRowCount(3);
	ui.tableCirclePt->verticalHeader()->setVisible(true);//行号可见
	ui.tableCirclePt->setVerticalHeaderLabels(VerticalHeaderXYZ);
	ui.tableCirclePt->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableCirclePt->verticalHeader()->setFixedWidth(25);
	ui.tableCirclePt->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableCirclePt->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableCirclePt->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableCirclePt->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableCirclePt->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableCirclePt->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableCirclePt->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableCirclePt->setFixedHeight(4*rowHeight+ scrollBarHeight);

	
	/**********tableCircleC  拟合后数据*****************/
	//列设置
	ui.tableCircleC->setColumnCount(1);
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "圆心" ;
	ui.tableCircleC->setHorizontalHeaderLabels(tableCircleCHorizontalHeader);
	ui.tableCircleC->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	ui.tableCircleC->setRowCount(3);
	ui.tableCircleC->verticalHeader()->setVisible(true);//行号可见
	ui.tableCircleC->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableCircleC->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableCircleC->verticalHeader()->setFixedWidth(25);
	ui.tableCircleC->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableCircleC->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableCircleC->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableCircleC->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableCircleC->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableCircleC->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableCircleC->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableCircleC->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableD_Struct ******/
	//列设置
	ui.tableD_Struct->setColumnCount(6);
	QStringList tableD_StructHorizontalHeader;
	tableD_StructHorizontalHeader << "动铰点1" << "动铰点2" << "动铰点3" << "动铰点4" << "动铰点5" << "动铰点6";
	ui.tableD_Struct->setHorizontalHeaderLabels(tableD_StructHorizontalHeader);
	ui.tableD_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	ui.tableD_Struct->setRowCount(3);
	ui.tableD_Struct->verticalHeader()->setVisible(true);//行号可见
	ui.tableD_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableD_Struct->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableD_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableD_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableD_Struct->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableD_Struct->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableD_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableD_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableD_Struct->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableD_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableD_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableS_Struct ******/
	//列设置
	ui.tableS_Struct->setColumnCount(6);
	QStringList tableS_StructHorizontalHeader;
	tableS_StructHorizontalHeader << "静铰点1" << "静铰点2" << "静铰点3" << "静铰点4" << "静铰点5" << "静铰点6";
	ui.tableS_Struct->setHorizontalHeaderLabels(tableS_StructHorizontalHeader);
	ui.tableS_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	ui.tableS_Struct->setRowCount(3);
	ui.tableS_Struct->verticalHeader()->setVisible(true);//行号可见
	ui.tableS_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableS_Struct->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableS_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableS_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableS_Struct->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableS_Struct->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableS_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableS_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableS_Struct->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableS_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableS_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableQ_DD_Struct ******/
	//列设置
	ui.tableQ_DD_Struct->setColumnCount(6);
	QStringList tableQ_DD_StructHorizontalHeader;
	tableQ_DD_StructHorizontalHeader << "动靶标1" << "动靶标2" << "动靶标3" << "动靶标4" << "动靶标5" << "动靶标6";
	ui.tableQ_DD_Struct->setHorizontalHeaderLabels(tableQ_DD_StructHorizontalHeader);
	ui.tableQ_DD_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	ui.tableQ_DD_Struct->setRowCount(3);
	ui.tableQ_DD_Struct->verticalHeader()->setVisible(true);//行号可见
	ui.tableQ_DD_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableQ_DD_Struct->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableQ_DD_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableQ_DD_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableQ_DD_Struct->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableQ_DD_Struct->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableQ_DD_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableQ_DD_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableQ_DD_Struct->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableQ_DD_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableQ_DD_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableQ_SS_Struct ******/
	//列设置
	ui.tableQ_SS_Struct->setColumnCount(6);
	QStringList tableQ_SS_StructHorizontalHeader;
	tableQ_SS_StructHorizontalHeader << "静靶标1" << "静靶标2" << "静靶标3" << "静靶标4" << "静靶标5" << "静靶标6";
	ui.tableQ_SS_Struct->setHorizontalHeaderLabels(tableQ_SS_StructHorizontalHeader);
	ui.tableQ_SS_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//行设置
	ui.tableQ_SS_Struct->setRowCount(3);
	ui.tableQ_SS_Struct->verticalHeader()->setVisible(true);//行号可见
	ui.tableQ_SS_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableQ_SS_Struct->verticalHeader()->setFont(QFont("黑体", 12));
	ui.tableQ_SS_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableQ_SS_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableQ_SS_Struct->horizontalHeader()->setStretchLastSection(true);//行头自适应表格
	ui.tableQ_SS_Struct->horizontalHeader()->setFont(QFont("黑体", 12));//表头字体
	ui.tableQ_SS_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//表头和第一行之间横线
	ui.tableQ_SS_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableQ_SS_Struct->setAlternatingRowColors(true);//行奇偶颜色不同
	ui.tableQ_SS_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //设置默认行高
	ui.tableQ_SS_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);
}

void ParameterCalibrate::initTableWidget()
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
	csvToTable("./Data/D.csv", ui.tableD_Struct);
	tableToMatrixXd(ui.tableD_Struct, UPSData::D);

	csvToTable("./Data/S.csv", ui.tableS_Struct);
	tableToMatrixXd(ui.tableS_Struct, UPSData::S);

	csvToTable("./Data/Q_DD.csv", ui.tableQ_DD_Struct);
	vector<int>Q_DDindex;
	tableToMatrixXd(ui.tableQ_DD_Struct, UPSData::Q_DD, Q_DDindex);

	csvToTable("./Data/Q_SS.csv", ui.tableQ_SS_Struct);
	vector<int>Q_SSindex;
	tableToMatrixXd(ui.tableQ_SS_Struct, UPSData::Q_SS, Q_SSindex);
	
}

#include "ParameterCalibrate.h"


ParameterCalibrate::ParameterCalibrate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//�ر�ʱ ��������
	qDebug() << "ParameterCalibrate ����";

	initTablesStyle(); 
	initTableWidget();//����Ҽ���ʼ��
}

ParameterCalibrate::~ParameterCalibrate()
{
	qDebug() << "ParameterCalibrate ����";
}

void ParameterCalibrate::initTablesStyle()
{
	int rowHeight = 25;//�и�
	int colWidth = 80;//�п�
	int scrollBarHeight = 20;//��������
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableCirclePt ����ϲ���******/
	//������
	ui.tableCirclePt->horizontalHeader()->setDefaultSectionSize(colWidth);
	ui.tableCirclePt->setColumnCount(4);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "��1" << "��2" << "��3"<< "��4";
	ui.tableCirclePt->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);
	
	//������
	ui.tableCirclePt->setRowCount(3);
	ui.tableCirclePt->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableCirclePt->setVerticalHeaderLabels(VerticalHeaderXYZ);
	ui.tableCirclePt->verticalHeader()->setFont(QFont("����", 12));
	ui.tableCirclePt->verticalHeader()->setFixedWidth(25);
	ui.tableCirclePt->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableCirclePt->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableCirclePt->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableCirclePt->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableCirclePt->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableCirclePt->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableCirclePt->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableCirclePt->setFixedHeight(4*rowHeight+ scrollBarHeight);

	
	/**********tableCircleC  ��Ϻ�����*****************/
	//������
	ui.tableCircleC->setColumnCount(1);
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "Բ��" ;
	ui.tableCircleC->setHorizontalHeaderLabels(tableCircleCHorizontalHeader);
	ui.tableCircleC->horizontalHeader()->setDefaultSectionSize(colWidth);
	//������
	ui.tableCircleC->setRowCount(3);
	ui.tableCircleC->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableCircleC->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableCircleC->verticalHeader()->setFont(QFont("����", 12));
	ui.tableCircleC->verticalHeader()->setFixedWidth(25);
	ui.tableCircleC->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableCircleC->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableCircleC->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableCircleC->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableCircleC->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableCircleC->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableCircleC->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableCircleC->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableD_Struct ******/
	//������
	ui.tableD_Struct->setColumnCount(6);
	QStringList tableD_StructHorizontalHeader;
	tableD_StructHorizontalHeader << "���µ�1" << "���µ�2" << "���µ�3" << "���µ�4" << "���µ�5" << "���µ�6";
	ui.tableD_Struct->setHorizontalHeaderLabels(tableD_StructHorizontalHeader);
	ui.tableD_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//������
	ui.tableD_Struct->setRowCount(3);
	ui.tableD_Struct->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableD_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableD_Struct->verticalHeader()->setFont(QFont("����", 12));
	ui.tableD_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableD_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableD_Struct->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableD_Struct->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableD_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableD_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableD_Struct->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableD_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableD_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableS_Struct ******/
	//������
	ui.tableS_Struct->setColumnCount(6);
	QStringList tableS_StructHorizontalHeader;
	tableS_StructHorizontalHeader << "���µ�1" << "���µ�2" << "���µ�3" << "���µ�4" << "���µ�5" << "���µ�6";
	ui.tableS_Struct->setHorizontalHeaderLabels(tableS_StructHorizontalHeader);
	ui.tableS_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//������
	ui.tableS_Struct->setRowCount(3);
	ui.tableS_Struct->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableS_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableS_Struct->verticalHeader()->setFont(QFont("����", 12));
	ui.tableS_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableS_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableS_Struct->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableS_Struct->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableS_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableS_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableS_Struct->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableS_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableS_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableQ_DD_Struct ******/
	//������
	ui.tableQ_DD_Struct->setColumnCount(6);
	QStringList tableQ_DD_StructHorizontalHeader;
	tableQ_DD_StructHorizontalHeader << "���б�1" << "���б�2" << "���б�3" << "���б�4" << "���б�5" << "���б�6";
	ui.tableQ_DD_Struct->setHorizontalHeaderLabels(tableQ_DD_StructHorizontalHeader);
	ui.tableQ_DD_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//������
	ui.tableQ_DD_Struct->setRowCount(3);
	ui.tableQ_DD_Struct->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableQ_DD_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableQ_DD_Struct->verticalHeader()->setFont(QFont("����", 12));
	ui.tableQ_DD_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableQ_DD_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableQ_DD_Struct->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableQ_DD_Struct->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableQ_DD_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableQ_DD_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableQ_DD_Struct->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableQ_DD_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	ui.tableQ_DD_Struct->setFixedHeight(4 * rowHeight + scrollBarHeight);

	/*****tableQ_SS_Struct ******/
	//������
	ui.tableQ_SS_Struct->setColumnCount(6);
	QStringList tableQ_SS_StructHorizontalHeader;
	tableQ_SS_StructHorizontalHeader << "���б�1" << "���б�2" << "���б�3" << "���б�4" << "���б�5" << "���б�6";
	ui.tableQ_SS_Struct->setHorizontalHeaderLabels(tableQ_SS_StructHorizontalHeader);
	ui.tableQ_SS_Struct->horizontalHeader()->setDefaultSectionSize(colWidth);
	//������
	ui.tableQ_SS_Struct->setRowCount(3);
	ui.tableQ_SS_Struct->verticalHeader()->setVisible(true);//�кſɼ�
	ui.tableQ_SS_Struct->setVerticalHeaderLabels(VerticalHeaderXYZ);

	ui.tableQ_SS_Struct->verticalHeader()->setFont(QFont("����", 12));
	ui.tableQ_SS_Struct->verticalHeader()->setFixedWidth(25);
	ui.tableQ_SS_Struct->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	ui.tableQ_SS_Struct->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	ui.tableQ_SS_Struct->horizontalHeader()->setFont(QFont("����", 12));//��ͷ����
	ui.tableQ_SS_Struct->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	ui.tableQ_SS_Struct->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	ui.tableQ_SS_Struct->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	ui.tableQ_SS_Struct->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
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
	vector<int> dataIndex;//�����ݵ�������
	tableToMatrixXd(ui.tableCirclePt, UPSData::circlePt, dataIndex);//�ӱ���д��
	if (UPSData::circlePt.cols() != 0)
	{
		solveOriginalPtByCircle(UPSData::circlePt, UPSData::circleC, UPSData::r);//���Բ��
		matrixXdToTable(UPSData::circleC, ui.tableCircleC);
	}
	else
	{
		qDebug() << "on_calCircleC_clicked ������";
	}
}
void ParameterCalibrate::on_calCircleS_clicked()
{
	qDebug() << "on_calCircleS_clicked";
	vector<int> dataIndex;//�����ݵ�������
	tableToMatrixXd(ui.tableCirclePt, UPSData::circlePt, dataIndex);//�ӱ���д��
	if (UPSData::circlePt.cols() != 0)
	{
		solveOriginalPtBySphere(UPSData::circlePt, UPSData::circleC, UPSData::r);//�������
		matrixXdToTable(UPSData::circleC, ui.tableCircleC);
	}
	else
	{
		qDebug() << "on_calCircleS_clicked ������";
	}
}

void ParameterCalibrate::addTableWidgetMenu()
{
	//��ȡ�źŷ�����
	QTableWidget *senderTableWidget = qobject_cast<QTableWidget*>(sender());
	//qDebug() << senderTableWidget->objectName();

	/************���ݱ�����趨��ͬ�б���**************/
	QString curTabName = senderTableWidget->objectName();//��ȡ�����źŵı����
	QStringList tabNameList;//���ֱ����
	tabNameList << "tableCirclePt" << "tableMPt_D" << "tableMPt_M" << "tableTarMPt_M" << "tableMPt_M_Comp";
	QString colTitle;//�б���
	switch (tabNameList.indexOf(curTabName))
	{
	case 0:
		colTitle = "���";
		break;
	case 1:
		colTitle = "�����";
		break;
	case 2:
		colTitle = "�����";
		break;
	case 3:
		colTitle = "Ŀ������";
		break;
	case 4:
		colTitle = "�����";
		break;
	default:
		break;
	}
	/*******************************************/
	//���ò˵�ѡ��
	QMenu *tableWidgetMenu = new QMenu(senderTableWidget);
	QAction *addColumn = new QAction("�����", senderTableWidget);
	QAction *deleteColumn = new QAction("ɾ����ǰ��", senderTableWidget);
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
			senderTableWidget->removeColumn(tmpItem->column());//ɾ����
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

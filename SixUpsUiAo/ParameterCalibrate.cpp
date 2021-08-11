#include "ParameterCalibrate.h"


ParameterCalibrate::ParameterCalibrate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//�ر�ʱ ��������
	qDebug() << "ParameterCalibrate ����";

	initTablesStyle(); 
	initTableWidget();
}

ParameterCalibrate::~ParameterCalibrate()
{
	qDebug() << "ParameterCalibrate ����";
}

void ParameterCalibrate::initTablesStyle()
{
	int rowHeight = 25;
	int scrollBarHeight = 20;
	/*tableCirclePt ����ϲ���*/
	//������
	ui.tableCirclePt->setColumnCount(4);
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "��1" << "��2" << "��3"<< "��4";
	ui.tableCirclePt->setHorizontalHeaderLabels(tableCirclePtHorizontalHeader);
	//������
	ui.tableCirclePt->setRowCount(3);
	ui.tableCirclePt->verticalHeader()->setVisible(true);//�кſɼ�
	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
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
	/*tableCircleC  ��Ϻ�����*/
	//������
	ui.tableCircleC->setColumnCount(1);
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "Բ��" ;
	ui.tableCircleC->setHorizontalHeaderLabels(tableCircleCHorizontalHeader);
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
		qDebug() << "������";
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
		qDebug() << "������";
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

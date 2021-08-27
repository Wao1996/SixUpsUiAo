#include "ParameterCalibrate.h"


ParameterCalibrate::ParameterCalibrate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//�ر�ʱ ��������
	qDebug() << "ParameterCalibrate ����";

	initTablesStyle(); 
	initTableWidgetConnect();//����Ҽ���ʼ��
}

ParameterCalibrate::~ParameterCalibrate()
{
	qDebug() << "ParameterCalibrate ����";
}

void ParameterCalibrate::initTablesStyle()
{

	QStringList VerticalHeaderXYZ;
	VerticalHeaderXYZ << "X" << "Y" << "Z";
	/*****tableCirclePt ����ϲ���******/
	QStringList tableCirclePtHorizontalHeader;
	tableCirclePtHorizontalHeader << "��1" << "��2" << "��3" << "��4";
	setTablesStyle(ui.tableCirclePt, VerticalHeaderXYZ, tableCirclePtHorizontalHeader);
	/**********tableCircleC  ��Ϻ�����*****************/
	QStringList tableCircleCHorizontalHeader;
	tableCircleCHorizontalHeader << "Բ��";
	setTablesStyle(ui.tableCircleC, VerticalHeaderXYZ, tableCircleCHorizontalHeader);
	/*****tableD_M ******/
	QStringList tableD_HorizontalHeader;
	tableD_HorizontalHeader << "���µ�1" << "���µ�2" << "���µ�3" << "���µ�4" << "���µ�5" << "���µ�6";
	setTablesStyle(ui.tableD_M, VerticalHeaderXYZ, tableD_HorizontalHeader);
	/*****tableD ******/
	QStringList tableS_HorizontalHeader;
	tableS_HorizontalHeader << "���µ�1" << "���µ�2" << "���µ�3" << "���µ�4" << "���µ�5" << "���µ�6";
	setTablesStyle(ui.tableD, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableS_M ******/
	setTablesStyle(ui.tableS_M, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableS ******/
	setTablesStyle(ui.tableS, VerticalHeaderXYZ, tableS_HorizontalHeader);
	/*****tableQ_DM ******/
	QStringList tableQ_DHorizontalHeader;
	tableQ_DHorizontalHeader << "���б�1" << "���б�2" << "���б�3" << "���б�4" << "���б�5" << "���б�6";
	setTablesStyle(ui.tableQ_DM, VerticalHeaderXYZ, tableQ_DHorizontalHeader);
	/*****tableQ_DD ******/
	QStringList tableQ_S_HorizontalHeader;
	tableQ_S_HorizontalHeader << "���б�1" << "���б�2" << "���б�3" << "���б�4" << "���б�5" << "���б�6";
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
	tableInitL_HorizontalHeader << "��ʼ�˳�mm";
	QStringList tableInitL_VerticalHeader;
	tableInitL_VerticalHeader << "��1" << "��2" << "��3" << "��4" << "��5" << "��6";
	setTablesStyle(ui.tableInitL, tableInitL_VerticalHeader, tableInitL_HorizontalHeader,45);


}

void ParameterCalibrate::setTablesStyle(QTableWidget * table, QStringList VerticalHeader, QStringList HorizontalHeader,int verticalHeaderWidth)
{
	int rowHeight = 30;//�и�
	int colWidth = 100;//�п�
	int scrollBarHeight = 20;//��������
	int rowNum = VerticalHeader.size();
	int colNum = HorizontalHeader.size();
	//������
	table->setColumnCount(colNum);
	table->horizontalHeader()->setDefaultSectionSize(colWidth);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	table->setHorizontalHeaderLabels(HorizontalHeader);
	//������
	table->setRowCount(rowNum);
	table->verticalHeader()->setVisible(true);//�кſɼ�
	table->setVerticalHeaderLabels(VerticalHeader);
	table->verticalHeader()->setFont(QFont("����", 10));
	table->verticalHeader()->setFixedWidth(verticalHeaderWidth);
	table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	table->horizontalHeader()->setStretchLastSection(true);//��ͷ����Ӧ���
	table->horizontalHeader()->setFont(QFont("����", 10));//��ͷ����
	table->horizontalHeader()->setStyleSheet("border-bottom-width: 1px;border-style: outset;border-color: rgb(229,229,229);");//��ͷ�͵�һ��֮�����
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	table->setAlternatingRowColors(true);//����ż��ɫ��ͬ
	table->verticalHeader()->setDefaultSectionSize(rowHeight);    //����Ĭ���и�
	table->setFixedHeight((rowNum+1) * rowHeight + scrollBarHeight);
}


void ParameterCalibrate::initTableWidgetConnect()
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

void ParameterCalibrate::on_inputTestDataBtn_clicked()
{
	csvToTable("./Data/S_M_test.csv", ui.tableS_M);
	csvToTable("./Data/D_M_test.csv", ui.tableD_M);
	csvToTable("./Data/Q_SM_test.csv", ui.tableQ_SM);
	csvToTable("./Data/Q_DM_test.csv", ui.tableQ_DM);
}

void ParameterCalibrate::on_dHingeCalbrateBtn_clicked()
{

	tableToMatrixXd(ui.tableD_M, UPSData::D_M);
	Trans_DM_calibrate = calibrateHingePoint(UPSData::D_theoretical, UPSData::D_M, UPSData::D );
	
	bool flag = matrixXdToTable(UPSData::D, ui.tableD);
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨������궨�ɹ�");
		ui.dTargetCalbrateBtn->setEnabled(true);
		ui.saveDBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨������궨ʧ�ܣ�");
	}
	
}

void ParameterCalibrate::on_sHingeCalbrateBtn_clicked()
{
	//���㾲ƽ̨����ϵԭ���ڲ�������ϵ�µ�λ��
	tableToMatrixXd(ui.tableS_M, UPSData::S_M);
	Trans_SM_calibrate = calibrateHingePoint(UPSData::S_theoretical, UPSData::S_M, UPSData::S);
	cout << Trans_SM_calibrate;
	bool flag = matrixXdToTable(UPSData::S, ui.tableS);
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨������궨�ɹ�");
		ui.sTargetCalbrateBtn->setEnabled(true);
		ui.saveSBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨������궨ʧ�ܣ�");
	}
}

void ParameterCalibrate::on_dTargetCalbrateBtn_clicked()
{
	vector<int> index;
	tableToMatrixXd(ui.tableQ_DM, UPSData::Q_DM, index);
	calibrateTargetPoint(UPSData::Q_DM, Trans_DM_calibrate, UPSData::Q_DD);
	bool flag = matrixXdToTable(UPSData::Q_DD, ui.tableQ_DD);
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨�б��궨�ɹ�");
		ui.saveQ_DDBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨�б��궨ʧ�ܣ�");
	}

}

void ParameterCalibrate::on_sTargetCalbrateBtn_clicked()
{
	vector<int> index;
	tableToMatrixXd(ui.tableQ_SM, UPSData::Q_SM, index);
	calibrateTargetPoint(UPSData::Q_SM, Trans_SM_calibrate, UPSData::Q_SS);
	bool flag = matrixXdToTable(UPSData::Q_SS, ui.tableQ_SS);
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨�б��궨�ɹ�");
		ui.saveQ_SSBtn->setEnabled(true);
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "��ƽ̨�б��궨ʧ�ܣ�");
	}
}



void ParameterCalibrate::on_saveD_MBtn_clicked()
{
	bool flag = tableToCsv(ui.tableD_M, "./Data/D_M.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveS_MBtn_clicked()
{
	bool flag = tableToCsv(ui.tableS_M, "./Data/S_M.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveQ_DMBtn_clicked()
{
	bool flag = tableToCsv(ui.tableQ_DM, "./Data/Q_DM.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveQ_SMBtn_clicked()
{
	bool flag = tableToCsv(ui.tableQ_SM, "./Data/Q_SM.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�������ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveDBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::D, "./Data/D.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveSBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::S, "./Data/S.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveQ_DDBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::Q_DD, "./Data/Q_DD.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_saveQ_SSBtn_clicked()
{
	bool flag = matrixXdToCsv(UPSData::Q_SS, "./Data/Q_SS.csv");
	if (flag)
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ɹ�");
	}
	else
	{
		QMessageBox::information(NULL, "��ʾ", "�궨���ݱ���ʧ��");
	}
}

void ParameterCalibrate::on_ininLcalbrateBtn_clicked()
{
	/*���뾲ƽ̨����*/
	MatrixXd Q_SM_initL;
	vector<int> index_Q_SM_initL;
	tableToMatrixXd(ui.tableQ_SM_initL, Q_SM_initL, index_Q_SM_initL);
	/*���붯ƽ̨����*/
	MatrixXd Q_DM_initL;
	vector<int> index_Q_DM_initL;
	tableToMatrixXd(ui.tableQ_DM_initL, Q_DM_initL, index_Q_DM_initL);
	/*���㾲ƽ̨��Բ�������ϵλ��*/
	//Matrix4d Trans_SM = rigidMotionSVDSolution( UPSData::Q_SS, Q_SM_initL);
	/*�궨��ʼ�˳�*/
	//calibrateInitLength(UPSData::initL_norm, UPSData::Q_DD, Q_DM_initL, Trans_SM, UPSData::D, UPSData::S);


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

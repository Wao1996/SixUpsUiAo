#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include "ui_ParameterCalibrate.h"
#include "Global.h"
#include "typeCast.h"


class ParameterCalibrate : public QWidget
{
	Q_OBJECT

public:
	ParameterCalibrate(QWidget *parent = Q_NULLPTR);
	~ParameterCalibrate();
	Ui::ParameterCalibrate ui;
	QSize tableCirclePtSize;
	void initTablesStyle();//��ʼ�������ʽ
	void setTablesStyle(QTableWidget * table);//���ñ����ʽ
	void initTableWidgetConnect();//��ʼ������źŲ�

private slots:
	/**************�궨�ṹ����*******************/
	void on_calCircleC_clicked();//Բ�����
	void on_calCircleS_clicked();//�������
	void on_dHingeCalbrateBtn_clicked();//��ƽ̨������궨
	void on_sHingeCalbrateBtn_clicked();//��ƽ̨������궨
	void on_dTargetCalbrateBtn_clicked();//��ƽ̨�б��궨
	void on_sTargetCalbrateBtn_clicked();//��ƽ̨�б��궨
	void on_inputTestDataBtn_clicked();//�����������
	void on_saveMeasureDataBtn_clicked();//�����������
	void on_saveCalibrateDataBtn_clicked();//����궨����

	/*****************���tablewidget�Ҽ��˵�***********/
	void addTableWidgetMenu();
	void on_loadStructPara_clicked();//���붯��ƽ̨����
};

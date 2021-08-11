#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include "ui_ParameterCalibrate.h"
#include "Global.h"
#include "typeCast.h"


class ParameterCalibrate : public QWidget
{
	Q_OBJECT

public:
	ParameterCalibrate(QWidget *parent = Q_NULLPTR);
	~ParameterCalibrate();

private:
	Ui::ParameterCalibrate ui;
	QSize tableCirclePtSize;
	void initTablesStyle();//��ʼ�������ʽ
	void initTableWidget();//��ʼ������źŲ�

private slots:
	/**************�궨�ṹ����*******************/
	void on_calCircleC_clicked();//Բ�����
	void on_calCircleS_clicked();//�������

	/*****************���tablewidget�Ҽ��˵�***********/
	void addTableWidgetMenu();
};

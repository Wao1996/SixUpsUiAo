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
	void initTablesStyle();//初始化表格样式
	void initTableWidget();//初始化表格信号槽

private slots:
	/**************标定结构参数*******************/
	void on_calCircleC_clicked();//圆心拟合
	void on_calCircleS_clicked();//球心拟合

	/*****************添加tablewidget右键菜单***********/
	void addTableWidgetMenu();
};

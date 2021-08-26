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
	void initTablesStyle();//初始化表格样式
	void setTablesStyle(QTableWidget * table);//设置表格样式
	void initTableWidgetConnect();//初始化表格信号槽

private slots:
	/**************标定结构参数*******************/
	void on_calCircleC_clicked();//圆心拟合
	void on_calCircleS_clicked();//球心拟合
	void on_dHingeCalbrateBtn_clicked();//动平台铰链点标定
	void on_sHingeCalbrateBtn_clicked();//静平台铰链点标定
	void on_dTargetCalbrateBtn_clicked();//动平台靶标点标定
	void on_sTargetCalbrateBtn_clicked();//静平台靶标点标定
	void on_inputTestDataBtn_clicked();//导入测试数据
	void on_saveMeasureDataBtn_clicked();//保存测量数据
	void on_saveCalibrateDataBtn_clicked();//保存标定数据

	/*****************添加tablewidget右键菜单***********/
	void addTableWidgetMenu();
	void on_loadStructPara_clicked();//导入动静平台参数
};

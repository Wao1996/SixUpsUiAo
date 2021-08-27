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
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;//eigen库 指针对齐 否则报错
	ParameterCalibrate(QWidget *parent = Q_NULLPTR);
	~ParameterCalibrate();
	Ui::ParameterCalibrate ui;
	QSize tableCirclePtSize;

	Matrix4d Trans_DM_calibrate = Matrix4d::Zero();//标定时 动系相对测量坐标系位姿齐次矩阵;
	Matrix4d Trans_SM_calibrate = Matrix4d::Zero();//标定时 静系相对测量坐标系位姿齐次矩阵;
	Matrix4d Trans_DS_calibrate = Matrix4d::Zero();//标定时 动系相对静系位姿齐次矩阵;

	void initTablesStyle();//初始化表格样式
	void setTablesStyle(QTableWidget * table, QStringList VerticalHeader, QStringList HorizontalHeader, int verticalHeaderWidth = 30);//设置表格样式
	void initTableWidgetConnect();//初始化表格信号槽

private slots:
	/**************标定结构参数*******************/
	void on_calCircleC_clicked();//圆心拟合
	void on_calCircleS_clicked();//球心拟合
	void on_inputTestDataBtn_clicked();//导入测试数据
	/*标定*/
	void on_dHingeCalbrateBtn_clicked();//动平台铰链点标定
	void on_sHingeCalbrateBtn_clicked();//静平台铰链点标定
	void on_dTargetCalbrateBtn_clicked();//动平台靶标点标定
	void on_sTargetCalbrateBtn_clicked();//静平台靶标点标定
	/*保存测量结果*/
	void on_saveD_MBtn_clicked();//保存动平台铰链点测量结果
	void on_saveS_MBtn_clicked();//保存静平台铰链点测量结果
	void on_saveQ_DMBtn_clicked();//保存动平台靶标点测量结果
	void on_saveQ_SMBtn_clicked();//保存静平台靶标点测量结果
	/*保存标定结果*/
	void on_saveDBtn_clicked();//保存动平台铰链点标定结果
	void on_saveSBtn_clicked();//保存静平台铰链点标定结果
	void on_saveQ_DDBtn_clicked();//保存动平台靶标点标定结果
	void on_saveQ_SSBtn_clicked();//保存静平台靶标点标定结果
	/*标定初始杆长*/
	void on_ininLcalbrateBtn_clicked();//标定初始杆长



	/*****************添加tablewidget右键菜单***********/
	void addTableWidgetMenu();
	void on_loadStructPara_clicked();//导入动静平台参数
};

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
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;//eigen�� ָ����� ���򱨴�
	ParameterCalibrate(QWidget *parent = Q_NULLPTR);
	~ParameterCalibrate();
	Ui::ParameterCalibrate ui;
	QSize tableCirclePtSize;

	Matrix4d Trans_DM_calibrate = Matrix4d::Zero();//�궨ʱ ��ϵ��Բ�������ϵλ����ξ���;
	Matrix4d Trans_SM_calibrate = Matrix4d::Zero();//�궨ʱ ��ϵ��Բ�������ϵλ����ξ���;
	Matrix4d Trans_DS_calibrate = Matrix4d::Zero();//�궨ʱ ��ϵ��Ծ�ϵλ����ξ���;

	void initTablesStyle();//��ʼ�������ʽ
	void setTablesStyle(QTableWidget * table, QStringList VerticalHeader, QStringList HorizontalHeader, int verticalHeaderWidth = 30);//���ñ����ʽ
	void initTableWidgetConnect();//��ʼ������źŲ�

private slots:
	/**************�궨�ṹ����*******************/
	void on_calCircleC_clicked();//Բ�����
	void on_calCircleS_clicked();//�������
	void on_inputTestDataBtn_clicked();//�����������
	/*�궨*/
	void on_dHingeCalbrateBtn_clicked();//��ƽ̨������궨
	void on_sHingeCalbrateBtn_clicked();//��ƽ̨������궨
	void on_dTargetCalbrateBtn_clicked();//��ƽ̨�б��궨
	void on_sTargetCalbrateBtn_clicked();//��ƽ̨�б��궨
	/*����������*/
	void on_saveD_MBtn_clicked();//���涯ƽ̨������������
	void on_saveS_MBtn_clicked();//���澲ƽ̨������������
	void on_saveQ_DMBtn_clicked();//���涯ƽ̨�б��������
	void on_saveQ_SMBtn_clicked();//���澲ƽ̨�б��������
	/*����궨���*/
	void on_saveDBtn_clicked();//���涯ƽ̨������궨���
	void on_saveSBtn_clicked();//���澲ƽ̨������궨���
	void on_saveQ_DDBtn_clicked();//���涯ƽ̨�б��궨���
	void on_saveQ_SSBtn_clicked();//���澲ƽ̨�б��궨���
	/*�궨��ʼ�˳�*/
	void on_ininLcalbrateBtn_clicked();//�궨��ʼ�˳�



	/*****************���tablewidget�Ҽ��˵�***********/
	void addTableWidgetMenu();
	void on_loadStructPara_clicked();//���붯��ƽ̨����
};

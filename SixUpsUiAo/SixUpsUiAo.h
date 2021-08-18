#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "ui_SixUpsUiAo.h"
#include "ParameterCalibrate.h"
#include "WaitWindow.h"
#include "Global.h"
#include "QPmac.h"
#include "UpsCalculateThread.h"

class SixUpsUiAo : public QMainWindow
{
    Q_OBJECT

public:
    SixUpsUiAo(QWidget *parent = Q_NULLPTR);
	~SixUpsUiAo();

	ParameterCalibrate * ParameterCalibrateUI;
	WaitWindow *WaitWindowUI;
	bool upsHomeSignalEmited = false;//并联机构回零程序发送标志
private:
    Ui::SixUpsUiAoClass ui;

	QPixmap onIcon;
	QPixmap offIcon;
	QPixmap loadingIcon;

	//各支链限位状态图片列表  因为C++不能使用eval语句
	QList<QLabel*> qlabNegLimit_group;
	QList<QLabel*> qlabPosLimit_group;
	QList<QLineEdit *> realTimeLengths_group;//杆长显示
	QList<QLineEdit *> realTimePos_group;//位姿显示
	QList<QDoubleSpinBox *> AbsTarPos_group;//多轴运动 绝对位置输入框
	QList<QDoubleSpinBox *> jogInc_group;//单轴运动 距离点动增量
	QList<QToolButton *> dipJog_group;//单轴运动 距离点动按钮
	QList<QToolButton *> prsJogPos_group;//单轴运动 长按正向运动
	QList<QToolButton *> prsJogNeg_group;//单轴运动 长按负向运动

	/************定时器***********/
	QTimer *dataGatherTimer;//数据收集定时器
	QTimer *updateUiDataTimer;//刷新ui定时器
	QTimer *upsCalculateTimer;//并联机构计算定时器
	QTimer *upsHomeCompleteTimer;//并联机构归零判断定时器
	/***********并联机构计算线程**********/
	QThread *calculateQthread = nullptr;
	UpsCalculateThread *myUpsCalculateThread = nullptr;

	/***********初始化相关********/
	void initIcon();
	void initUIList();
	void initStructPara();
	void initConnect();
	/*********Pmac相关************/
	QPmac *myPmac = nullptr;
	void switchPmacThread();//切换Pmac线程的开启与关闭

	/************平台相关*************/
	bool QMesBoxWhetherHome();//平台是否回归零位对话框
signals:
	void upsHome_signal();
	void platformDHome_signal();//动平台回初始零位
private slots:
	
	void upsHome_slot();//并联机构回零信号槽
	void platformDHome_slot();//动平台回初始零位 信号槽
	/*********工具栏************/
	void on_paraCailbrate_triggered();
	/*********定时器***********/
	void on_updateUiDataTimer();//刷新界面数据定时器溢出
	void on_upsHomeCompleteTimer();//各轴回零完成状态判断

	/**********PMAC按钮************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();

	/**************电机使能按钮************/
	void on_servoOnBtn_clicked();
	void on_servoOffBtn_clicked();
	/***********多轴运动***************/
	void on_getRealTimePosBtn_clicked();//获取当前位姿
	void on_startMoveBtn_clicked();//开始运动
	void absTarPos_group_valueChange(double);
	/***********单轴运动***************/
	void on_led_jogSpeed_valueChanged(double);//单轴运动速度改变
	void on_jogStopBtn_clicked();//所有电机停止
	void led_jogInc_valueChanged(double);//距离点动 输入框 信号槽
	void dipJog_Clicked();//距离电动 按钮点击 信号槽
	void prsJogPos_pressed();//长按点动 正方向按钮按下 信号槽
	void prsJogPos_released();//长按点动 正方向按钮松开 信号槽
	void prsJogNeg_pressed();//长按点动 负方向按钮按下 信号槽
	void prsJogNeg_released();//长按点动 正方向按钮松开 信号槽

};

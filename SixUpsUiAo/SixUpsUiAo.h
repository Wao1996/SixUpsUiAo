#pragma once
//#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QButtonGroup>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileDialog>
#include "ui_SixUpsUiAo.h"
#include "ParameterCalibrate.h"
#include "WaitWindow.h"
#include "Global.h"
#include "QPmac.h"
#include "UpsCalculateThread.h"
#include "HMIcommunication.h"
#include "qcustomplot.h"
#include "CurvePlot.h"

class SixUpsUiAo : public QMainWindow
{
    Q_OBJECT

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;			//eigen库 指针对齐 否则报错
    SixUpsUiAo(QWidget *parent = Q_NULLPTR);
	~SixUpsUiAo();
	/*窗口*/
	ParameterCalibrate * ParameterCalibrateUI;	//参数标定窗口
	WaitWindow *WaitWindowUI;					//等待窗口
	bool upsHomeSignalEmited = false;			//并联机构回零程序发送标志
private:
    Ui::SixUpsUiAoClass ui;

	QPixmap onIcon;
	QPixmap offIcon;
	QPixmap loadingIcon;

	//各支链限位状态图片列表  因为C++不能使用eval语句
	QList<QLabel*> qlabNegLimit_group;				//轴负限位开关状态图标
	QList<QLabel*> qlabPosLimit_group;				//轴正限位开关状态图标
	QList<QLabel*> qlabOrigin_group;				//轴原点开关状态图标
	QList<QLineEdit *> realTimeLengths_group;		//杆长显示
	QList<QLineEdit *> realTimeForce_group;			//力显示
	QList<QLineEdit *> realTimePose_DS_group;		//动平台相对静平台 位姿显示
	QList<QLineEdit *> realTimePose_setS_group;		//运动坐标系相对静平台  位姿显示
	QList<QLineEdit *> realTimePose_Dset_group;		//动平台相对运动坐标系 位姿显示
	QList<QDoubleSpinBox *> AbsTarPos_group;		//多轴运动 绝对位置输入框
	QList<QDoubleSpinBox *> jogInc_group;			//单轴运动 距离点动增量
	QList<QToolButton *> dipJog_group;				//单轴运动 距离点动按钮
	QList<QToolButton *> prsJogPos_group;			//单轴运动 长按正向运动
	QList<QToolButton *> prsJogNeg_group;			//单轴运动 长按负向运动
	QButtonGroup *btnGroupMultiAxisDirection;		//多轴点动 按钮组 判断哪个运动方向被选中
	QButtonGroup *btnGroupMultiSpeed;				//多轴联动 速度 按钮组
	/************定时器***********/
	QTimer *dataGatherTimer;			//数据收集定时器
	QTimer *updateUiDataTimer;			//刷新ui定时器
	QTimer *upsCalculateTimer;			//并联机构计算定时器
	QTimer *upsHomeCompleteTimer;		//并联机构归零判断定时器
	QTimer *upsJogTimer;				//并联机构长按点动定时器
	/************多线程**********/
	//并联机构计算线程
	QThread *calculateQthread = nullptr;
	UpsCalculateThread *myUpsCalculateThread = nullptr;
	//HMI通讯线程
	QThread *HMIQthread = nullptr;
	HMIcommunication *myHMIcommunication = nullptr;
	/***********初始化相关********/
	void initIcon();
	void initUIList();
	void initStructPara();
	void initTablesStyle();
	void initConnect();
	/*******UI控件相关**********/
	void myWidgetEnable();				//控件使能
	void myWidgetDisnable();			//控件失去使能
	/*********Pmac相关************/
	QPmac *myPmac = nullptr;
	void switchPmacThread();			//切换Pmac线程的开启与关闭
	/**********平台相关函数*************/
	bool QMesBoxWhetherHome();			//平台是否回归零位对话框
	/*导入关键点表格*/
	bool inputKeyPoint(const QString & filePath, QTableWidget * tab);
	/*退出时记录当前位姿PMAC中各轴相对自身原点的长度*/
	void recordCurLengthsMM();
	/*连接PMAC后 导入上次退出时的PMAC中各轴相对自身原点的长度*/
	bool loadLastLengthsMM();
	//要删除的文件夹或文件的路径
	bool DeleteFileOrFolder(const QString &strPath);


	/***********力绘制***********/
	QTimer *plotTimer;					//开始实时绘图定时器
	QDateTime startTime;
	QDateTime endTime;
	double intervalTime = 0;
	QList<QVector<double>> recordData;
	/*曲线操作绘制*/
	void addPoint(CurvePlot *graph, const double &x_temp, const double &y_temp);//添加数据点
	/*1号轴轴*/
	QCustomPlot *widget_Force1;
	CurvePlot *curves_Force1;

	QCustomPlot *widget_Force2;
	CurvePlot *curves_Force2;

	QCustomPlot *widget_Force3;
	CurvePlot *curves_Force3;

	QCustomPlot *widget_Force4;
	CurvePlot *curves_Force4;

	QCustomPlot *widget_Force5;
	CurvePlot *curves_Force5;

	QCustomPlot *widget_Force6;
	CurvePlot *curves_Force6;

	QCustomPlot *widget_Force7;
	CurvePlot *curves_Force7;

signals:
	void upsHome_signal();				//并联机构回零位信号
	void platformDHome_signal();		//动平台回初始零位信号
private slots:
	
	void upsHome_slot();					//并联机构回零信号槽
	void platformDHome_slot();				//动平台回初始零位 信号槽
	/*********工具栏************/
	void on_paraCailbrate_triggered();
	/*********定时器***********/
	void on_updateUiDataTimer();			//刷新界面数据定时器溢出
	void on_upsHomeCompleteTimer();			//各轴回零完成状态判断
	void on_upsJogTimer();					//并联机构点动定时器
	/***************PMAC按钮************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();
	/*电机使能按钮*/
	void on_servoOnBtn_clicked();
	void on_servoOffBtn_clicked();
	/*各轴回零*/
	void on_axlesHomeBtn_clicked();
	/*平台回零*/
	void on_upsHomeBtn_clicked();
	/***************测量准备***************/
	void on_calibrateSMBtn_clicked();		//静平台在测量坐标系
	void on_calibrateGMBtn_clicked();		//工装在动平台坐标系
	//表格右键添加列
	void addTableWidgetMenuAddCoils();
	/***************工装目标位姿转换*************/
	void on_transTarPoseBtn_clicked();		//工装目标位姿转换
	/*************设置运动原点***********/
	void on_setOriginBtn_clicked();			//将测量坐标系中的点设为运动原点
	void on_setSPosOriginBtn_clicked();		//将静坐标系原点设为运动原点
	void on_setCurPosOriginBtn_clicked();	//将当前位姿设为运动原点
	/*************记录关键点************/
	void on_recordKeyPointBtn_clicked();
	void on_inputKeyPointBtn_clicked();
	void on_saveKeyPointBtn_clicked();
	//表格右键
	void addTableWidgetMenuKeyPoint();
	/***********多轴运动***************/
	//联动
	void btnGroupMultiSpeed_clicked(int id);		//多轴运动联动速度选择
	void on_getRealTimePosBtn_clicked();			//获取当前位姿
	void on_startMoveBtn_clicked();					//开始运动
	void on_stopMoveBtn_clicked();					//停止运动
	void absTarPos_group_valueChanged(double);		//运动方向变化
	//点动
	void btnGroupMultiAxisDirection_clicked(int id);				//点动运动方向
	void on_led_multiAxisJogTranslationSpeed_valueChanged(double);	//平动点动速度改变
	void on_led_multiAxisJogTranslationStep_valueChanged(double);	//平动点动步长改变
	void on_led_multiAxisJogRotateSpeed_valueChanged(double);		//转动点动速度改变
	void on_led_multiAxisJogRotateStep_valueChanged(double);		//转动点动步长改变
	void on_disMultiAxisJog_clicked();								//距离点动按钮
	void on_disMultiAxisJog_stop_clicked();							//距离点动停止按钮
	void on_prsMultiAxisJogNeg_pressed();							//负方向长按点动按下
	void on_prsMultiAxisJogNeg_released();							//负方向长按点动松开
	void on_prsmultiAxisJogPos_pressed();							//正方向长按点动按下
	void on_prsmultiAxisJogPos_released();							//正方向长按点动松开
	/***********单轴运动***************/
	void on_led_jogSpeed_valueChanged(double);		//单轴运动速度改变
	void on_jogStopBtn_clicked();					//所有电机停止
	void led_jogInc_valueChanged(double);			//距离点动 输入框 信号槽
	void dipJog_Clicked();							//距离电动 按钮点击 信号槽
	void prsJogPos_pressed();						//长按点动 正方向按钮按下 信号槽
	void prsJogPos_released();						//长按点动 正方向按钮松开 信号槽
	void prsJogNeg_pressed();						//长按点动 负方向按钮按下 信号槽
	void prsJogNeg_released();						//长按点动 正方向按钮松开 信号槽
	/**************HMI相关************/
	//按钮
	void on_openHMIBtn_clicked();
	void on_closeHMIBtn_clicked();
	//写保持寄存器
	void wirteRegisters_slot(const int & address, const double & data);
	void writeCoils_slot(const int & address, const bool & flag);
	/************导入数据 矩阵变换位姿*********/
	void on_inputBtn_clicked();

	/*******力绘制********/
	void on_plotForceBtn_toggled(bool checked);
	void on_clearForceBtn_clicked();
	void on_recordForceBtn_clicked();
	void on_plotTimer();					//力绘制定时器
};
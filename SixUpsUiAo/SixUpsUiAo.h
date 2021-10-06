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
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;			//eigen�� ָ����� ���򱨴�
    SixUpsUiAo(QWidget *parent = Q_NULLPTR);
	~SixUpsUiAo();
	/*����*/
	ParameterCalibrate * ParameterCalibrateUI;	//�����궨����
	WaitWindow *WaitWindowUI;					//�ȴ�����
	bool upsHomeSignalEmited = false;			//����������������ͱ�־
private:
    Ui::SixUpsUiAoClass ui;

	QPixmap onIcon;
	QPixmap offIcon;
	QPixmap loadingIcon;

	//��֧����λ״̬ͼƬ�б�  ��ΪC++����ʹ��eval���
	QList<QLabel*> qlabNegLimit_group;				//�Ḻ��λ����״̬ͼ��
	QList<QLabel*> qlabPosLimit_group;				//������λ����״̬ͼ��
	QList<QLabel*> qlabOrigin_group;				//��ԭ�㿪��״̬ͼ��
	QList<QLineEdit *> realTimeLengths_group;		//�˳���ʾ
	QList<QLineEdit *> realTimeForce_group;			//����ʾ
	QList<QLineEdit *> realTimePose_DS_group;		//��ƽ̨��Ծ�ƽ̨ λ����ʾ
	QList<QLineEdit *> realTimePose_setS_group;		//�˶�����ϵ��Ծ�ƽ̨  λ����ʾ
	QList<QLineEdit *> realTimePose_Dset_group;		//��ƽ̨����˶�����ϵ λ����ʾ
	QList<QDoubleSpinBox *> AbsTarPos_group;		//�����˶� ����λ�������
	QList<QDoubleSpinBox *> jogInc_group;			//�����˶� ����㶯����
	QList<QToolButton *> dipJog_group;				//�����˶� ����㶯��ť
	QList<QToolButton *> prsJogPos_group;			//�����˶� ���������˶�
	QList<QToolButton *> prsJogNeg_group;			//�����˶� ���������˶�
	QButtonGroup *btnGroupMultiAxisDirection;		//����㶯 ��ť�� �ж��ĸ��˶�����ѡ��
	QButtonGroup *btnGroupMultiSpeed;				//�������� �ٶ� ��ť��
	/************��ʱ��***********/
	QTimer *dataGatherTimer;			//�����ռ���ʱ��
	QTimer *updateUiDataTimer;			//ˢ��ui��ʱ��
	QTimer *upsCalculateTimer;			//�����������㶨ʱ��
	QTimer *upsHomeCompleteTimer;		//�������������ж϶�ʱ��
	QTimer *upsJogTimer;				//�������������㶯��ʱ��
	/************���߳�**********/
	//�������������߳�
	QThread *calculateQthread = nullptr;
	UpsCalculateThread *myUpsCalculateThread = nullptr;
	//HMIͨѶ�߳�
	QThread *HMIQthread = nullptr;
	HMIcommunication *myHMIcommunication = nullptr;
	/***********��ʼ�����********/
	void initIcon();
	void initUIList();
	void initStructPara();
	void initTablesStyle();
	void initConnect();
	/*******UI�ؼ����**********/
	void myWidgetEnable();				//�ؼ�ʹ��
	void myWidgetDisnable();			//�ؼ�ʧȥʹ��
	/*********Pmac���************/
	QPmac *myPmac = nullptr;
	void switchPmacThread();			//�л�Pmac�̵߳Ŀ�����ر�
	/**********ƽ̨��غ���*************/
	bool QMesBoxWhetherHome();			//ƽ̨�Ƿ�ع���λ�Ի���
	/*����ؼ�����*/
	bool inputKeyPoint(const QString & filePath, QTableWidget * tab);
	/*�˳�ʱ��¼��ǰλ��PMAC�и����������ԭ��ĳ���*/
	void recordCurLengthsMM();
	/*����PMAC�� �����ϴ��˳�ʱ��PMAC�и����������ԭ��ĳ���*/
	bool loadLastLengthsMM();
	//Ҫɾ�����ļ��л��ļ���·��
	bool DeleteFileOrFolder(const QString &strPath);


	/***********������***********/
	QTimer *plotTimer;					//��ʼʵʱ��ͼ��ʱ��
	QDateTime startTime;
	QDateTime endTime;
	double intervalTime = 0;
	QList<QVector<double>> recordData;
	/*���߲�������*/
	void addPoint(CurvePlot *graph, const double &x_temp, const double &y_temp);//������ݵ�
	/*1������*/
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
	void upsHome_signal();				//������������λ�ź�
	void platformDHome_signal();		//��ƽ̨�س�ʼ��λ�ź�
private slots:
	
	void upsHome_slot();					//�������������źŲ�
	void platformDHome_slot();				//��ƽ̨�س�ʼ��λ �źŲ�
	/*********������************/
	void on_paraCailbrate_triggered();
	/*********��ʱ��***********/
	void on_updateUiDataTimer();			//ˢ�½������ݶ�ʱ�����
	void on_upsHomeCompleteTimer();			//����������״̬�ж�
	void on_upsJogTimer();					//���������㶯��ʱ��
	/***************PMAC��ť************/
	void on_connectPmacBtn_clicked();
	void on_initPmacBtn_clicked();
	/*���ʹ�ܰ�ť*/
	void on_servoOnBtn_clicked();
	void on_servoOffBtn_clicked();
	/*�������*/
	void on_axlesHomeBtn_clicked();
	/*ƽ̨����*/
	void on_upsHomeBtn_clicked();
	/***************����׼��***************/
	void on_calibrateSMBtn_clicked();		//��ƽ̨�ڲ�������ϵ
	void on_calibrateGMBtn_clicked();		//��װ�ڶ�ƽ̨����ϵ
	//����Ҽ������
	void addTableWidgetMenuAddCoils();
	/***************��װĿ��λ��ת��*************/
	void on_transTarPoseBtn_clicked();		//��װĿ��λ��ת��
	/*************�����˶�ԭ��***********/
	void on_setOriginBtn_clicked();			//����������ϵ�еĵ���Ϊ�˶�ԭ��
	void on_setSPosOriginBtn_clicked();		//��������ϵԭ����Ϊ�˶�ԭ��
	void on_setCurPosOriginBtn_clicked();	//����ǰλ����Ϊ�˶�ԭ��
	/*************��¼�ؼ���************/
	void on_recordKeyPointBtn_clicked();
	void on_inputKeyPointBtn_clicked();
	void on_saveKeyPointBtn_clicked();
	//����Ҽ�
	void addTableWidgetMenuKeyPoint();
	/***********�����˶�***************/
	//����
	void btnGroupMultiSpeed_clicked(int id);		//�����˶������ٶ�ѡ��
	void on_getRealTimePosBtn_clicked();			//��ȡ��ǰλ��
	void on_startMoveBtn_clicked();					//��ʼ�˶�
	void on_stopMoveBtn_clicked();					//ֹͣ�˶�
	void absTarPos_group_valueChanged(double);		//�˶�����仯
	//�㶯
	void btnGroupMultiAxisDirection_clicked(int id);				//�㶯�˶�����
	void on_led_multiAxisJogTranslationSpeed_valueChanged(double);	//ƽ���㶯�ٶȸı�
	void on_led_multiAxisJogTranslationStep_valueChanged(double);	//ƽ���㶯�����ı�
	void on_led_multiAxisJogRotateSpeed_valueChanged(double);		//ת���㶯�ٶȸı�
	void on_led_multiAxisJogRotateStep_valueChanged(double);		//ת���㶯�����ı�
	void on_disMultiAxisJog_clicked();								//����㶯��ť
	void on_disMultiAxisJog_stop_clicked();							//����㶯ֹͣ��ť
	void on_prsMultiAxisJogNeg_pressed();							//�����򳤰��㶯����
	void on_prsMultiAxisJogNeg_released();							//�����򳤰��㶯�ɿ�
	void on_prsmultiAxisJogPos_pressed();							//�����򳤰��㶯����
	void on_prsmultiAxisJogPos_released();							//�����򳤰��㶯�ɿ�
	/***********�����˶�***************/
	void on_led_jogSpeed_valueChanged(double);		//�����˶��ٶȸı�
	void on_jogStopBtn_clicked();					//���е��ֹͣ
	void led_jogInc_valueChanged(double);			//����㶯 ����� �źŲ�
	void dipJog_Clicked();							//����綯 ��ť��� �źŲ�
	void prsJogPos_pressed();						//�����㶯 ������ť���� �źŲ�
	void prsJogPos_released();						//�����㶯 ������ť�ɿ� �źŲ�
	void prsJogNeg_pressed();						//�����㶯 ������ť���� �źŲ�
	void prsJogNeg_released();						//�����㶯 ������ť�ɿ� �źŲ�
	/**************HMI���************/
	//��ť
	void on_openHMIBtn_clicked();
	void on_closeHMIBtn_clicked();
	//д���ּĴ���
	void wirteRegisters_slot(const int & address, const double & data);
	void writeCoils_slot(const int & address, const bool & flag);
	/************�������� ����任λ��*********/
	void on_inputBtn_clicked();

	/*******������********/
	void on_plotForceBtn_toggled(bool checked);
	void on_clearForceBtn_clicked();
	void on_recordForceBtn_clicked();
	void on_plotTimer();					//�����ƶ�ʱ��
};
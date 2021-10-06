#ifndef CURVEPLOT_H
#define CURVEPLOT_H
#include "qcustomplot.h"
#include <QAxObject>
#pragma execution_character_set("utf-8")
class CurvePlot :public QObject
{
	Q_OBJECT
public:
	explicit CurvePlot(QCustomPlot * Plot);
	virtual ~CurvePlot();


	void setxAxisName(QString name);
	void setyAxisName(QString name);
	void setxAxisRange(double lower, double upper);
	void setyAxisRange(double lower, double upper);
	void setGraphName(QString name0, QString name1);
	void setSelectLegend(bool enabled);
	void setLegendFont(QFont font);

	/*************曲线数据*************/
	int curIndex;//当前曲线号 = 0, = 1, = -1时未选中曲线
	//0号曲线
	QVector<double> x0;//存储x坐标的向量
	QVector<double> y0;//存储y坐标的向量
	QList<QList<QVariant>> x_y0;

public slots:
	void on_act_tracerToggled(bool arg1);
	void myMouseMoveEvent(QMouseEvent* event);//鼠标事件
	void rescaleAxes();//自动调整
	void clearCurve();//清除曲线
	void clearAllCurves();//清除所有曲线
private:
	/*绘图窗口*/
	QCustomPlot * myPlot = nullptr;

	/*画笔*/
	QVector<QPen> pen;
	/*************游标*************/
	bool tracerEnable;//游标使能
	QCPItemTracer *tracer0 = nullptr; // 0号曲线游标
	QCPItemText *tracer0Label = nullptr; // 0号曲线X游标标签
	void setVisibleTracer(bool trueorfalse);//游标可见/不可见 true/false

	/*********曲线X轴范围**********/
	QCPRange QrangeX0;//0号曲线X轴范围
	double  QrangeX0_lower;
	double  QrangeX0_upper;
	bool foundRange;
};

#endif

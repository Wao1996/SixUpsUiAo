#include "CurvePlot.h"

CurvePlot::CurvePlot(QCustomPlot * Plot)
{
	myPlot = Plot;
	tracerEnable = false;
	/*************绘图模块***************/
	//设置坐标轴字体
	QFont YHfont("Microsoft YaHei", 5, QFont::Normal);
	myPlot->setFont(YHfont);
	myPlot->xAxis->setLabelFont(YHfont);
	myPlot->xAxis->setTickLabelFont(YHfont);
	myPlot->yAxis->setLabelFont(YHfont);
	myPlot->yAxis->setTickLabelFont(YHfont);
	//设定右上角图例标注的字体
	myPlot->legend->setFont(YHfont);
	//设定右上角图例标注可见
	myPlot->legend->setVisible(false);

	//添加图形
	myPlot->addGraph();
	//曲线全部可见
	myPlot->graph(0)->rescaleAxes(true);
	//设置画笔
	QPen pen0(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	pen.append(pen0);
	myPlot->graph(0)->setPen(pen[0]);
	//设置线型
	myPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
	//设置线上点的风格
	myPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
	//右键菜单自定义
	myPlot->setContextMenuPolicy(Qt::CustomContextMenu);
	/**********鼠标操作图像模块************/
	myPlot->selectionRect()->setPen(QPen(Qt::black, 1, Qt::DashLine));//设置选框的样式：虚线
	myPlot->selectionRect()->setBrush(QBrush(QColor(0, 0, 100, 50)));//设置选框的样式：半透明浅蓝
	myPlot->setInteraction(QCP::iRangeDrag, true); //鼠标单击拖动 QCPAxisRect::mousePressEvent() 左键拖动
	myPlot->setInteraction(QCP::iRangeZoom, true); //滚轮滑动缩放
	myPlot->setInteraction(QCP::iSelectAxes, true);
	myPlot->setInteraction(QCP::iSelectLegend, true); //图例可选
	myPlot->selectionRect()->setPen(QPen(Qt::black, 1, Qt::DashLine));
	myPlot->selectionRect()->setBrush(QBrush(QColor(0, 0, 100, 50)));
}

CurvePlot::~CurvePlot()
{
}
//X轴名称
void CurvePlot::setxAxisName(QString name)
{
	myPlot->xAxis->setLabel(name);
}
//Y轴名称
void CurvePlot::setyAxisName(QString name)
{
	myPlot->yAxis->setLabel(name);
}
//X轴范围
void CurvePlot::setxAxisRange(double lower, double upper)
{
	myPlot->xAxis->setRange(lower, upper);
}
//Y轴范围
void CurvePlot::setyAxisRange(double lower, double upper)
{
	myPlot->yAxis->setRange(lower, upper);
}
//曲线图例名称
void CurvePlot::setGraphName(QString name0, QString name1)
{
	//设置右上角图形标注名称
	myPlot->graph(0)->setName(name0);
}
void CurvePlot::setSelectLegend(bool enabled)
{
	if (enabled)
	{
		myPlot->setInteraction(QCP::iSelectLegend, true); //图例可选
	}
	else
	{
		myPlot->setInteraction(QCP::iSelectLegend, false); //图例可选
	}
}
void CurvePlot::setLegendFont(QFont font)
{
	myPlot->legend->setFont(font);
}
void CurvePlot::on_act_tracerToggled(bool arg1)
{
	if (arg1)
	{
		qDebug() << "act_tracer is on!";
		tracerEnable = true;
		tracer0 = new QCPItemTracer(myPlot);
		tracer0->setStyle(QCPItemTracer::tsCrosshair);//游标样式：十字星、圆圈、方框
		tracer0->setPen(QPen(Qt::green));//设置tracer的颜色绿色
		tracer0->setPen(QPen(Qt::DashLine));//虚线游标
		tracer0->setBrush(QBrush(Qt::red));
		tracer0->setSize(10);
		tracer0->setInterpolating(true);//false禁用插值


		tracer0Label = new QCPItemText(myPlot);
		tracer0Label->setClipToAxisRect(false);
		tracer0Label->setLayer("overlay");
		tracer0Label->setPen(QPen(Qt::green));
		tracer0Label->setFont(QFont("Microsoft YaHei", 10));
		tracer0Label->setPadding(QMargins(2, 2, 2, 2));
		tracer0Label->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
		//下面这个语句很重要，它将游标说明锚固在tracer位置处，实现自动跟随
		tracer0Label->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例为单位/实际坐标为单位）
		tracer0Label->position->setParentAnchor(tracer0->position);

	}
	else
	{
		qDebug() << "act_tracer is off !";
		tracerEnable = false;
		setVisibleTracer(false);
	}
}
void CurvePlot::myMouseMoveEvent(QMouseEvent * event)//鼠标移动事件
{
	if (tracerEnable)//游标使能判断
	{
		double x = myPlot->xAxis->pixelToCoord(event->pos().x());//鼠标点的像素坐标转plot坐标

		foundRange = true;
		QrangeX0 = myPlot->graph(0)->getKeyRange(foundRange, QCP::sdBoth);//获取0号曲线X轴坐标范围
		QrangeX0_lower = QrangeX0.lower;
		QrangeX0_upper = QrangeX0.upper;

		//如果鼠标移动超出0号曲线X轴范围，则0号曲线隐藏游标
		if (x < QrangeX0_upper && x > QrangeX0_lower)
		{
			tracer0->setGraph(myPlot->graph(0));//设置游标吸附在traceGraph这条曲线上
			tracer0->setGraphKey(x);//将游标横坐标（key）设置成刚获得的横坐标数据x （这就是游标随动的关键代码）
			tracer0->updatePosition(); //使得刚设置游标的横纵坐标位置生效
			double traceX0 = tracer0->position->key();
			double traceY0 = tracer0->position->value();

			tracer0Label->setText(QString::number(traceX0, 'f', 3));//游标文本框，指示游标的X值
			tracer0Label->setText(QString("x = %1\ny = %2").arg(QString::number(traceX0, 'f', 3)).arg(QString::number(traceY0, 'f', 3)));
			tracer0->setVisible(true);
			tracer0Label->setVisible(true);
		}
		else
		{
			tracer0->setVisible(false);
			tracer0Label->setVisible(false);
		}

		myPlot->replot(QCustomPlot::rpQueuedReplot); //刷新图标，不能省略
	}
}

void CurvePlot::rescaleAxes()//曲线全部显示
{
	//给第一个graph设置rescaleAxes()，后续所有graph都设置rescaleAxes(true)即可实现显示所有曲线。
	//rescaleAxes(true)时如果plot的X或Y轴本来能容纳下本graph的X或Y数据点，
	//那么plot的X或Y轴的可视范围就无需调整，只有plot容纳不下本graph时，才扩展plot两个轴的显示范围。
	//见博客https://www.csdn.net/gather_26/MtTaYg2sMzgxNDgtYmxvZwO0O0OO0O0O.html
	myPlot->graph(0)->rescaleAxes(true);
	myPlot->replot();
}

void CurvePlot::clearCurve()
{
	switch (this->curIndex)
	{
	case 0:
		this->x0.clear();
		this->y0.clear();
		this->x_y0.clear();
		myPlot->graph(0)->setData(this->x0, this->y0);
		break;
	case 1:
		break;
	case -1:
		qDebug() << "未选中曲线！";
		break;
	}
	myPlot->replot(QCustomPlot::rpQueuedReplot);
}

void CurvePlot::clearAllCurves()
{
	this->x0.clear();
	this->y0.clear();
	this->x_y0.clear();
	myPlot->graph(0)->setData(this->x0, this->y0);
	myPlot->replot(QCustomPlot::rpQueuedReplot);
}

void CurvePlot::setVisibleTracer(bool trueorfalse)
{
	tracer0->setVisible(trueorfalse);
	tracer0Label->setVisible(trueorfalse);
	myPlot->replot(QCustomPlot::rpQueuedReplot); //刷新图标，不能省略
}
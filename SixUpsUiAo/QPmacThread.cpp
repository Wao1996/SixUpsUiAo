#include "QPmacThread.h"

QPmacThread::QPmacThread(QObject *parent):QObject(parent)
{
	//初始化Pmac参数
	Pmac0 = new PCOMMSERVERLib::PmacDevice();
	this->pDeviceNumber = 0;
	this->pbSuccess_open = false;
	this->pbSuccess_select = false;
	this->pbSucess_download = false;
	this->pAnswer = "";
	this->bAddLF = true;
	this->pstatus = 0;
	qDebug() << "QPmacThread";
}

QPmacThread::~QPmacThread()
{
	////关闭Pmac  //详细解释见switchPmacThread()函数
	//if (pbSuccess_open)
	//{
	//	Pmac0->Close(pDeviceNumber);
	//	pbSuccess_open = false;
	//	qDebug() << "Pmac0->Close";
	//}
	//delete Pmac0;//在此处delete会导致线程无法退出,见switchPmacThread()函数
	//Pmac0 = nullptr;
	
}

void QPmacThread::startPmac()
{
	qDebug() << "startpmac thread id:" << QThread::currentThreadId();
}

double QPmacThread::onGetMotorDisp()
{
	Pmac0->GetResponse(pDeviceNumber, "M162", pAnswer);//获取位移mm
	double disp = pAnswer.left(pAnswer.length() - 1).toDouble() / 3072 / 8192 * 16;//位移换算
	qDebug() << "Disp = " << disp << "  ,response thread id:" << QThread::currentThreadId();
	return disp;
}

bool QPmacThread::creatPmacSelect()
{
	qDebug() << "creatPmac thread id:" << QThread::currentThreadId();
	pbSuccess_open = false;
	pbSuccess_select = false;
	Pmac0->SelectDevice(NULL, pDeviceNumber, pbSuccess_select);
	Pmac0->Open(pDeviceNumber, pbSuccess_open);
	if (pbSuccess_open)
	{
		qDebug() << "open success";
		QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("连接成功,请初始化Pmac!"));
		return  true;
	}
	else
	{
		qDebug() << "open failed";
		QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("连接失败,您取消了连接或者检查是否以管理员身份运行!"));
		return false;
	}
}

#include "QPmacThread.h"

QPmacThread::QPmacThread(QObject *parent):QObject(parent)
{
	//��ʼ��Pmac����
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
	////�ر�Pmac  //��ϸ���ͼ�switchPmacThread()����
	//if (pbSuccess_open)
	//{
	//	Pmac0->Close(pDeviceNumber);
	//	pbSuccess_open = false;
	//	qDebug() << "Pmac0->Close";
	//}
	//delete Pmac0;//�ڴ˴�delete�ᵼ���߳��޷��˳�,��switchPmacThread()����
	//Pmac0 = nullptr;
	
}

void QPmacThread::startPmac()
{
	qDebug() << "startpmac thread id:" << QThread::currentThreadId();
}

double QPmacThread::onGetMotorDisp()
{
	Pmac0->GetResponse(pDeviceNumber, "M162", pAnswer);//��ȡλ��mm
	double disp = pAnswer.left(pAnswer.length() - 1).toDouble() / 3072 / 8192 * 16;//λ�ƻ���
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
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("���ӳɹ�,���ʼ��Pmac!"));
		return  true;
	}
	else
	{
		qDebug() << "open failed";
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("����ʧ��,��ȡ�������ӻ��߼���Ƿ��Թ���Ա�������!"));
		return false;
	}
}

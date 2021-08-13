#include "QPmac.h"

QReadWriteLock lock;

QPmac::QPmac(QObject *parent):QObject(parent)
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
	qDebug() << "QPmac";
}

QPmac::~QPmac()
{
	//�ر�Pmac  //��ϸ���ͼ�switchPmacThread()����
	if (pbSuccess_open)
	{
		Pmac0->Close(pDeviceNumber);
		pbSuccess_open = false;
		qDebug() << "Pmac0->Close";
	}
	delete Pmac0;//�ڴ˴�delete�ᵼ���߳��޷��˳�,��switchPmacThread()����
	Pmac0 = nullptr;
	
}

bool QPmac::creatPmacSelect()
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

void QPmac::initPmac()
{
	GlobalSta::pmacIsInitialed = true;

}

int QPmac::getNegLimitState(int num)
{
	QString negLimitM = "M" + QString::number(num) + "32";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

int QPmac::getPosLimitState(int num)
{
	QString negLimitM = "M" + QString::number(num) + "31";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

double QPmac::getCurLengths(int num)
{
	QString negLimitM = "M" + QString::number(num) + "62";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	double position = pAnswer.left(pAnswer.length() - 1).toDouble() / 3072 / 40960;
	return position;
}


void QPmac::on_dataGatherTimer()
{
	//qDebug() << "on_dataGatherTimer";

	for (int i = 0; i < PmacData::numL; i++)
	{
		PmacData::negLimitState(i) = getNegLimitState(i + 1);
		PmacData::posLimitState(i) = getPosLimitState(i + 1);
		PmacData::curLengths(i) = getCurLengths(i + 1);
		UPSData::curL_norm = PmacData::curLengths.head(6) + UPSData::initL_norm;
	}
}
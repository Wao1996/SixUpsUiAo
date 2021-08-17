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
	this->pAnswer = "default";
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

bool QPmac::downloadFile(QString strFile)
{
	pbSucess_download = false;
	Pmac0->Download(pDeviceNumber, strFile, true, true, true, true, pbSucess_download);
	if (pbSucess_download)
	{
		qDebug() << "�����ļ�:" << strFile << "�ɹ�";
		//�����������ֲᣬֻ�ǳɹ���ʼ�����̣߳����������سɹ�����鿴��־�ļ�
	}
	return pbSucess_download;
}

int QPmac::getNegLimitState(int num)
{
	QString negLimitM = "M" + QString::number(num) + "32";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

void QPmac::getNegLimitState(VectorXi & negLimitState)
{
	int nums = negLimitState.size();//���ж��ٸ���
	QString strCommand;
	//����ƴ��
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append( "M" + QString::number(i) + "32");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//������ֵ���
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		negLimitState(i) = res.at(i).toInt();
	}
}

int QPmac::getPosLimitState(int num)
{
	QString posLimitM = "M" + QString::number(num) + "31";
	Pmac0->GetResponse(pDeviceNumber, posLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

void QPmac::getPosLimitState(VectorXi & posLimitState)
{
	int nums = posLimitState.size();//���ж��ٸ���
	QString strCommand;
	//����ƴ��
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "31");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//������ֵ���
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		posLimitState(i) = res.at(i).toInt();
	}
}

int QPmac::getHomeCompleteState(int num)
{
	QString homeCompleteM = "M" + QString::number(num) + "45";
	Pmac0->GetResponse(pDeviceNumber, homeCompleteM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

void QPmac::getHomeCompleteState(VectorXi & axleHomeCompleteState)
{
	int nums = axleHomeCompleteState.size();//���ж��ٸ���
	QString strCommand;
	//����ƴ��
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "45");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//������ֵ���
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		axleHomeCompleteState(i) = res.at(i).toInt();
	}
}

double QPmac::getCurLengths(int num)
{
	QString negLimitM = "M" + QString::number(num) + "62";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	double position = pAnswer.left(pAnswer.length() - 1).toDouble() / 3072 / 20480;
	return position;
}

void QPmac::getCurLengths(VectorXd & curLengths)
{
	int nums = curLengths.size();//���ж��ٸ���
	QString strCommand;
	//����ƴ��
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "62");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//������ֵ���
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		curLengths(i) = res.at(i).toDouble() / 3072 / 20480;
	}
}

void QPmac::jogDisp(int num, double disp)
{
	//���������
	int jog_disp_cts = disp * 20480;//λ��תΪ����
	QString strCts = QString::number(jog_disp_cts);
	QString strNum = QString::number(num);
	QString strCommand = "#"+ strNum+"j^" + strCts;
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "jogDisp:" << strCommand << ":" << pAnswer;
}

void QPmac::jogPosContinuously(int num)
{
	QString strNum = QString::number(num);
	QString strCommand = "#" + strNum + "j+";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "jogPosContinuously:"  << strCommand << ":" << pAnswer;
}

void QPmac::jogNegContinuously(int num)
{
	QString strNum = QString::number(num);
	QString strCommand = "#" + strNum + "j-";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "jogNegContinuously:" << strCommand << ":" << pAnswer;
}

void QPmac::jogStop()
{
	QString strCommand =  "j/";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "jogStop:" << strCommand << ":" << pAnswer;
}

void QPmac::jogStop(int num)
{
	QString strNum = QString::number(num);
	QString strCommand = "#"+ strNum+"j/";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "jogStop:" << strCommand << ":" << pAnswer;
}

void QPmac::setJogSpeed(int num, double speed)
{
	QString strNum = QString::number(num);
	QString strVel = QString::number( speed / 1000 * 20480, 'f', 4);//�ٶȲ�������
	QString strCommand = "I" + strNum + "22=" + strVel;
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "setJogSpeed:" << strCommand << ":" << pAnswer;
}

void QPmac::setServoOff()
{
	QString strCommand =  "K";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "setServoOff:" << strCommand << ":" << pAnswer;
}

void QPmac::setAllAxleHome()
{
}

void QPmac::setALLaxleHomez()
{
	QString strCommand = "#1hmz#2hmz#3hmz#4hmz#5hmz#6hmz";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "setALLaxleHomez:" << strCommand << ":" << pAnswer;
}

void QPmac::upsAbsMove(Matrix<double, 6, 1> absL)
{
	QDir temDir("./pmacProgram");
	QString filePath = temDir.absolutePath();
	QString strUpsAbsMove = filePath + "/upsAbsMove.pmc";
	QFile file(strUpsAbsMove);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString Head;
		QString content;
		double vel = 1;//�����ٶ�
		Head.sprintf("A\nOPEN PROG1 CLEAR\nABS\nLINEAR\nFRAX(X,Y,Z,A,B,C)\nF%.4f\n", vel);
		content.sprintf("X%.4f Y%.4f Z%.4f A%.4f B%.4f C%.4f\nCLOSE\n", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
		pmacOutput << Head << content << endl;
		file.close();
		qDebug() << strUpsAbsMove << "write success";
	}
	bool downloadFileState= downloadFile(strUpsAbsMove);
	if (downloadFileState == true)
	{
		//Pmac0->GetResponse(pDeviceNumber, "&1b1r", pAnswer);
	} 
	else
	{
	}
}

void QPmac::upsIncMove(Matrix<double, 6, 1> incL)
{
}


void QPmac::on_dataGatherTimer()
{
	//qDebug() << "on_dataGatherTimer";	
	getNegLimitState(PmacData::negLimitState);
	getPosLimitState(PmacData::posLimitState);
	getCurLengths(PmacData::curLengths);
	getHomeCompleteState(PmacData::axleHomeCompleteState);
	UPSData::curL_norm = PmacData::curLengths.head(6) + UPSData::initL_norm;	
}
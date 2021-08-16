#include "QPmac.h"

QReadWriteLock lock;

QPmac::QPmac(QObject *parent):QObject(parent)
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
	qDebug() << "QPmac";
}

QPmac::~QPmac()
{
	//关闭Pmac  //详细解释见switchPmacThread()函数
	if (pbSuccess_open)
	{
		Pmac0->Close(pDeviceNumber);
		pbSuccess_open = false;
		qDebug() << "Pmac0->Close";
	}
	delete Pmac0;//在此处delete会导致线程无法退出,见switchPmacThread()函数
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
		qDebug() << "下载文件:" << strFile << "成功";
		//经过过查阅手册，只是成功开始现在线程，不代表下载成功，需查看日志文件
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

int QPmac::getPosLimitState(int num)
{
	QString posLimitM = "M" + QString::number(num) + "31";
	Pmac0->GetResponse(pDeviceNumber, posLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

int QPmac::getHomeCompleteState(int num)
{
	QString homeCompleteM = "M" + QString::number(num) + "45";
	Pmac0->GetResponse(pDeviceNumber, homeCompleteM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

double QPmac::getCurLengths(int num)
{
	QString negLimitM = "M" + QString::number(num) + "62";
	Pmac0->GetResponse(pDeviceNumber, negLimitM, pAnswer);
	double position = pAnswer.left(pAnswer.length() - 1).toDouble() / 3072 / 20480;
	return position;
}

void QPmac::jogDisp(int num, double disp)
{
	//换算成脉冲
	int jog_disp_cts = disp * 20480;//位移转为脉冲
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
	QString strVel = QString::number( speed / 1000 * 20480, 'f', 4);//速度参数换算
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
}

void QPmac::upsAbsMove(Matrix<double, 6, 1> absL)
{
	QDir temDir("./pmacProgram");
	QString filePath = temDir.absolutePath();
	QString strUpsAbsMove = filePath + "/upsAbsMove.pmc";
	qDebug() << filePath;
	QFile file(strUpsAbsMove);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString content = "123";
		pmacOutput << content << endl;
		pmacOutput << content << endl;
		//stream << write_txt << "\n";
		file.close();
		qDebug() << " write_txt";
	}
}

void QPmac::upsIncMove(Matrix<double, 6, 1> incL)
{
}


void QPmac::on_dataGatherTimer()
{
	//qDebug() << "on_dataGatherTimer";

	for (int i = 0; i < PmacData::numL; i++)
	{
		PmacData::negLimitState(i) = getNegLimitState(i + 1);
		PmacData::posLimitState(i) = getPosLimitState(i + 1);
		PmacData::curLengths(i) = getCurLengths(i + 1);
		PmacData::axleHomeCompleteState(i) = getHomeCompleteState(i + 1);
		UPSData::curL_norm = PmacData::curLengths.head(6) + UPSData::initL_norm;
	}
	
}
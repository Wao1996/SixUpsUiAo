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
	this->pAnswer = "default";
	this->bAddLF = true;
	this->pstatus = 0;
	qDebug() << "QPmac";

	pmacVariableRecipe.append(NEGLIMITSTATE);
	pmacVariableRecipe.append(POSLIMITSTATE);
	pmacVariableRecipe.append(AXLEHOMECOMPLETESTATE);
	pmacVariableRecipe.append(CURLENGTHS);
	pmacVariableRecipe.append(PVARIABLE);

	pmacVariableList.append(&PmacData::negLimitState);
	pmacVariableList.append(&PmacData::posLimitState);
	pmacVariableList.append(&PmacData::axleHomeCompleteState);
	pmacVariableList.append(&PmacData::curLengths);
	pmacVariableList.append(&PmacData::pVariable);
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
		qDebug() << "downloadFile:" << strFile << "success";
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

void QPmac::getNegLimitState(VectorXd & negLimitState)
{
	int nums = negLimitState.size();//共有多少个轴
	QString strCommand;
	//命令拼接
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append( "M" + QString::number(i) + "32");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		negLimitState(i) = res.at(i).toDouble();
	}
}

int QPmac::getPosLimitState(int num)
{
	QString posLimitM = "M" + QString::number(num) + "31";
	Pmac0->GetResponse(pDeviceNumber, posLimitM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

void QPmac::getPosLimitState(VectorXd & posLimitState)
{
	int nums = posLimitState.size();//共有多少个轴
	QString strCommand;
	//命令拼接
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "31");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		posLimitState(i) = res.at(i).toDouble();
	}
}

int QPmac::getHomeCompleteState(int num)
{
	QString homeCompleteM = "M" + QString::number(num) + "45";
	Pmac0->GetResponse(pDeviceNumber, homeCompleteM, pAnswer);
	int flag = pAnswer.left(pAnswer.length() - 1).toInt();
	return flag;
}

void QPmac::getHomeCompleteState(VectorXd & axleHomeCompleteState)
{
	int nums = axleHomeCompleteState.size();//共有多少个轴
	QString strCommand;
	//命令拼接
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "45");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		axleHomeCompleteState(i) = res.at(i).toDouble();
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
	int nums = curLengths.size();//共有多少个轴
	QString strCommand;
	//命令拼接
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("M" + QString::number(i) + "62");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		curLengths(i) = res.at(i).toDouble() / 3072 / 20480;
	}
}

void QPmac::getMotorsState(QList<PmacVariable>& pmacVariableRecipe, QList<VectorXd*>  pmacVariableList)
{
	/*分析需要采集哪些数据 并按顺序生成字符串*/
	int numRecipe = pmacVariableRecipe.size();
	int numTemp = 0;
	QString strCommand;
	for (int i = 0; i < numRecipe; i++)
	{
		switch (pmacVariableRecipe.at(i))
		{
		case NEGLIMITSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("M" + QString::number(j) + "32");
			}
			break;
		case POSLIMITSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("M" + QString::number(j) + "31");
			}
			break;
		case AXLEHOMECOMPLETESTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("M" + QString::number(j) + "45");
			}
			break;
		case CURLENGTHS:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("#" + QString::number(j) + "P");
			}
			break;
		case PVARIABLE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("P" + QString::number(j));
			}
			break;
		default:
			qDebug() << "getMotorsState solve pmacVariableRecipe ERROR!";
			break;
		}
	}
	/*将命令发送给PMAC 解析结果*/
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < numRecipe; i++)
	{
		switch (pmacVariableRecipe.at(i))
		{
		case NEGLIMITSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j)= res.at(0).toDouble();
				res.removeFirst();
			}
			break;
		case POSLIMITSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble();
				res.removeFirst();
			}
			break;
		case AXLEHOMECOMPLETESTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble();
				res.removeFirst();
			}
			break;
		case CURLENGTHS:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble() / 204800;
				res.removeFirst();
			}
			break;
		case PVARIABLE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble();
				res.removeFirst();
			}
			break;
		default:
			qDebug() << "getMotorsState solve pmacVariableRecipe ERROR!";
			break;
		}
	}
}


void QPmac::getPVariableList(VectorXd &pVariable)
{
	int nums = pVariable.size();//共有多少个轴
	QString strCommand;
	//命令拼接
	for (int i = 1; i <= nums; i++)
	{
		strCommand.append("P" + QString::number(i));
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	//将返回值拆解
	QStringList res = pAnswer.left(pAnswer.length() - 1).split("\r");
	for (int i = 0; i < nums; i++)
	{
		pVariable(i) = res.at(i).toDouble();
	}
}

void QPmac::setPvariable(int p, double data)
{
	QString strCommand = "P" + QString::number(p) + "=" + QString::number(data);
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
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
		double vel = 1;//进给速度
		Head.sprintf("A\nOPEN PROG1 CLEAR\nABS\nLINEAR\nFRAX(X,Y,Z,A,B,C)\nF%.4f\n", vel);
		content.sprintf("X%.4f Y%.4f Z%.4f A%.4f B%.4f C%.4f\nCLOSE\n", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
		pmacOutput << Head << content << endl;
		file.close();
		qDebug() << strUpsAbsMove << "writeFile success";
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

void QPmac::upsHomeMove(Matrix<double, 6, 1> absL)
{
	QDir temDir("./pmacProgram");
	QString filePath = temDir.absolutePath();
	QString strUpsHomeMove = filePath + "/upsHomeMove.pmc";
	QFile file(strUpsHomeMove);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString Head;
		QString content;
		char cmdP2Start[] = "CMD\"P2=0\"";
		char cmdP2End[] = "CMD\"P2=1\"";
		double vel = 1;//进给速度
		Head.sprintf("A\nOPEN PROG1 CLEAR\n%s\nABS\nLINEAR\nFRAX(X,Y,Z,A,B,C)\nF%.4f\n", cmdP2Start, vel);
		content.sprintf("X%.4f Y%.4f Z%.4f A%.4f B%.4f C%.4f\nDWELL10\n%s\nCLOSE\n", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5), cmdP2End);
		pmacOutput << Head << content << endl;
		file.close();
		qDebug() << strUpsHomeMove << "writeFile success";
	}
	bool downloadFileState = downloadFile(strUpsHomeMove);
	if (downloadFileState == true)
	{
		//Pmac0->GetResponse(pDeviceNumber, "&1b1r", pAnswer);
	}
	else
	{
	}
}




void QPmac::on_dataGatherTimer()
{
	//qDebug() << "on_dataGatherTimer";	
	/*getNegLimitState(PmacData::negLimitState);
	getPosLimitState(PmacData::posLimitState);
	getCurLengths(PmacData::curLengths);
	getHomeCompleteState(PmacData::axleHomeCompleteState);
	getPVariableList(PmacData::pVariable);*/
	getMotorsState( pmacVariableRecipe,  pmacVariableList);
	UPSData::curL_norm = PmacData::curLengths.head(6) + UPSData::initL_norm;	
}
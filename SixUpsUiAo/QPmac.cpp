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
	pmacVariableRecipe.append(CURLENGTHSMM);
	pmacVariableRecipe.append(PVARIABLE); 
	pmacVariableRecipe.append(ORIGINSTATE);

	pmacVariableList.append(&PmacData::negLimitState);
	pmacVariableList.append(&PmacData::posLimitState);
	pmacVariableList.append(&PmacData::axleHomeCompleteState);
	pmacVariableList.append(&PmacData::curLengthsMM);
	pmacVariableList.append(&PmacData::pVariable);
	pmacVariableList.append(&PmacData::originState);
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
		case CURLENGTHSMM:
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
		case ORIGINSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("M" + QString::number(j)+"20");
			}
			break;
		//需要采集什么数据 在此处添加即可；
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
		case CURLENGTHSMM:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble() / 204800;//204800个脉冲 电动缸走1mm
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
		case ORIGINSTATE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble();
				res.removeFirst();
			}
			break;
		//需要采集什么数据 在此处添加即可；
		default:
			qDebug() << "getMotorsState solve pmacVariableRecipe ERROR!";
			break;
		}
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

void QPmac::setHomeCompleteZero()
{
	QString strCommand;
	for (int i = 1;i<=6;i++)
	{
		strCommand.append("M" + QString::number(i) + "45=0");
	}
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "setHomeCompleteZero:" << strCommand << ":" << pAnswer;
}

void QPmac::axlesHomeMove()
{
	QDir temDir("./pmacProgram");
	QString filePath = temDir.absolutePath();
	QString axlesHomeFile = filePath + "/axlesHome.pmc";
	bool downloadFileState = downloadFile(axlesHomeFile);
	if (downloadFileState == true)
	{
		//Pmac0->GetResponse(pDeviceNumber, "&1b1r&2b2r&3b3r&4b4r&5b5r&6b6r", pAnswer);
	}
	else
	{
		qDebug() << "axlesHomeMove() error!";
	}
}

void QPmac::axlesHomeZMove()
{
	QString strCommand = "#1hmz#2hmz#3hmz#4hmz#5hmz#6hmz";
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "axlesHomeZMove:" << strCommand << ":" << pAnswer;
}

void QPmac::upsAbsMove(Matrix<double, 6, 1> absL,double vel)
{
	QDir temDir("./pmacProgram");
	QString filePath = temDir.absolutePath();
	QString strUpsHomeMove = filePath + "/upsAbsMove.pmc";
	QFile file(strUpsHomeMove);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString Head;
		QString content;
		Head.sprintf("A\n\
					  CLOSE\n\
					  UNDEFINE ALL\n\
					  &1\n\
					  #1->20480X\n\
					  #2->20480Y\n\
					  #3->20480Z\n\
					  #4->20480A\n\
					  #5->20480B\n\
					  #6->20480C\n\
					  OPEN PROG1 CLEAR\n\
					  ABS\n\
					  LINEAR\n\
					  FRAX(X,Y,Z,A,B,C)\n\
					  F%.4f\n", vel);
		content.sprintf("X%.4f Y%.4f Z%.4f A%.4f B%.4f C%.4f\n\
						DWELL10\n\
						CLOSE\n", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
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
		qDebug() << "upsAbsMove() error!";
	}
}


void QPmac::upsHomeMove(Matrix<double, 6, 1> absL,  double vel)
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
		//char cmdP2Start[] = "CMD\"P2=0\"";
		//char cmdP2End[] = "CMD\"P2=1\"";
		Head.sprintf("A\n\
					  CLOSE\n\
					  UNDEFINE ALL\n\
					  &1\n\
					  #1->20480X\n\
					  #2->20480Y\n\
					  #3->20480Z\n\
					  #4->20480A\n\
					  #5->20480B\n\
					  #6->20480C\n\
					  OPEN PROG1 CLEAR\n\
					  P2=0\n\
					  ABS\n\
					  LINEAR\n\
					  FRAX(X,Y,Z,A,B,C)\n\
					  F%.4f\n",vel);
		content.sprintf("X%.4f Y%.4f Z%.4f A%.4f B%.4f C%.4f\n\
						DWELL10\n\
						P2=1\n\
						CLOSE\n", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
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
		qDebug() << "upsHomeMove() error!";
	}
}

void QPmac::upsIncMove(Matrix<double, 6, 1> incL)
{
}




void QPmac::on_dataGatherTimer()
{
	getMotorsState( pmacVariableRecipe,  pmacVariableList);
	UPSData::curL_norm = PmacData::curLengthsMM.head(6) + UPSData::initL_norm;	
}
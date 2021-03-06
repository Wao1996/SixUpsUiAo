#include "QPmac.h"

extern QReadWriteLock rwLock;

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
	pmacVariableRecipe.append(CURFORCE);

	pmacVariableList.append(&PmacData::negLimitState);
	pmacVariableList.append(&PmacData::posLimitState);
	pmacVariableList.append(&PmacData::axleHomeCompleteState);
	pmacVariableList.append(&PmacData::curLengthsMM);
	pmacVariableList.append(&PmacData::pVariable);
	pmacVariableList.append(&PmacData::originState);
	pmacVariableList.append(&PmacData::curForce);
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
		//QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("连接成功,请初始化Pmac!"));
		QMessageBox::information(NULL, "提示", "连接成功,请初始化Pmac!");
		return  true;
	}
	else
	{
		qDebug() << "open failed";
		//QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("连接失败,您取消了连接或者检查是否以管理员身份运行!"));
		QMessageBox::information(NULL, "提示", "连接失败,您取消了连接或者检查是否以管理员身份运行!");
		return false;
	}
}

void QPmac::initPmac()
{
	//创建获取pmac变量在线命令的字符串
	PmacData::pmacGetVariableCommand = creatPmacVariableCommand(pmacVariableRecipe);
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

QString QPmac::creatPmacVariableCommand(QList<PmacVariable> &pmacVariableRecipe)
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
				strCommand.append("M" + QString::number(j) + "20");
			}
			break;
		case CURFORCE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 1; j <= numTemp; j++)
			{
				strCommand.append("M" + QString::number(j) + "05");
			}
			break;
			//需要采集什么数据 在此处添加即可；
		default:
			qDebug() << "getPmacVariable solve pmacVariableRecipe ERROR!";
			break;
		}
	}
	return strCommand;
}

void QPmac::getPmacVariable(QList<PmacVariable>& pmacVariableRecipe, QString strCommand, QList<VectorXd*>  pmacVariableList)
{
	/*分析需要采集哪些数据 并按顺序生成字符串*/
	int numRecipe = pmacVariableRecipe.size();
	int numTemp = 0;
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
				(*pmacVariableList.at(i))(j) = res.at(0).toDouble() / PmacData::cts2mm;//PmacData::cts2mm个脉冲 电动缸走1mm
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
		case CURFORCE:
			numTemp = pmacVariableList.at(i)->size();
			for (int j = 0; j < numTemp; j++)
			{
				if (j==3)
				{
					(*pmacVariableList.at(i))(j) = - res.at(0).toDouble() * 500 / 2048 * 9.80665;
					res.removeFirst();
				} 
				else
				{
					(*pmacVariableList.at(i))(j) = res.at(0).toDouble() * 500 / 2048 * 9.80665;
					res.removeFirst();
				}

			}
			break;
		//需要采集什么数据 在此处添加即可；
		default:
			qDebug() << "getPmacVariable solve pmacVariableRecipe ERROR!";
			break;
		}
	}
}


void QPmac::setPvariable(int p, double data)
{
	QString strCommand = "P" + QString::number(p) + "=" + QString::number(data);
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
}

void QPmac::setCurLengths(const Matrix<double, 6, 1> & lastCurLengthsMM)
{
	
	Matrix<double, 6, 1> lastCurLengthsCts = lastCurLengthsMM * 96 * 32 * PmacData::cts2mm;
	QString strCommand = "#1K#2K#3K#4K#5K#6K";
	for (int i = 0; i < 6; i++)
	{
		strCommand.append("M" + QString::number(i+1) + "62=" + QString::number(lastCurLengthsCts(i), 'f', 4));
	}
	qDebug() << strCommand;
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
}

void QPmac::enablePLC(int num)
{
	QString strCommand = "ENABLE PLC" + QString::number(num);
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
}

void QPmac::disablePLC(int num)
{
	QString strCommand = "DISABLE PLC" + QString::number(num);
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
}
void QPmac::jogDisp(int num, double disp)
{
	//换算成脉冲
	int jog_disp_cts = disp * PmacData::cts2mm;//位移转为脉冲
	QString strCts = QString::number(jog_disp_cts,'f',1);
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

	QString strCommand =  "&1A&2A&3A&4A&5A&6A#1j/#2j/#3j/#4j/#5j/#6j/";
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
	QString strVel = QString::number( speed / 1000 * PmacData::cts2mm, 'f', 4);//速度参数换算
	QString strCommand = "I" + strNum + "22=" + strVel;
	Pmac0->GetResponse(pDeviceNumber, strCommand, pAnswer);
	qDebug() << "setJogSpeed:" << strCommand << ":" << pAnswer;
}

void QPmac::setServoOff()
{
	QString strCommand =  "#1K#2K#3K#4K#5K#6K";
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
	//QDir temDir("./pmacProgram");
	//QString filePath = temDir.absolutePath();
	QString axlesHomeFile = GlobalSta::dataFile + "/pmacProgram/axlesHome.pmc";
	bool downloadFileState = downloadFile(axlesHomeFile);
	if (downloadFileState == true)
	{
		Pmac0->GetResponse(pDeviceNumber, "&1b1r&2b2r&3b3r&4b4r&5b5r&6b6r", pAnswer);
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
	//QDir temDir("./pmacProgram");
	//QString filePath = temDir.absolutePath();
	QString absHomeMoveFile = GlobalSta::dataFile + "/pmacProgram/upsAbsMove.pmc";
	QFile file(absHomeMoveFile);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString Head;
		QString content;
		Head.sprintf("A\
					  \nI124=$100001\
				      \nI224=$100001\
				      \nI324=$100001\
				      \nI424=$100001\
				      \nI524=$100001\
					  \nI624=$100001\
					  \nCLOSE\
					  \nUNDEFINE ALL\
					  \n&1\
					  \n#1->20480X\
					  \n#2->20480Y\
					  \n#3->20480Z\
					  \n#4->20480U\
					  \n#5->20480V\
					  \n#6->20480W\
					  \nOPEN PROG1 CLEAR\
					  \nABS\
					  \nLINEAR\
					  \nFRAX(X,Y,Z,U,V,W)\
					  \nF%.4f", vel);
		content.sprintf("\nX%.4f Y%.4f Z%.4f U%.4f V%.4f W%.4f\
						\nCLOSE", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
		pmacOutput << Head << content << endl;
		file.close();
		qDebug() << absHomeMoveFile << "writeFile success";
	}
	bool downloadFileState = downloadFile(absHomeMoveFile);
	if (downloadFileState == true)
	{
		Pmac0->GetResponse(pDeviceNumber, "&1b1r", pAnswer);
	}
	else
	{
		qDebug() << "upsAbsMove() error!";
	}
}


void QPmac::upsHomeMove(Matrix<double, 6, 1> absL,  double vel)
{
	//QDir temDir("./pmacProgram");
	//QString filePath = temDir.absolutePath();
	QString upsHomeMoveFile = GlobalSta::dataFile + "/pmacProgram/upsHomeMove.pmc";
	QFile file(upsHomeMoveFile);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream pmacOutput(&file);
		QString Head;
		QString content;
		//char cmdP2Start[] = "CMD\"P2=0\"";
		//char cmdP2End[] = "CMD\"P2=1\"";
		Head.sprintf("A\
					  \nI124=$100001\
				      \nI224=$100001\
				      \nI324=$100001\
				      \nI424=$100001\
				      \nI524=$100001\
					  \nI624=$100001\
					  \nCLOSE\
					  \nUNDEFINE ALL\
					  \n&1\
					  \n#1->20480X\
					  \n#2->20480Y\
					  \n#3->20480Z\
					  \n#4->20480U\
					  \n#5->20480V\
					  \n#6->20480W\
					  \nOPEN PROG1 CLEAR\
					  \nP2=0\
					  \nABS\
					  \nLINEAR\
					  \nFRAX(X,Y,Z,U,V,W)\
					  \nF%.4f",vel);
		content.sprintf("\nX%.4f Y%.4f Z%.4f U%.4f V%.4f W%.4f\
						\nDWELL10\
						\nP2=1\
						\nCLOSE", absL(0), absL(1), absL(2), absL(3), absL(4), absL(5));
		pmacOutput << Head << content << endl;
		file.close();
		qDebug() << upsHomeMoveFile << "writeFile success";
	}
	bool downloadFileState = downloadFile(upsHomeMoveFile);
	if (downloadFileState == true)
	{
		Pmac0->GetResponse(pDeviceNumber, "&1b1r", pAnswer);
	}
	else
	{
		qDebug() << "upsHomeMove() error!";
	}
}

void QPmac::upsJogJMove(Matrix<double, 6, 1> tarAxlesL_norm, Matrix<double, 6, 1> speed)
{
	Matrix<double, 6, 1> tarLengthCTS;
	tarLengthCTS = tarAxlesL_norm * PmacData::cts2mm;
	QString strCommend;
	for (int i = 1; i <= 6; i++)
	{
		strCommend.append("I" + QString::number(i) + "22=" + QString::number((speed(i - 1)), 'f', 4));
	}
	for (int i = 1; i <= 6; i++)
	{
		strCommend.append("#" + QString::number(i) + "J=" + QString::number((tarLengthCTS(i - 1)), 'f', 2));
	}
	//qDebug() << strCommend;
	Pmac0->GetResponse(pDeviceNumber, strCommend, pAnswer);
}



void QPmac::on_dataGatherTimer()
{

	getPmacVariable(pmacVariableRecipe, PmacData::pmacGetVariableCommand, pmacVariableList);
	for (int i = 0;i < PmacData::numL;i++)
	{
		if (PmacData::curLengthsMM(i) > -0.0005 && PmacData::curLengthsMM(i) < 0.0005 )
		{
			PmacData::curLengthsMM(i) = 0;
		}
	}
	UPSData::curL_norm = PmacData::curLengthsMM.head(6) + UPSData::initL_norm;	

}
#pragma once
//#pragma execution_character_set("utf-8")
#include <QtCore>
#include <QMessageBox>
#include "PmacDeviceLib.h"
class QPmacThread :public QObject
{
	Q_OBJECT
public:
	QPmacThread(QObject *parent = Q_NULLPTR);
	virtual ~QPmacThread();

	void startPmac();
public slots:
	bool creatPmacSelect();
	double onGetMotorDisp();
public:
	PCOMMSERVERLib::PmacDevice *Pmac0;
	int pDeviceNumber;
private:
	
	
	bool pbSuccess_select;
	bool pbSuccess_open;
	bool pbSucess_download;
	QString pAnswer;
	bool bAddLF;
	int pstatus;
};


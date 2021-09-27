#pragma once

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTcpServer>
#include <QTimer>
#include "typeCast.h"
#include "Global.h"
class HMIcommunication : public QObject
{
	Q_OBJECT

public:
	HMIcommunication(QObject *parent = Q_NULLPTR);
	~HMIcommunication();
	QTcpSocket* m_HMIsocket = nullptr;  
	QTcpServer *m_HMIserver = nullptr;
public:
	void sendInformation(QByteArray);
public slots:
	void startHMI();
	void on_newconnected();
	void on_disconnected();
	void readInformation();
	void stopHMI();
signals:
	void wirteRegisters(const int & address, const double & data);
	void writeCoils(const int & address, const bool & flag);
};

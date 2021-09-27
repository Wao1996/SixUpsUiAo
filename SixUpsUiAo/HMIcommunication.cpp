#include "HMIcommunication.h"

HMIcommunication::HMIcommunication(QObject *parent)
	: QObject(parent)
{

}

HMIcommunication::~HMIcommunication()
{
	qDebug() << "~HMIcommunication() ";
	delete m_HMIserver;
}

void HMIcommunication::startHMI()
{
	m_HMIserver = new QTcpServer(this);
	QString HMIip = "192.6.94.1";
	quint16 HMIport = 502;
	if (!m_HMIserver->listen(QHostAddress(HMIip), HMIport)) {
		qDebug() << "m_HMIserver ERROR";
		return;
	}
	connect(m_HMIserver, &QTcpServer::newConnection, this, &HMIcommunication::on_newconnected);
}

void HMIcommunication::on_newconnected()
{
	if (m_HMIserver->hasPendingConnections()) {//如果服务端有一个待处理的连接，就返回真，否则返回假
		m_HMIsocket = m_HMIserver->nextPendingConnection();//将下一个挂起的连接作为已连接的qtcsocket对象返回。
		if (!m_HMIsocket->isValid()) {//套接字是否有效
			qDebug() << "m_HMIsocket->isValid() ";
			return;
		}
		connect(m_HMIsocket, &QTcpSocket::readyRead, this, &HMIcommunication::readInformation);
		connect(m_HMIsocket, &QTcpSocket::disconnected, this, &HMIcommunication::on_disconnected);
	}
}

void HMIcommunication::on_disconnected()
{
	qDebug() << "HMI on_disconnected ";
}


void HMIcommunication::sendInformation(QByteArray data)
{
	if (nullptr == m_HMIsocket)
	{
		return;
	}
	//给对方发送数据，使用套接字是tcpSocket
	m_HMIsocket->write(data);
}

/*MBAP为报文头，长度为7字节
事务处理标识 2字节	可以理解为报文的序列号，一般每次通信之后就要加1以区别不同的通信数据报文。
协议标识符	2字节	00 00表示ModbusTCP协议。
长度			2字节	表示接下来的数据长度，单位为字节。
单元标识符	1字节	可以理解为设备地址
*/
/*功能码
0x01	读线圈
0x05	写单个线圈
0x0F	写多个线圈
0x02	读离散量输入
0x04	读输入寄存器
0x03	读保持寄存器
0x06	写单个保持寄存器
0x10	写多个保持寄存器
*/
void HMIcommunication::readInformation()
{

	QByteArray recInfor = m_HMIsocket->readAll();//读取到的信息
	QByteArray sendInfor;//要发送的信息
	QByteArray funCode = recInfor.mid(7, 1);//功能码

	//qDebug() << "readInformation :" << recInfor.toHex();

	//读保持寄存器 功能码03
	if (funCode.toHex() == "03")
	{
		/***********要发送的数据*********/
		float temp1 = UPSData::multiJogTranslationSpeed;
		float temp2 = UPSData::multiJogTranslationStep;
		float temp3 = UPSData::multiJogRotateSpeed;
		float temp4 = UPSData::multiJogRotateStep;
		float temp5 = UPSData::multiJogMoveDirectionID;
		QList<float> floatList;
		floatList.append(temp1);
		floatList.append(temp2);
		floatList.append(temp3);
		floatList.append(temp4);
		floatList.append(temp5);

		//qDebug() << "funCode = 03";
		//原来的报文头
		QByteArray MBAP = recInfor.mid(0, 7);
		//查询的寄存器数量
		int  registersNum = byte2ToInt16(recInfor.right(2), BigEndian);
		/***********要发送的数据*********/
		QByteArray tempB;
		for (int i = 0; i < floatList.size(); i++)
		{
			tempB.append(floatToByte(floatList.at(i), BigEndian));
		}
		/***************MBAP*******************/
		//MBAP中的数据长度 字节数
		qint16 length1 = 3 + registersNum * 2;
		QByteArray returnMBAP = MBAP.replace(4, 2, int16ToByte(length1, BigEndian));
		sendInfor.append(returnMBAP);
		/***********数据体**********/
		//功能码
		sendInfor.append(0x03);
		//要发送的数据 的字节数
		qint8 length2 = registersNum * 2;
		sendInfor.append(int8ToByte(length2, BigEndian));
		//数据
		sendInfor.append(tempB);

		QTimer::singleShot(200, [=]() {
			m_HMIsocket->write(sendInfor);
		});

	}
	//写多个保持寄存器
	else if (funCode.toHex() == "10")
	{
		//qDebug() << "funCode = 10";
		//原来的报文头
		QByteArray MBAP = recInfor.mid(0, 7);
		//写入的寄存器的起始地址
		qint16 address = byte2ToInt16(recInfor.mid(8, 2), BigEndian) + 1;
		//写入的寄存器数量
		int  registersNum = byte2ToInt16(recInfor.mid(10, 2), BigEndian);

		QByteArray data10 = recInfor.right(registersNum * 2);
		double dataD = byte4ToFloat(data10, BigEndian);
		//发送信号
		emit wirteRegisters(address, dataD);
		m_HMIsocket->write(recInfor.mid(0, 12));

		//switch (address)
		//{
		//case 1:
		//	emit wirteRegisters(1, dataD);
		//	//写入成功回复
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//case 3:
		//	emit wirteRegisters(3, dataD);
		//	//写入成功回复
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//case 5:
		//{
		//	emit wirteRegisters(5, dataD);
		//	//写入成功回复
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//}
		//default:
		//	qDebug() << "funCode = 10 error";
		//	break;
		//}
	}
	//0x05	写单个线圈
	else if (funCode.toHex() == "05")
	{
		//写入的寄存器的起始地址
		qint16 address = byte2ToInt16(recInfor.mid(8, 2), BigEndian) + 1;
		QByteArray data05 = recInfor.right(2);
		int addressINT = address;
		if (data05.toHex() == "ff00")
		{
			emit writeCoils(address, true);
		}
		else if (data05.toHex() == "0000")
		{
			emit writeCoils(address, false);
		}
		/*
		switch (address)
		{
			//运动停止
		case 1:
			if (data05.toHex() == "ff00")
			{
				qDebug() << "move stop";
				writeCoils(1, true);
			}
			break;
			//距离点动
		case 2:
			if (data05.toHex() == "ff00")
			{
				qDebug() << "distance move";
				writeCoils(2, true);
			}
			break;
			//正方向长按点动
		case 3:
			if (data05.toHex() == "ff00")
			{
				writeCoils(3, true);
				qDebug() << "+press move start";
			}
			else if (data05.toHex() == "0000")
			{
				writeCoils(3, false);
				qDebug() << "+press move stop";
			}
			break;
			//负方向长按点动
		case 4:
			if (data05.toHex() == "ff00")
			{
				writeCoils(4, true);
				qDebug() << "-press move start";
			}
			else if (data05.toHex() == "0000")
			{
				writeCoils(4, false);
				qDebug() << "-press move stop";
			}
			break;
		default:
			break;
		}
		*/

		//写入成功回复
		m_HMIsocket->write(recInfor);

	}
}

void HMIcommunication::stopHMI()
{
	m_HMIserver->close();
}



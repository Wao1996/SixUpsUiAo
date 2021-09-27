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
	if (m_HMIserver->hasPendingConnections()) {//����������һ������������ӣ��ͷ����棬���򷵻ؼ�
		m_HMIsocket = m_HMIserver->nextPendingConnection();//����һ�������������Ϊ�����ӵ�qtcsocket���󷵻ء�
		if (!m_HMIsocket->isValid()) {//�׽����Ƿ���Ч
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
	//���Է��������ݣ�ʹ���׽�����tcpSocket
	m_HMIsocket->write(data);
}

/*MBAPΪ����ͷ������Ϊ7�ֽ�
�������ʶ 2�ֽ�	�������Ϊ���ĵ����кţ�һ��ÿ��ͨ��֮���Ҫ��1������ͬ��ͨ�����ݱ��ġ�
Э���ʶ��	2�ֽ�	00 00��ʾModbusTCPЭ�顣
����			2�ֽ�	��ʾ�����������ݳ��ȣ���λΪ�ֽڡ�
��Ԫ��ʶ��	1�ֽ�	�������Ϊ�豸��ַ
*/
/*������
0x01	����Ȧ
0x05	д������Ȧ
0x0F	д�����Ȧ
0x02	����ɢ������
0x04	������Ĵ���
0x03	�����ּĴ���
0x06	д�������ּĴ���
0x10	д������ּĴ���
*/
void HMIcommunication::readInformation()
{

	QByteArray recInfor = m_HMIsocket->readAll();//��ȡ������Ϣ
	QByteArray sendInfor;//Ҫ���͵���Ϣ
	QByteArray funCode = recInfor.mid(7, 1);//������

	//qDebug() << "readInformation :" << recInfor.toHex();

	//�����ּĴ��� ������03
	if (funCode.toHex() == "03")
	{
		/***********Ҫ���͵�����*********/
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
		//ԭ���ı���ͷ
		QByteArray MBAP = recInfor.mid(0, 7);
		//��ѯ�ļĴ�������
		int  registersNum = byte2ToInt16(recInfor.right(2), BigEndian);
		/***********Ҫ���͵�����*********/
		QByteArray tempB;
		for (int i = 0; i < floatList.size(); i++)
		{
			tempB.append(floatToByte(floatList.at(i), BigEndian));
		}
		/***************MBAP*******************/
		//MBAP�е����ݳ��� �ֽ���
		qint16 length1 = 3 + registersNum * 2;
		QByteArray returnMBAP = MBAP.replace(4, 2, int16ToByte(length1, BigEndian));
		sendInfor.append(returnMBAP);
		/***********������**********/
		//������
		sendInfor.append(0x03);
		//Ҫ���͵����� ���ֽ���
		qint8 length2 = registersNum * 2;
		sendInfor.append(int8ToByte(length2, BigEndian));
		//����
		sendInfor.append(tempB);

		QTimer::singleShot(200, [=]() {
			m_HMIsocket->write(sendInfor);
		});

	}
	//д������ּĴ���
	else if (funCode.toHex() == "10")
	{
		//qDebug() << "funCode = 10";
		//ԭ���ı���ͷ
		QByteArray MBAP = recInfor.mid(0, 7);
		//д��ļĴ�������ʼ��ַ
		qint16 address = byte2ToInt16(recInfor.mid(8, 2), BigEndian) + 1;
		//д��ļĴ�������
		int  registersNum = byte2ToInt16(recInfor.mid(10, 2), BigEndian);

		QByteArray data10 = recInfor.right(registersNum * 2);
		double dataD = byte4ToFloat(data10, BigEndian);
		//�����ź�
		emit wirteRegisters(address, dataD);
		m_HMIsocket->write(recInfor.mid(0, 12));

		//switch (address)
		//{
		//case 1:
		//	emit wirteRegisters(1, dataD);
		//	//д��ɹ��ظ�
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//case 3:
		//	emit wirteRegisters(3, dataD);
		//	//д��ɹ��ظ�
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//case 5:
		//{
		//	emit wirteRegisters(5, dataD);
		//	//д��ɹ��ظ�
		//	m_HMIsocket->write(recInfor.mid(0, 12));
		//	break;
		//}
		//default:
		//	qDebug() << "funCode = 10 error";
		//	break;
		//}
	}
	//0x05	д������Ȧ
	else if (funCode.toHex() == "05")
	{
		//д��ļĴ�������ʼ��ַ
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
			//�˶�ֹͣ
		case 1:
			if (data05.toHex() == "ff00")
			{
				qDebug() << "move stop";
				writeCoils(1, true);
			}
			break;
			//����㶯
		case 2:
			if (data05.toHex() == "ff00")
			{
				qDebug() << "distance move";
				writeCoils(2, true);
			}
			break;
			//�����򳤰��㶯
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
			//�����򳤰��㶯
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

		//д��ɹ��ظ�
		m_HMIsocket->write(recInfor);

	}
}

void HMIcommunication::stopHMI()
{
	m_HMIserver->close();
}



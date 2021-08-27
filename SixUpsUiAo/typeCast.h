#pragma once
#pragma execution_character_set("utf-8")
#include <QtCore>
#include <QTableWidget>
#include <QByteArray>
#include <QList>
#include <Eigen\Dense>
#include "Global.h"

using namespace Eigen;

enum Endian
{
	LittileEndian,
	BigEndian
};

QByteArray int8ToByte(qint8, Endian);
QByteArray int16ToByte(qint16, Endian);
QByteArray int32ToByte(qint32, Endian);
QByteArray int64ToByte(qint64, Endian);

qint8 byte1ToInt8(QByteArray , Endian);
qint16 byte2ToInt16(QByteArray , Endian);
qint32 byte4ToInt32(QByteArray , Endian);
qint64 byte8ToInt64(QByteArray , Endian);

quint8 byte1ToUint8(QByteArray, Endian);
quint16 byte2ToUint16(QByteArray, Endian);
quint32 byte4ToUint32(QByteArray, Endian);
quint64 byte8ToUint64(QByteArray, Endian);

double byte8ToDouble(QByteArray, Endian);
QByteArray doubleToByte(double, Endian);
QByteArray doubleListToByte(QList<double>, Endian);

QList<double> vectorXd2QList(VectorXd);

//�������е����ݵ�MatrixXd��,��������ݵ�����ţ����ڰб������Ķ�Ӧ��ϵ��
bool tableToMatrixXd(const QTableWidget* tab, MatrixXd &mat, vector<int> &dataIndex);
//�������е����ݵ�Matrix<double,3,6>��
bool tableToMatrixXd(const QTableWidget* tab, Matrix<double, 3, 6>& mat);
//MatrixXd����д�����
bool matrixXdToTable(const MatrixXd &mat, QTableWidget* tab);
//MatrixXd����д��csv�ļ�
bool matrixXdToCsv(const MatrixXd &mat, const QString &filePath);
//��ȡcsv�ļ������
bool csvToTable(const QString &filePath, QTableWidget *tab);
//��ȡcsv�ļ���MatrixXd
bool csvToMatrixXd(const QString &filePath, Matrix<double, 6, 1> &mat);
bool csvToMatrixXd(const QString &filePath, Matrix<double, 3, 6> &mat);
bool csvToMatrixXd(const QString &filePath, MatrixXd &mat);
//���д��csv�ļ�
bool tableToCsv(const QTableWidget *tab, const QString &filePath);


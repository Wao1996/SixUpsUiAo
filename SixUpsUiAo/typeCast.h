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

double byte4ToFloat(QByteArray, Endian);
QByteArray floatToByte(float, Endian);

double byte8ToDouble(QByteArray, Endian);
QByteArray doubleToByte(double, Endian);
QByteArray doubleListToByte(QList<double>, Endian);

QList<double> vectorXd2QList(VectorXd);

//遍历表中的数据到MatrixXd中,输出有数据的列序号（用于靶标点坐标的对应关系）
bool tableToMatrixXd(const QTableWidget* tab, MatrixXd &mat, vector<int> &dataIndex);
//遍历表中的数据到Matrix<double,3,6>中
bool tableToMatrixXd(const QTableWidget* tab, Matrix<double, 3, 6>& mat);
//MatrixXd数据写入表中
bool matrixXdToTable(const MatrixXd &mat, QTableWidget* tab);
bool matrixXdToTable(const MatrixXd &mat, QTableWidget* tab, const QString colTitle);
//MatrixXd数据写入csv文件
bool matrixXdToCsv(const MatrixXd &mat, const QString &filePath);
//读取csv文件到表格
bool csvToTable(const QString &filePath, QTableWidget *tab);
bool csvToTableAdapt(const QString &filePath, QTableWidget *tab);//根据csv大小调整table大小
bool csvToTable(const QString &filePath, QTableWidget *tab, const QString colTitle);
//读取csv文件到MatrixXd
bool csvToMatrixXd(const QString &filePath, Matrix<double, 6, 1> &mat);
bool csvToMatrixXd(const QString &filePath, Matrix<double, 3, 6> &mat);
bool csvToMatrixXd(const QString &filePath, MatrixXd &mat);
//表格写入csv文件
bool tableToCsv(const QTableWidget *tab, const QString &filePath);

//表格写入csv文件
bool qlistQvectorToCsv( QList<QVector<double>> qlist, const QString &filePath);


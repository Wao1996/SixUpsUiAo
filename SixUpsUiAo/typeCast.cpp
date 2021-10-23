#include "typeCast.h"


QByteArray int8ToByte(qint8 i, Endian endian = BigEndian)
{
	QByteArray result;
	result.resize(1);
	result[0] = (uchar)(0x000000ff & i);
	return result;
}

QByteArray int16ToByte(qint16 i,Endian endian = BigEndian)
{

	QByteArray result;
	result.resize(2);
	// 小端模式
	if (endian == LittileEndian)
	{
		result[0] = (uchar)(0x00ff & i);
		result[1] = (uchar)((0xff00 & i) >> 8);
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		result[0] = (uchar)((0xff00 & i) >> 8);
		result[1] = (uchar)(0x00ff & i);
	}
	return result;
}

QByteArray int32ToByte(qint32 i, Endian endian = BigEndian)
{
	QByteArray result;
	result.resize(4);
	// 小端模式
	if (endian == LittileEndian)
	{
		result[0] = (uchar)(0x000000ff & i);
		result[1] = (uchar)((0x0000ff00 & i) >> 8);
		result[2] = (uchar)((0x00ff0000 & i) >> 16);
		result[3] = (uchar)((0xff000000 & i) >> 24);
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		result[0] = (uchar)((0xff000000 & i) >> 24);
		result[1] = (uchar)((0x00ff0000 & i) >> 16);
		result[2] = (uchar)((0x0000ff00 & i) >> 8);
		result[3] = (uchar)(0x000000ff & i);
	}
	return result;

}

QByteArray int64ToByte(qint64 i, Endian endian = BigEndian)
{
	QByteArray result;
	result.resize(8);
	// 小端模式
	if (endian == LittileEndian)
	{
		result[0] = (uchar) (0x00000000000000FF & i);
		result[1] = (uchar)((0x000000000000FF00 & i) >> 8);
		result[2] = (uchar)((0x0000000000FF0000 & i) >> 16);
		result[3] = (uchar)((0x00000000FF000000 & i) >> 24);
		result[4] = (uchar)((0x000000FF00000000 & i) >> 32);
		result[5] = (uchar)((0x0000FF0000000000 & i) >> 40);
		result[6] = (uchar)((0x00FF000000000000 & i) >> 48);
		result[7] = (uchar)((0xFF00000000000000 & i) >> 56);
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		result[0] = (uchar)((0xFF00000000000000 & i) >> 56);
		result[1] = (uchar)((0x00FF000000000000 & i) >> 48);
		result[2] = (uchar)((0x0000FF0000000000 & i) >> 40);
		result[3] = (uchar)((0x000000FF00000000 & i) >> 32);
		result[4] = (uchar)((0x00000000FF000000 & i) >> 24);
		result[5] = (uchar)((0x0000000000FF0000 & i) >> 16);
		result[6] = (uchar)((0x000000000000FF00 & i) >> 8);
		result[7] = (uchar)(0x00000000000000FF & i);
	}
	return result;
}

qint8 byte1ToInt8(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 1)
	{
		return 0;
	}

	qint8 res = 0;
	res = arr.at(0) & 0xFF;
	return res;
}

qint16 byte2ToInt16(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 2)
	{
		return 0;
	}
	qint16 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res = arr.at(0) & 0x00FF;
		res |= (arr.at(1) << 8) & 0xFF00;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res = (arr.at(0) << 8) & 0xFF00;
		res |= arr.at(1)  & 0x00FF;
	}
	return res;
}

qint32 byte4ToInt32(QByteArray arr , Endian endian = BigEndian)
{
	if (arr.size() != 4)
	{
		return 0;
	}
	qint32 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res = arr.at(0) & 0x000000FF;
		res |= (arr.at(1) << 8) & 0x0000FF00;
		res |= (arr.at(2) << 16) & 0x00FF0000;
		res |= (arr.at(3) << 24) & 0xFF000000;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res = (arr.at(0) << 24) & 0xFF000000;
		res |= (arr.at(1) << 16) & 0x00FF0000;
		res |= arr.at(2) << 8 & 0x0000FF00;
		res |= arr.at(3) & 0x000000FF;
	}
	return res;
}

qint64 byte8ToInt64(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size()!=8)
	{
		return 0;
	}

	qint64 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res =   arr.at(0)        & 0x00000000000000FF;
		res |= (arr.at(1) << 8)  & 0x000000000000FF00;
		res |= (arr.at(2) << 16) & 0x0000000000FF0000;
		res |= (arr.at(3) << 24) & 0x00000000FF000000;
		res |= (arr.at(4) << 32) & 0x000000FF00000000;
		res |= (arr.at(5) << 40) & 0x0000FF0000000000;
		res |= (arr.at(6) << 48) & 0x00FF000000000000;
		res |= (arr.at(7) << 56) & 0xFF00000000000000;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res =  (arr.at(0) << 56) & 0xFF00000000000000;
		res |= (arr.at(1) << 48) & 0x00FF000000000000;
		res |= (arr.at(2) << 40) & 0x0000FF0000000000;
		res |= (arr.at(3) << 32) & 0x000000FF00000000;
		res |= (arr.at(4) << 24) & 0x00000000FF000000;
		res |= (arr.at(5) << 16) & 0x0000000000FF0000;
		res |= (arr.at(6) << 8)  & 0x000000000000FF00;
		res |= arr.at(7)         & 0x00000000000000FF;
	}
	return res;
}

quint8 byte1ToUint8(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 1)
	{
		return 0;
	}
	char * temp =arr.data();
	quint8 res = *((quint8*)temp);
	return res;
}

quint16 byte2ToUint16(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 2)
	{
		return 0;
	}
	quint16 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res = arr.at(0) & 0x00FF;
		res |= (arr.at(1) << 8) & 0xFF00;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res = (arr.at(0) << 8) & 0xFF00;
		res |= arr.at(1) & 0x00FF;
	}
	return res;

}

quint32 byte4ToUint32(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 4)
	{
		return 0;
	}
	quint32 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res = arr.at(0) & 0x000000FF;
		res |= (arr.at(1) << 8) & 0x0000FF00;
		res |= (arr.at(2) << 16) & 0x00FF0000;
		res |= (arr.at(3) << 24) & 0xFF000000;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res = (arr.at(0) << 24) & 0xFF000000;
		res |= (arr.at(1) << 16) & 0x00FF0000;
		res |= arr.at(2) << 8 & 0x0000FF00;
		res |= arr.at(3) & 0x000000FF;
	}
	return res;
}

quint64 byte8ToUint64(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 8)
	{
		return 0;
	}

	quint64 res = 0;

	// 小端模式
	if (endian == LittileEndian)
	{
		res = arr.at(0) & 0x00000000000000FF;
		res |= (arr.at(1) << 8) & 0x000000000000FF00;
		res |= (arr.at(2) << 16) & 0x0000000000FF0000;
		res |= (arr.at(3) << 24) & 0x00000000FF000000;
		res |= (arr.at(4) << 32) & 0x000000FF00000000;
		res |= (arr.at(5) << 40) & 0x0000FF0000000000;
		res |= (arr.at(6) << 48) & 0x00FF000000000000;
		res |= (arr.at(7) << 56) & 0xFF00000000000000;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		res = (arr.at(0) << 56) & 0xFF00000000000000;
		res |= (arr.at(1) << 48) & 0x00FF000000000000;
		res |= (arr.at(2) << 40) & 0x0000FF0000000000;
		res |= (arr.at(3) << 32) & 0x000000FF00000000;
		res |= (arr.at(4) << 24) & 0x00000000FF000000;
		res |= (arr.at(5) << 16) & 0x0000000000FF0000;
		res |= (arr.at(6) << 8) & 0x000000000000FF00;
		res |= arr.at(7) & 0x00000000000000FF;
	}
	return res;
}


double byte4ToFloat(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 4)
	{
		return 0.0f;
	}
	float res = 0.0f;

	// 小端模式
	if (endian == LittileEndian)
	{
		memcpy(&res, arr.data(), sizeof(res));
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		QByteArray arrInver;
		for (int i = arr.size() - 1; i > -1; i--)
		{
			arrInver.append(arr.at(i));
		}
		memcpy(&res, arrInver.data(), sizeof(res));
	}
	return res;
}

QByteArray floatToByte(float dataF, Endian endian = BigEndian)
{
	QByteArray res;
	res.resize(4);

	// 小端模式
	if (endian == LittileEndian)
	{
		unsigned char *hex = (unsigned char *)&dataF;
		res = QByteArray((char*)hex, 4);
		return res;
	}

	// 大端模式
	else if (endian == BigEndian)
	{
		QByteArray arrInver;
		unsigned char *hex = (unsigned char *)&dataF;
		res = QByteArray((char*)hex, 4);
		for (int i = res.size() - 1; i > -1; i--)
		{
			arrInver.append(res.at(i));
		}
		return arrInver;
	}

	return res;
}


double byte8ToDouble(QByteArray arr, Endian endian = BigEndian)
{
	if (arr.size() != 8)
	{
		return 0.0;
	}
	double res = 0.0;

	// 小端模式
	if (endian == LittileEndian)
	{
		memcpy(&res, arr.data(), sizeof(res));
	}

	// 大端模式
	else 
	{
		QByteArray arrInver;
		for (int i = arr.size() - 1; i > -1; i--)
		{
			arrInver.append(arr.at(i));
		}
		memcpy(&res, arrInver.data(), sizeof(res));
	}
}

QByteArray doubleToByte(double i, Endian endian = BigEndian)
{
	QByteArray res;
	res.resize(8);

	// 小端模式
	if (endian == LittileEndian)
	{
		memcpy(res.data(), &i, sizeof(res));
		return res;
	}

	// 大端模式
	else
	{
		QByteArray arrInver;
		memcpy(res.data(), &i, sizeof(res));
		for (int i = res.size() - 1; i > -1; i--)
		{
			arrInver.append(res.at(i));
		}
		return arrInver;
	}
	
}

QByteArray doubleListToByte(QList<double> list, Endian endian = BigEndian)
{
	QByteArray res;

	// 小端模式
	if (endian == LittileEndian)
	{
		for (int i = 0; i < list.size(); i++)
		{
			double temp = list.at(i);
			res.append(doubleToByte(temp, LittileEndian));
		}
		return res;
	}

	// 大端模式
	else 
	{
		for (int i = 0; i < list.size(); i++)
		{
			double temp = list.at(i);
			res.append(doubleToByte(temp, BigEndian));
		}
		return res;
	}
}

QList<double> vectorXd2QList(VectorXd vec)
{
	int num = vec.size();
	QList<double> result;
	for (int i = 0; i < num; i++)
	{
		result.append(vec(i));
	}
	return result;
}


bool tableToMatrixXd(const QTableWidget * tab, MatrixXd & mat, vector<int>& dataIndex)
{
	int rows = tab->rowCount();
	int cols = tab->columnCount();

	for (int j = 0; j < cols; j++)//统计有数据的列
	{
		if (tab->item(0, j) != nullptr)
		{
			if (!tab->item(0, j)->text().isEmpty())
			{
				dataIndex.push_back(j);
			}
		}
	}
	int dataColCount = dataIndex.size();
	//qDebug() << dataColCount;
	if (dataIndex.size() == 0)
	{
		qDebug() << tab->objectName() << "无数据!";
		return false;
	}
	mat = MatrixXd::Zero(rows, dataColCount);//重新初始化动态mat
	for (int i = 0; i < rows; i++)//遍历数据录入mat
	{
		for (int j = 0; j < dataColCount; j++)
		{
			if (tab->item(i, dataIndex[j]) != nullptr)
			{
				if (!tab->item(i, dataIndex[j])->text().isEmpty())
				{
					mat(i, j) = tab->item(i, dataIndex[j])->text().toDouble();
				}
			}
		}
	}
	cout << tab->objectName().toStdString() << endl;
	cout << mat << endl;
	return true;
}

bool tableToMatrixXd(const QTableWidget * tab, Matrix<double, 3, 6> &mat)
{
	int rows = tab->rowCount();
	int cols = tab->columnCount();
	int dataRows = 0;//有数据的行数
	int dataCols = 0;
	for (int i = 0; i < rows; i++)//统计有数据的行列
	{
		for (int j = 0; j < cols; j++)
		{
			if (tab->item(i, j) != nullptr)
			{
				if (!tab->item(i, j)->text().isEmpty())
				{
					dataRows = i + 1;
					dataCols = j + 1;
				}
			}
		}
	}
	if (dataRows != 3 || dataCols != 6)
	{
		qDebug() << "数据行列数不匹配!!!";
		return false;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				if (tab->item(i, j) != nullptr)
				{
					if (!tab->item(i, j)->text().isEmpty())
					{
						mat(i, j) = tab->item(i, j)->text().toDouble();
					}
				}
			}
		}
	}
	return true;
	//cout << tab->objectName().toStdString() << endl;
	//cout << mat << endl;
}

bool tableToMatrixXd(const QTableWidget * tab, Matrix4d & mat)
{
	int rows = tab->rowCount();
	int cols = tab->columnCount();
	int dataRows = 0;//有数据的行数
	int dataCols = 0;
	for (int i = 0; i < rows; i++)//统计有数据的行列
	{
		for (int j = 0; j < cols; j++)
		{
			if (tab->item(i, j) != nullptr)
			{
				if (!tab->item(i, j)->text().isEmpty())
				{
					dataRows = i + 1;
					dataCols = j + 1;
				}
			}
		}
	}
	if (dataRows != 4 || dataCols != 4)
	{
		qDebug() << "数据行列数不匹配!!!";
		return false;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (tab->item(i, j) != nullptr)
				{
					if (!tab->item(i, j)->text().isEmpty())
					{
						mat(i, j) = tab->item(i, j)->text().toDouble();
					}
				}
			}
		}
	}
	return true;
}

bool matrixXdToTable(const MatrixXd & mat, QTableWidget * tab)
{
	int matRows = mat.rows();
	int matCols = mat.cols();
	int tabRows = tab->rowCount();
	int tabCols = tab->columnCount();
	if (matRows > tabRows)
	{
		qDebug() << "数据行数超限!!";
		return false;
	}
	//若csv列数大于当前表格列数则扩展表格列数
	if (matCols > tabCols)
	{
		qDebug() << "自动扩展列数->" << matCols;
		for (int i = 0; i < matCols - tabCols; i++)
		{
			tab->insertColumn(i + tabCols);
			for (int j = 0; j < matRows; j++)
			{
				tab->setItem(j, i + tabCols, new QTableWidgetItem());//添加新元素
			}
		}
	}
	for (int i = 0; i < matRows; i++)
	{
		for (int j = 0; j < matCols; j++)
		{
			tab->setItem(i, j, new QTableWidgetItem(QString::number(mat(i, j), 'f', 4)));
		}
	}
	return true;
}

bool matrixXdToTable(const MatrixXd & mat, QTableWidget * tab, const QString colTitle)
{
	int matRows = mat.rows();
	int matCols = mat.cols();
	int tabRows = tab->rowCount();
	int tabCols = tab->columnCount();
	if (matRows > tabRows)
	{
		qDebug() << "数据行数超限!!";
		return false;
	}
	//若csv列数大于当前表格列数则扩展表格列数
	if (matCols > tabCols)
	{
		qDebug() << "自动扩展列数->" << matCols;
		for (int i = 0; i < matCols - tabCols; i++)
		{
			tab->insertColumn(i + tabCols);
			QTableWidgetItem * tmpItem = new QTableWidgetItem();
			tab->setHorizontalHeaderItem(i + tabCols, tmpItem);//设置列标题
			QTableWidgetItem * colItem = tab->horizontalHeaderItem(i + tabCols);
			colItem->setText(colTitle + QString::number(i + tabCols + 1));
			for (int j = 0; j < matRows; j++)
			{
				tab->setItem(j, i + tabCols, new QTableWidgetItem());//添加新元素
			}
		}
	}
	else if (matCols < tabCols)
	{
		qDebug() << "自动删除列数->" << matCols;
		for (int i = 0; i < tabCols - matCols; i++)
		{
			tab->removeColumn(i + matCols);
		}

	}
	for (int i = 0; i < matRows; i++)
	{
		for (int j = 0; j < matCols; j++)
		{
			tab->setItem(i, j, new QTableWidgetItem(QString::number(mat(i, j), 'f', 4)));
		}
	}
	return true;
}


bool matrixXdToCsv(const MatrixXd & mat, const QString & filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) { return false; }
	QTextStream stream(&file);
	QString conTents;
	for (int i = 0; i < mat.rows(); i++)
	{
		for (int j = 0; j < mat.cols(); j++)
		{
			QString str = QString::number(mat(i,j),'f',6);
			str.replace(",", " ");
			conTents += str + ",";
		}
		conTents = conTents.left(conTents.length() - 1);
		conTents += "\n";
	}
	stream << conTents;
	file.close();
	return true;
}




bool csvToTable(const QString &filePath, QTableWidget *tab)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	//cout << "csvRowCount:" << csvRowCount << endl;
	//cout << "csvColCount:" << csvColCount << endl;
	structParaFile.close();
	//获取table行列数
	int tabRow = tab->rowCount();
	int tabCol = tab->columnCount();
	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	if (csvRowCount > tabRow)
	{
		qDebug() << "数据行数超限!!";
		return false;
	}
	//若csv列数大于当前表格列数则扩展表格列数
	if (csvColCount > tabCol)
	{
		qDebug() << "自动扩展列数->" << csvColCount;
		for (int i = 0; i < csvColCount - tabCol; i++)
		{
			tab->insertColumn(i + tabCol);
			for (int j = 0; j < csvRowCount; j++)
			{
				tab->setItem(j, i + tabCol, new QTableWidgetItem());//添加新元素
			}
		}
	}
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			tab->setItem(i, j, new QTableWidgetItem(fields[j]));
			//tab->setItem(i, j, new QTableWidgetItem(QString::number(fields[j].toDouble(),'f',4)));
		}
	}
	structParaFile.close();
	return true;

}

bool csvToTableAdapt(const QString & filePath, QTableWidget * tab)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	//cout << "csvRowCount:" << csvRowCount << endl;
	//cout << "csvColCount:" << csvColCount << endl;
	structParaFile.close();
	//获取table行列数
	int tabRow = tab->rowCount();
	int tabCol = tab->columnCount();
	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	if (csvRowCount > tabRow)
	{
		qDebug() << "数据行数超限!!";
		return false;
	}
	//若csv列数大于当前表格列数则扩展表格列数
	if (csvColCount > tabCol)
	{
		qDebug() << "自动扩展列数->" << csvColCount;
		for (int i = 0; i < csvColCount - tabCol; i++)
		{
			tab->insertColumn(i + tabCol);
			for (int j = 0; j < csvRowCount; j++)
			{
				tab->setItem(j, i + tabCol, new QTableWidgetItem());//添加新元素
			}
		}
	}
	//若csv列数小于当前表格列数则缩小表格
	else if (csvColCount < tabCol)
	{
		for (int i = csvColCount; i < tabCol; i++)
		{
			tab->removeColumn(csvColCount);
		}
	}
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			tab->setItem(i, j, new QTableWidgetItem(fields[j]));
			//tab->setItem(i, j, new QTableWidgetItem(QString::number(fields[j].toDouble(),'f',4)));
		}
	}
	structParaFile.close();
	return true;
}

bool csvToTable(const QString & filePath, QTableWidget * tab, const QString colTitle)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}

	structParaFile.close();
	//获取table行列数
	int tabRow = tab->rowCount();
	int tabCol = tab->columnCount();
	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	if (csvRowCount > tabRow)
	{
		qDebug() << "数据行数超限!!";
		return false;
	}
	//若csv列数大于当前表格列数则扩展表格列数
	if (csvColCount > tabCol)
	{
		qDebug() << "自动扩展列数->" << csvColCount;
		for (int i = 0; i < csvColCount - tabCol; i++)
		{
			tab->insertColumn(i + tabCol);
			QTableWidgetItem * tmpItem = new QTableWidgetItem();
			tab->setHorizontalHeaderItem(i + tabCol, tmpItem);//设置列标题
			QTableWidgetItem * colItem = tab->horizontalHeaderItem(i + tabCol);
			colItem->setText(colTitle + QString::number(i + tabCol + 1));
			for (int j = 0; j < csvRowCount; j++)
			{
				tab->setItem(j, i + tabCol, new QTableWidgetItem());//添加新元素
			}
		}
	}
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			tab->setItem(i, j, new QTableWidgetItem(fields[j]));
			//tab->setItem(i, j, new QTableWidgetItem(QString::number(fields[j].toDouble(),'f',4)));
		}
	}
	structParaFile.close();
	return true;
}

bool csvToMatrixXd(const QString & filePath, Matrix<double, 6, 1>& mat)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	//cout << "csvRowCount:" << csvRowCount << endl;
	//cout << "csvColCount:" << csvColCount << endl;
	structParaFile.close();

	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			mat(i, j) = fields[j].toDouble();
			//qDebug() << fields[j];
		}
	}
	structParaFile.close();
	return true;
}
bool csvToMatrixXd(const QString & filePath, Matrix<double, 3, 6>& mat)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	//cout << "csvRowCount:" << csvRowCount << endl;
	//cout << "csvColCount:" << csvColCount << endl;
	structParaFile.close();

	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			mat(i, j) = fields[j].toDouble();
			//qDebug() << fields[j];
		}
	}
	structParaFile.close();
	return true;
}


bool csvToMatrixXd(const QString & filePath, MatrixXd & mat)
{
	QFile structParaFile(filePath);
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return false;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	cout << "csvRowCount:" << csvRowCount << endl;
	cout << "csvColCount:" << csvColCount << endl;
	structParaFile.close();

	//给矩阵重新分配行列数
	mat.resize(csvRowCount, csvColCount);
	//从csv文件读取数据
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	QTextStream in(&structParaFile);
	QString line;
	QStringList fields;
	for (int i = 0; i < csvRowCount; i++)
	{
		line = in.readLine();
		fields = line.split(',');//按照,分割
		for (int j = 0; j < csvColCount; j++)
		{
			mat(i, j) = fields[j].toDouble();
			//qDebug() << fields[j];
		}
	}
	structParaFile.close();
	return true;
}


bool tableToCsv(const QTableWidget * tab, const QString & filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) { return false; }
	QTextStream stream(&file);
	QString conTents;
	for (int i = 0; i < tab->rowCount(); i++)
	{
		for (int j = 0; j < tab->columnCount(); j++)
		{
			QTableWidgetItem *item = tab->item(i, j);
			if (!item)
			{
				continue;
			}
			QString str = item->text();
			str.replace(",", " ");
			conTents += str + ",";
		}
		conTents = conTents.left(conTents.length() - 1);
		conTents += "\n";
	}
	stream << conTents;
	file.close();
	return true;
}

bool qlistQvectorToCsv( QList<QVector<double>> qlist, const QString & filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) { return false; }
	QTextStream stream(&file);
	QString conTents;
	int rows = qlist.at(0).size();
	int cols = qlist.size();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			
			QString str = QString::number(qlist.at(j).at(i), 'f', 6);
			str.replace(",", " ");
			conTents += str + ",";
		}
		conTents = conTents.left(conTents.length() - 1);
		conTents += "\n";
	}
	stream << conTents;
	file.close();
	return true;
}

Vector2d getCsvSize(const QString &filePath)
{
	QFile structParaFile(filePath);
	Vector2d csvSize = Vector2d::Zero();
	//获取csv文件的行列数
	if (!structParaFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "未打开文件:" << filePath;
		return csvSize;
	}
	QTextStream getCsvRowColCount(&structParaFile);
	int csvRowCount = 0;
	int csvColCount = 0;
	while (!getCsvRowColCount.atEnd())
	{
		QString getRowCount = getCsvRowColCount.readLine();
		QStringList getColCount = getRowCount.split(',');
		csvColCount = getColCount.size();
		csvRowCount++;
	}
	structParaFile.close();
	csvSize(0) = csvRowCount;
	csvSize(1) = csvColCount;
	return csvSize;

}

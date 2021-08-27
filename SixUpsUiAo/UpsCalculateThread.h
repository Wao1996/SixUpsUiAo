#pragma once

#include <QObject>
#include <QDebug>
#include "AlgorithmFor6UPS.h"
#include "Global.h"

class UpsCalculateThread : public QObject
{
	Q_OBJECT

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;//eigen�� ָ����� ���򱨴�
	UpsCalculateThread(QObject *parent = Q_NULLPTR);
	~UpsCalculateThread();
public slots:
	void on_upsCalculateTimer();
};

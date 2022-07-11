#pragma once

#include <QObject>
#include <QThread>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
#include <cmath>
#include "attoWPUsrc/AttoWPU.h"

using namespace attoWPU::simulator;

class AttoWPUthread : public QThread
{
    Q_OBJECT

    AttoWPU *emul;
    volatile float *freq;
    volatile bool *maxhz;
    volatile bool stop;
    QTime time;
    QMutex StopMutex;

public:
    AttoWPUthread(AttoWPU *wpu, float *frequency, bool *unlimited);
    void run();
    void StartIfNotRunning() { if(!isRunning()) start(QThread::NormalPriority); sleep(0); }
    void Stop();

signals:
    void UpdateUI();
};

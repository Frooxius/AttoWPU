#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QRadioButton>
#include <QCheckBox>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QFile>
#include <QTableWidget>
#include "attoWPUsrc/AttoWPU.h"
#include "attoWPUsrc/attoasm.h"
#include "attoWPUsrc/AttocodeWriteMem.h"
#include "attoWPUsrc/SourceFile.h"
#include "HEXeditor.h"
#include "lcdlargeview.h"
#include <QAudioOutput>
#include <QIODevice>
#include <QByteArray>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QByteArray>
#include "attoWPUsrc/MessageDummy.h"
#include <QKeyEvent>
#include "NameStrings.h"
#include "attoassembling.h"
#include "aboutdialog.h"
#include "QLedIndicator-0.1.1/qledindicator.h"
#include "Switch.h"
#include "AttoWPUthread.h"

namespace Ui {
    class SimulatorWindow;
}



class WPUSwitch : public Switch
{
    Q_OBJECT

    unsigned int _id;

public:
    WPUSwitch(unsigned int id, QWidget *parent) : Switch(parent)
    {
        _id = id;
        connect(this, SIGNAL(toggled(bool)), this, SLOT(checkboxChanged(bool)) );
    }

    unsigned int GetID() { return _id; }

public slots:
    void checkboxChanged(bool state)
    {
        emit switchToggled(state, _id%32, _id/32);
    }

signals:
    void switchToggled(bool state, int id, int row);
};

class SimulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulatorWindow(QWidget *parent = 0);
    ~SimulatorWindow();

signals:
    void newIA(int addr);
    void AttoCodeUpdated();
    void newLCDdata();

private:
    Ui::SimulatorWindow *ui;

    attoWPU::simulator::AttoWPU *wpu;
    AttoWPUthread *emulthread;
    uchar *attoCodeMemory;
    int attoCodeMemorySize;

    QTimer *update, *freq;
    QTime time;

    QLedIndicator *LED[128];
    WPUSwitch *SWITCH[128];
    Switch *ROWSWITCH[4];

    uchar lcdData[49152];
    QGraphicsScene *LCDscene;
    LCDlargeview *hugeLCD;

    HEXedit *attoCodeMemView, *RAMView, *registerView, *queueView, *LCDview, *textView;

    AttoAssembling *attoasm;


    void UpdateWPU();

    void UpdateSpeed();
    bool lockSpeed;

    float hz;    // frequency
    bool unlimitedHz;   // unlimited frequency
    unsigned long long prevCycles;
    unsigned long long realtime;    // the real time

    bool eventFilter(QObject *object, QEvent *event);

    // Loading and saving from/to files
    void LoadAttocodeHEX(QString filename);
    void LoadAttocodeATT(QString filename);

    // Audio
    QAudioOutput *audioout;
    QIODevice *audiowrite;
    QAudioFormat *audioformat;
    QTimer *audiotimer;

    // Windows
    AboutDialog *about;

private slots:
    void on_actionAbout_attoWPU_simulator_triggered();
    void on_openHugeLCD_clicked();
    void on_actionHardReset_triggered();
    void on_actionSoftReset_triggered();
    void on_actionLoad_Source_triggered();
    void on_actionLoad_HEX_triggered();
    void on_toggleSWrow3_stateChanged(bool state);
    void on_toggleSWrow2_stateChanged(bool state);
    void on_toggleSWrow1_stateChanged(bool state);
    void on_toggleSWrow0_stateChanged(bool state);
    void on_WPUstep_clicked();
    void on_WPU1MHz_clicked();
    void on_WPU30Hz_clicked();
    void on_WPUstop_clicked();
    void on_speedValue_editingFinished();
    void on_speedSlider_valueChanged(int position);
    void Update();
    void UpdateFreq();
    void ToggleSwitch(bool state, int id, int row);
    void FillAudioBuffer();
    void Closing();

    void UpdateUI();
    void on_WPMax_clicked();
};



#endif // SIMULATORWINDOW_H

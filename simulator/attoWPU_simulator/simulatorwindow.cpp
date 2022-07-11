#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include <cmath>
#include <cstdio>

SimulatorWindow::SimulatorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);

    // Windows
    about = new AboutDialog(this);

    connect(this, SIGNAL(destroyed()), this, SLOT(Closing()));

    update = new QTimer(this);
    freq = new QTimer(this);
    ui->unlimitedL->setVisible(false);
    unlimitedHz = false;
    connect(update, SIGNAL(timeout()), this, SLOT(Update()));
    connect(freq, SIGNAL(timeout()), this, SLOT(UpdateFreq()));

    for(int i = 0, a = 0; i < 128; i++)
    {
        if(!(i%8))
            a++;
        if(!(i%32))
            a = 0;
        LED[i] = new QLedIndicator(ui->LEDsG);
        LED[i]->setMinimumSize(14, 14);
        LED[i]->setGeometry(QRect(51+(i%32)*14 + a*4, 15+((i/32)*15), 14, 14));
        LED[i]->setAutoExclusive(false);
        LED[i]->setOnColor1(QColor(255, 128, 64));
        LED[i]->setOnColor2(QColor(255, 0, 0));
        LED[i]->setOffColor1(QColor(64, 64, 64));
        LED[i]->setOffColor2(QColor(0, 0, 0));

        SWITCH[i] = new WPUSwitch(i, ui->switchesG);
        SWITCH[i]->setGeometry(QRect( ((66+ (31-(i%32)) *14) - a*4) + 12, 12+((i/32)*15), 14, 14));
        connect(SWITCH[i], SIGNAL(switchToggled(bool,int,int)), this, SLOT(ToggleSwitch(bool,int,int)));
    }

    for(int i = 0; i < 4; i++)
    {
        ROWSWITCH[i] = new Switch(ui->switchesG);
        ROWSWITCH[i]->setGeometry(QRect(6, 12+i*15, 14, 14));
    }

    connect(ROWSWITCH[0], SIGNAL(toggled(bool)), this, SLOT(on_toggleSWrow0_stateChanged(bool)));
    connect(ROWSWITCH[1], SIGNAL(toggled(bool)), this, SLOT(on_toggleSWrow1_stateChanged(bool)));
    connect(ROWSWITCH[2], SIGNAL(toggled(bool)), this, SLOT(on_toggleSWrow2_stateChanged(bool)));
    connect(ROWSWITCH[3], SIGNAL(toggled(bool)), this, SLOT(on_toggleSWrow3_stateChanged(bool)));

    LCDscene = new QGraphicsScene(0, 0, 128, 128, this);
    ui->LCD->setScene(LCDscene);
    hugeLCD = new LCDlargeview(LCDscene, this);

    freq->setInterval(1000);
    freq->start();
    hz = 0;

    lockSpeed = false;

     // Create the WPU
    attoCodeMemorySize = 16*1024*1024;
    attoCodeMemory = new uchar[attoCodeMemorySize];

    wpu = new attoWPU::simulator::AttoWPU(attoCodeMemory, attoCodeMemorySize, &realtime);
    emulthread = new AttoWPUthread(wpu, &hz, &unlimitedHz);
    connect(emulthread, SIGNAL(UpdateUI()), this, SLOT(UpdateUI()));
    prevCycles = 0;

    // create the viewers
    attoCodeMemView = new HEXedit(2, 2, 640, attoCodeMemory, attoCodeMemorySize, ui->attoCodeMemRegion);
    connect(this, SIGNAL(newIA(int)), attoCodeMemView, SLOT(Select(int)));

    LCDview = new HEXedit(2, 2, 640, wpu->GetLCDDispPtr(), 128*128*3, ui->lcdMemRegion);
    connect(this, SIGNAL(newLCDdata()), LCDview, SLOT(FetchData()));

    textView = new HEXedit(2, 2, 640, wpu->GetTextDispPtr(), 160, ui->textDisplayMemRegion);

    registerView = new HEXedit(2, 2, 640, wpu->GetRegMemPtr(), 256, ui->registerMemoryRegion);

    queueView = new HEXedit(2, 2, 640, wpu->GetSQueuePtr(), 256, ui->smallQueueRegion);

    /*
    // Audio output
    audioformat = new QAudioFormat();
    audioformat->setSampleRate(44100);
    audioformat->setSampleSize(8);
    audioformat->setCodec("audio/pcm");
    audioformat->setByteOrder(QAudioFormat::LittleEndian);
    audioformat->setChannels(1);
    audioformat->setSampleType(QAudioFormat::UnSignedInt);

    audioout = new QAudioOutput(*audioformat, this);
    audioout->setBufferSize(4096);
    audiowrite = audioout->start();

    audiotimer = new QTimer(this);
    audiotimer->start(1);
    connect(audiotimer, SIGNAL(timeout()), this, SLOT(FillAudioBuffer()));*/

    attoasm = new AttoAssembling(this);
    connect(attoasm, SIGNAL(AttoAssemblyFinished()), attoCodeMemView, SLOT(FetchData()));

    UpdateUI();
}

void SimulatorWindow::Update()
{

}

void SimulatorWindow::UpdateFreq()
{
    char buf[50];
    std::string tmpstr;

    // detect real frequency
    sprintf(buf, "%d", wpu->GetCycles()-prevCycles );
    prevCycles = wpu->GetCycles();
    tmpstr = "Real: ";
    tmpstr += buf;
    tmpstr += " Hz";
    ui->WPUspeedL->setText(tmpstr.c_str());
}

void SimulatorWindow::UpdateUI()
{
    emit newIA(wpu->GetIA());

    char buf[200];
    std::string tmpstr;
    sprintf(buf, "%d", wpu->GetIA());
    ui->processedInstructionsL->setText( buf );

    // update bus
    tmpstr = wpu->GetBusStr();
    ui->addrB->setText( tmpstr.substr(0, 8).c_str() );
    ui->ctrlB->setText( tmpstr.substr(8, 8).c_str() );
    ui->dataB->setText( tmpstr.substr(16, 32).c_str() );
    ui->ajmpB->setText( tmpstr.substr(48, 16).c_str() );

    sprintf(buf, "%2.2X", wpu->GetAddr());
    ui->addrH->setText( buf );
    if(wpu->GetAddr() < 0x0F)
        ui->addrS->setText( nameADDR[wpu->GetAddr()] );
    else
        ui->addrS->setText( "----");

    sprintf(buf, "%2.2X", wpu->GetCtrl() >> 1);
    ui->ctrlH->setText( buf );
    sprintf(buf, "%8.8X", wpu->GetData());
    ui->dataH->setText( buf );
    sprintf(buf, "%4.4X", wpu->GetAjmp());
    ui->ajmpH->setText( buf );

    sprintf(buf, "%d", wpu->GetData());
    ui->dataINT->setText(buf);
    sprintf(buf, "%u", wpu->GetData());
    ui->dataUINT->setText(buf);
    union { unsigned int i; float f; } intfloat;
    intfloat.i = wpu->GetData();
    sprintf(buf, "%e", (double)intfloat.f);
    ui->dataFLOAT->setText(buf);

    // Update LEDs
    for(int i = 0; i < 4; i++)
    {
        unsigned int row = wpu->GetLEDrow(i);
        for(int n = 0; n < 32; n++)
            LED[(31-n)+(32*i)]->setChecked( (row >> n) & 0x01 );
    }

    // Update Text Display
    wpu->GetTextDisplay(buf);
    ui->TextDisplay->setPlainText(buf);
    textView->FetchData();

    if(hz > 1000)
    {
        attoCodeMemView->setDisabled(true);
    }
    else
    {
        attoCodeMemView->setEnabled(true);
    }

    // Update LCD
    if(wpu->LCDNewData())
    {
        wpu->GetLCDNewData24b(lcdData);
        QImage img(lcdData, 128, 128, QImage::Format_RGB888);
        LCDscene->clear();
        LCDscene->addPixmap(QPixmap::fromImage(img));
        emit newLCDdata();
    }

    // DEBUG
    registerView->FetchData();
}

void SimulatorWindow::UpdateWPU()
{

}

void SimulatorWindow::UpdateSpeed()
{
    lockSpeed = true;
    if(unlimitedHz)
        ui->speedSlider->setValue(ui->speedSlider->maximum());
    else if(hz > 0.0)
        ui->speedSlider->setValue(std::min(std::log10(hz+1.0)*42.85, 300.0));
    else
    {
        ui->speedSlider->setValue(0);
        emulthread->Stop();
    }
    ui->speedValue->setValue(hz);
    lockSpeed = false;

    ui->speedValue->setVisible(!unlimitedHz);
    ui->unlimitedL->setVisible(unlimitedHz);

    if((hz > 0.0 || unlimitedHz))
        emulthread->StartIfNotRunning();
}

SimulatorWindow::~SimulatorWindow()
{
    delete ui;
    delete wpu;
}

void SimulatorWindow::on_speedSlider_valueChanged(int position)
{
    if(lockSpeed) return;
    unlimitedHz = false;
    if(position == 0)
        hz = 0.0;
    else if (position == ui->speedSlider->maximum())
        unlimitedHz = true;
    else
        hz = std::pow(10.0, position/42.85)-1.0;
    UpdateSpeed();
}

void SimulatorWindow::on_speedValue_editingFinished()
{
    if(lockSpeed) return;
    hz = ui->speedValue->value();
    unlimitedHz = false;
    UpdateSpeed();
}

void SimulatorWindow::on_WPUstop_clicked()
{
    hz = 0.0;
    unlimitedHz = false;
    UpdateSpeed();
}

void SimulatorWindow::on_WPU30Hz_clicked()
{
    hz = 30.0;
    unlimitedHz = false;
    UpdateSpeed();
}

void SimulatorWindow::on_WPU1MHz_clicked()
{
    hz = 1000000.0;
    unlimitedHz = false;
    UpdateSpeed();
}

void SimulatorWindow::on_WPUstep_clicked()
{
    wpu->Cycle();
    UpdateUI();
}

void SimulatorWindow::ToggleSwitch(bool state, int id, int row)
{
    wpu->SetInputSwitch(row, id, state);
}

// Hadnle keypresses
bool SimulatorWindow::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *key = (QKeyEvent *)event;
        if(key->key() >= Qt::Key_0 && key->key() <= Qt::Key_9)
            wpu->SetInputNumKey(key->key() - Qt::Key_0, event->type() == QEvent::KeyPress);

        if(key->key() >= Qt::Key_A && key->key() <= Qt::Key_Z)
            wpu->SetInputAlphaKey(key->key() - Qt::Key_A, event->type() == QEvent::KeyPress);
    }
    return false;
}

void SimulatorWindow::on_toggleSWrow0_stateChanged(bool state)
{
    for(int i = 0; i < 32; ++i)
        SWITCH[i]->setChecked(state);
}

void SimulatorWindow::on_toggleSWrow1_stateChanged(bool state)
{
    for(int i = 32; i < 64; ++i)
        SWITCH[i]->setChecked(state);
}

void SimulatorWindow::on_toggleSWrow2_stateChanged(bool state)
{
    for(int i = 64; i < 96; ++i)
        SWITCH[i]->setChecked(state);
}

void SimulatorWindow::on_toggleSWrow3_stateChanged(bool state)
{
    for(int i = 96; i < 128; ++i)
        SWITCH[i]->setChecked(state);
}

void SimulatorWindow::on_actionLoad_HEX_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load HEX file into the attocode memory"), "", tr("HEX files (*.hex *.bin)"));
    LoadAttocodeHEX(file);
}

void SimulatorWindow::LoadAttocodeHEX(QString filename)
{
    QFile file(filename);

    if(!file.exists())
        return;

    if(!file.open( QFile::ReadOnly ))
        return;

    file.read( (char *)attoCodeMemory, attoCodeMemorySize );
    file.close();
    emit AttoCodeUpdated();
}

void SimulatorWindow::LoadAttocodeATT(QString filename)
{
    if(!QFile(filename).exists())
        return;

    attoasm->AttoAssembly(filename, attoCodeMemory, attoCodeMemorySize);
}

void SimulatorWindow::on_actionLoad_Source_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Attoassembly attoASM source file into the attocode memory"), "", tr("AttoASM source files (*.att)"));
    LoadAttocodeATT(file);
}

void SimulatorWindow::on_actionSoftReset_triggered()
{
    hz = 0;
    unlimitedHz = false;
    UpdateSpeed();
    wpu->Reset(false);
    UpdateUI();
}

void SimulatorWindow::on_actionHardReset_triggered()
{
    hz = 0;
    unlimitedHz = false;
    UpdateSpeed();

    wpu->Reset(true);
    for(int i = 0; i < 128; ++i)
        SWITCH[i]->setChecked(false);
    UpdateUI();
    emit AttoCodeUpdated();
}

void SimulatorWindow::on_openHugeLCD_clicked()
{
    if(hugeLCD->isHidden())
        hugeLCD->show();
}

void SimulatorWindow::FillAudioBuffer()
{
    QByteArray data = QByteArray((const char *)wpu->GetSpeakerSoundData(), (int)wpu->GetSpeakerDataSize());
    audiowrite->write(data);
    wpu->ResetSpeakerBuffer();
}

void SimulatorWindow::on_actionAbout_attoWPU_simulator_triggered()
{
    about->show();
}

void SimulatorWindow::on_WPMax_clicked()
{
    unlimitedHz = true;
    UpdateSpeed();
}

void SimulatorWindow::Closing()
{
    hugeLCD->close();
}

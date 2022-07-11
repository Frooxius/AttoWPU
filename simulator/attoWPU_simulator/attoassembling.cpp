#include "attoassembling.h"
#include "ui_attoassembling.h"
#include "attoWPUsrc/MessageDummy.h"
#include "attoWPUsrc/Exception.h"
#include "SourceQFile.h"

AttoAssembling::AttoAssembling(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttoAssembling)
{
    ui->setupUi(this);

    update = new QTimer(this);
    update->setInterval(10);

    connect(update, SIGNAL(timeout()), this, SLOT(Cycle()));
}

AttoAssembling::~AttoAssembling()
{
    delete ui;
    delete update;
}

void AttoAssembling::AttoAssembly(QString filename, uchar *attocodeMemory, int attocodeSize)
{
    assembler = new attoWPU::assembler::AttoASM(new attoWPU::assembler::SourceQFile(filename),
                                              new attoWPU::assembler::AttocodeWriteMem(attocodeSize, attocodeMemory),
                                              new attoWPU::shared::MessageDummy(false, 0));
    show();
    update->start(); // start the timer
}

void AttoAssembling::Cycle()
{
    try
    {
    assembler->Assembly(50000);
}
    catch (attoWPU::assembler::Exception e)
    {
        ui->textBrowser->setText(e.GetMessage().c_str());
        update->stop();
        delete assembler;
        return;
    }

    if(assembler->Done())
    {
        update->stop();
        close();
        delete assembler;
        emit AttoAssemblyFinished();
    }
}

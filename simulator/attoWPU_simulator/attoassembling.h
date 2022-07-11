#ifndef ATTOASSEMBLING_H
#define ATTOASSEMBLING_H

#include <QDialog>
#include "attoWPUsrc/attoasm.h"
#include "attoWPUsrc/AttocodeWriteMem.h"
#include "QTimer"

namespace Ui {
    class AttoAssembling;
}

class AttoAssembling : public QDialog
{
    Q_OBJECT

public:
    explicit AttoAssembling(QWidget *parent = 0);
    ~AttoAssembling();

    QTimer *update;

    void AttoAssembly(QString filename, uchar *attocodeMemory, int attocodeSize);

signals:
    void AttoAssemblyFinished();

private:
    Ui::AttoAssembling *ui;
    attoWPU::assembler::AttoASM *assembler;

public slots:
    void Cycle();
};

#endif // ATTOASSEMBLING_H

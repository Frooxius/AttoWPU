#include "lcdlargeview.h"
#include "ui_lcdlargeview.h"

LCDlargeview::LCDlargeview(QGraphicsScene *scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LCDlargeview)
{
    ui->setupUi(this);
    ui->HugeLCD->setScene(scene);
    ui->HugeLCD->scale(ui->HugeLCD->width()/128, ui->HugeLCD->height()/128);
    setWindowFlags(Qt::Drawer | Qt::MSWindowsFixedSizeDialogHint);
}

LCDlargeview::~LCDlargeview()
{
    delete ui;
}

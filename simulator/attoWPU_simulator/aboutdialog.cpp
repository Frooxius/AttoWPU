#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_label_linkActivated(QString link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutDialog::on_pushButton_clicked()
{
    close();
}

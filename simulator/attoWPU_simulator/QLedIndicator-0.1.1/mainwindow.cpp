/***************************************************************************
 *   Copyright (C) 2010 by Tn                                              *
 *   thenobody@poczta.fm                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QButtonGroup>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    group = new QButtonGroup(this);
    timer = new QTimer(this);
    timer->setInterval(250);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    for(int i=0; i<8; i++) {
        leds.append(new QLedIndicator(this));
    }
    for(int i=0; i<leds.size(); i++) {
        ui->layoutIndependent->addWidget(leds[i]);
    }
    for(int i=0; i<8; i++) {
        group->addButton(new QLedIndicator(this), i);
        ui->layoutGroup->addWidget(group->button(i));
    }
    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(on_group_change(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonTest_clicked()
{
    qDebug() << "Test time!";
    if(timer->isActive()) {
        timer->stop();
        ui->buttonTest->setText("Test");
    } else {
        timer->start();
        ui->buttonTest->setText("Stop test");
    }
}

void MainWindow::on_buttonQuit_clicked()
{
    close();
}

void MainWindow::on_timer_timeout() {
    static unsigned char j = 0;
    for(int i=0; i<8; i++) {
        leds[7-i]->setChecked( ((j>>i)&0x01)==1 );
    }
    group->button(j%8)->setChecked(true);
    ui->ledIndicator_0->toggle();
    j++;
    qDebug() << "timer!";
}

void MainWindow::on_ledIndicator_0_toggled(bool checked)
{
    qDebug() << "Led toggled: " << checked;
}

void MainWindow::on_group_change(int i) {
    qDebug() << "Signal from QButtonGroup! Button id: " << i;
}

#include "Switch.h"
#include <QPainter>

const qreal Switch::scaledSize = 500;

Switch::Switch(QWidget *parent) : QAbstractButton(parent)
{
    setMinimumSize(16,16);
    setCheckable(true);
}

void Switch::resizeEvent(QResizeEvent *event) {
    update();
}

void Switch::paintEvent(QPaintEvent *event) {
    qreal realSize = qMin(width(), height());

    QLinearGradient gradient;
    QPainter painter(this);
    QPen     pen(Qt::black);
             pen.setWidth(1);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/4, height()/4);
    painter.scale(realSize/scaledSize, realSize/scaledSize);

    gradient = QLinearGradient(QPointF(0,0), QPointF(0,500));
    gradient.setColorAt(0, QColor(32,32,32));
    gradient.setColorAt(1, QColor(0,0,0));
    painter.setPen(pen);
    painter.setBrush(QBrush(gradient));
    painter.drawRect(0, 0, 500, 500);

    gradient = QLinearGradient(QPointF(0, 0), QPointF(0,400));
    gradient.setColorAt(0, QColor(0,0,0));
    gradient.setColorAt(1, QColor(128,128,128));
    painter.setPen(pen);
    painter.setBrush(QBrush(gradient));
    painter.drawRect(25, 25, 325, 325);

    painter.setPen(pen);
    gradient = QLinearGradient(QPointF(0,0), QPointF(0,200));
    gradient.setColorAt(0, QColor(255*(!isChecked()),255*(isChecked()),0));
    gradient.setColorAt(1, QColor(220*(!isChecked()),160*(isChecked()),0));

    painter.setBrush(gradient);
    painter.drawRect(25, 25+(325/2)*(!isChecked()), 325, 325/2);
}


#pragma once

#include <QAbstractButton>
#include <QResizeEvent>
#include <QColor>
#include <QDebug>

class Switch : public QAbstractButton
{
    Q_OBJECT
    public:
        Switch(QWidget *parent);

    protected:
        virtual void paintEvent (QPaintEvent *event);
        virtual void resizeEvent(QResizeEvent *event);

    private:
        static const qreal scaledSize;  /* init in cpp */
        QPixmap drawBuffer;
};

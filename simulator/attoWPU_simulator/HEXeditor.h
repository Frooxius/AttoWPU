#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#endif // HEXEDITOR_H

#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QWheelEvent>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>

class HEXedit : public QTableWidget
{
    Q_OBJECT

private:

    uchar  *data;  // data that are being shown/edited
    qint64  size,    // size of the data
            base;   // current base address
    QScrollBar *scrollbar;

    void UpdateRowCaptions();

public:
    HEXedit(int x, int y, int _height, uchar *_data, qint64 _size, QWidget *parent = 0) : QTableWidget(0, 17, parent)
    {
        size = _size;
        data = _data;
        base = 0;

        setFont(QFont("Lucida Console", 8));
        setGeometry(x, y, 544+16, _height-(_height%16));
        horizontalHeader()->setResizeMode(QHeaderView::Fixed);
        verticalHeader()->setResizeMode(QHeaderView::Fixed);
        horizontalHeader()->setFont(QFont("Lucida Console", 8));
        setContentsMargins(0,0,0,0);

        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QStringList labels;
        for(int i = 0; i < 16; i++)
        {
            char buf[3];
            sprintf(buf, "%2.2X", i);
            labels << buf;
            setColumnWidth(i, 22);
        }
        labels << "ASCII";
        setColumnWidth(16, 120);
        setHorizontalHeaderLabels(labels);

        // generate rows
        for(int i = 0; i < (height()/15)-1; i++)
        {
            insertRow(i);
            setRowHeight(i, 15);
        }

        scrollbar = new QScrollBar(Qt::Vertical, this);
        scrollbar->setGeometry(width()-17, 1, 16, height()-2);
        scrollbar->setMaximum( (size)/16 - rowCount() + 1 );
        connect(scrollbar, SIGNAL(valueChanged(int)), this, SLOT(ScrollUpdate(int)));

        UpdateRowCaptions();
        FetchData();
    }


private slots:
    void ScrollUpdate(int pos);
    void wheelEvent(QWheelEvent *event);

public slots:
    void FetchData();
    void Select(int addr);
};

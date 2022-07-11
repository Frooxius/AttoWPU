#include "HEXeditor.h"

void HEXedit::UpdateRowCaptions()
{
    char buf[9];
    QStringList labels;
    for(int i = 0; i < rowCount(); i++)
    {
        sprintf(buf, "%8.8X", base+i*16);
        labels << buf;
    }

    setVerticalHeaderLabels(labels);
}

void HEXedit::ScrollUpdate(int pos)
{
    base = pos*16;
    if(base < 0)
        base = 0;

    scrollbar->setValue(base/16);

    UpdateRowCaptions();
    FetchData();
}

void HEXedit::wheelEvent(QWheelEvent *event)
{
    ScrollUpdate(base/16 - event->delta()/20);
}

void HEXedit::FetchData()
{
    if(!isEnabled())
        return;
    char buf[3], bufascii[17];
    *(bufascii+16) = 0;
    for(int r = 0, i = 0; r < rowCount(); ++r)
    {
        for(int c = 0; c < 16; ++c, ++i)
        {
            if(base+i < size)
            {
                sprintf(buf, "%2.2X", *(data+base+i));
                setItem(r, c, new QTableWidgetItem(buf));

                if(isprint(*(data+base+i)))
                    *(bufascii+c) = *(data+base+i);
                else
                    *(bufascii+c) = '.';
            }
            else
            {
                setItem(r, c, new QTableWidgetItem(""));
                *(bufascii+c) = ' ';
            }
        }
        setItem(r, 16, new QTableWidgetItem(bufascii));
    }
}

void HEXedit::Select(int addr)
{
    if(!isEnabled())
        return;

    if(addr < base || addr > (base+rowCount()*16) )
        ScrollUpdate(addr/16);

    setCurrentCell((addr-base)/16, (addr-base)%16);
}

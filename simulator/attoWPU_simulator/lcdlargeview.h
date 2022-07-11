#ifndef LCDLARGEVIEW_H
#define LCDLARGEVIEW_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
    class LCDlargeview;
}

class LCDlargeview : public QDialog
{
    Q_OBJECT

public:
    explicit LCDlargeview(QGraphicsScene *scene, QWidget *parent = 0);
    ~LCDlargeview();

private:
    Ui::LCDlargeview *ui;
};

#endif // LCDLARGEVIEW_H

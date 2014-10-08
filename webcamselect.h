#ifndef WEBCAMSELECT_H
#define WEBCAMSELECT_H

#include <QDialog>

namespace Ui {
class webcamselect;
}

class webcamselect : public QDialog
{
    Q_OBJECT

public:
    explicit webcamselect(QWidget *parent = 0);
    ~webcamselect();

private:
    Ui::webcamselect *ui;
};

#endif // WEBCAMSELECT_H

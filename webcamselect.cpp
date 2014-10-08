#include "webcamselect.h"
#include "ui_webcamselect.h"

webcamselect::webcamselect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webcamselect)
{
    ui->setupUi(this);
}

webcamselect::~webcamselect()
{
    delete ui;
}

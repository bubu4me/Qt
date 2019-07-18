#include "dialogtransform.h"
#include "ui_dialogtransform.h"

#include <QDebug>

DialogTransform::DialogTransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTransform)
{
    ui->setupUi(this);
}

DialogTransform::~DialogTransform()
{
    delete ui;
}

void DialogTransform::accept()
{
    qDebug() << "DialogTransform::accept";
    bool ok;
    double c = ui->lineEdit->text().toDouble(&ok);
    if (ok) {
        emit confirmed(c);// 发送信号
    }
    close();
}

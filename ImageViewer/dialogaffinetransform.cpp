#include "dialogaffinetransform.h"
#include "ui_dialogaffinetransform.h"

#include <QDebug>

DialogAffineTransform::DialogAffineTransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAffineTransform)
{
    ui->setupUi(this);
}

DialogAffineTransform::~DialogAffineTransform()
{
    delete ui;
}

void DialogAffineTransform::accept()
{
    unsigned int matSize = 6;
    QMatrix data;
    bool *ok = new bool[matSize];
    bool flag = true;

    double m11 = ui->lineEdit_00->text().toDouble(&ok[0]);
    double m12 = ui->lineEdit_01->text().toDouble(&ok[1]);
    double m21 = ui->lineEdit_10->text().toDouble(&ok[2]);
    double m22 = ui->lineEdit_11->text().toDouble(&ok[3]);
    double dx = ui->lineEdit_20->text().toDouble(&ok[4]);
    double dy = ui->lineEdit_21->text().toDouble(&ok[5]);

    for (unsigned int i = 0; i < matSize; ++i) {
        if (!ok[i]) {
            flag = false;
        }
    }

    if (flag) {
        qDebug() << "true" << endl;
        data.setMatrix(m11, m12, m21, m22, dx, dy);
    }else{
        qDebug() << "invalid input" << endl;
        data.setMatrix(1, 1, 1, 1, 1, 1);
    }
    emit confirmed(data);
    close();
}

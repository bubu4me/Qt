#include "dialoggaussfilter.h"
#include "ui_dialoggaussfilter.h"

#include <QDebug>

DialogGaussFilter::DialogGaussFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGaussFilter)
{
    ui->setupUi(this);
}

DialogGaussFilter::~DialogGaussFilter()
{
    delete ui;
}

void DialogGaussFilter::accept()
{
    // 获取图像类型
    int imageType = 0;
    if (ui->radioButton_gray->isChecked()) {
        imageType = 0;
    }else if (ui->radioButton_rgb->isChecked()) {
        imageType = 1;
    }

    // 获取边缘处理模式
    int borderType = 0;
    if (ui->radioButton_fillZeros->isChecked()) {
        borderType = 0;
    }else if (ui->radioButton_repeat->isChecked()) {
        borderType = 1;
    }else if (ui->radioButton_mirror->isChecked()) {
        borderType = 2;
    }

    bool ok_row;
    bool ok_sigma;
    int row = ui->lineEdit_row->text().toInt(&ok_row);
    double sigma = ui->lineEdit_sigma->text().toDouble(&ok_sigma);
    qDebug() << ok_row << ok_sigma;
    if (ok_row && ok_sigma) {
        emit confirmed(imageType, borderType, row, sigma);
    }else {
        qDebug() << "wrong row or sigma";
        qDebug() << row << sigma;
    }
    qDebug() << "gauss accept";
    close();
}

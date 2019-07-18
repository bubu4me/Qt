#include "dialogmedianfilter.h"
#include "ui_dialogmedianfilter.h"
#include <QDebug>

DialogMedianFilter::DialogMedianFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMedianFilter)
{
    ui->setupUi(this);
}

DialogMedianFilter::~DialogMedianFilter()
{
    delete ui;
}

void DialogMedianFilter::accept()
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
    bool ok_col;
    int row = ui->lineEdit_row->text().toInt(&ok_row);
    int col = ui->lineEdit_col->text().toInt(&ok_col);
    if (ok_row && ok_col) {
        emit confirmed(imageType, borderType, row, col);
    }else{
        qDebug() << "wrong row or col";
        qDebug() << row << col;
    }
    close();

}

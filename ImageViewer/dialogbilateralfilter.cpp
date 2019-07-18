#include "dialogbilateralfilter.h"
#include "ui_dialogbilateralfilter.h"
#include <QDebug>

DialogBilateralFilter::DialogBilateralFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBilateralFilter)
{
    ui->setupUi(this);
}

DialogBilateralFilter::~DialogBilateralFilter()
{
    delete ui;
}

void DialogBilateralFilter::accept()
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
    bool ok_sigma_filter;
    bool ok_sigma_image;
    int row = ui->lineEdit_row->text().toInt(&ok_row);
    double sigma_filter = ui->lineEdit_sigma_filter->text().toDouble(&ok_sigma_filter);
    double sigma_image = ui->lineEdit_sigma_image->text().toDouble(&ok_sigma_image);

    qDebug() << ok_row << ok_sigma_filter << ok_sigma_image;
    if (ok_row && ok_sigma_filter && ok_sigma_image) {
        emit confirmed(imageType, borderType, row, sigma_filter, sigma_image);
    }else {
        qDebug() << "wrong row or sigma";
        qDebug() << row << sigma_filter << sigma_image;
    }
    qDebug() << "gauss accept";
    close();
}

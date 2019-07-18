#include "dialoglinear.h"
#include "ui_dialoglinear.h"
#include <QDebug>
#include <QRegularExpression>


using std::vector;

DialogLinear::DialogLinear(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLinear)
{
    ui->setupUi(this);
}

DialogLinear::~DialogLinear()
{
    delete ui;
}

void DialogLinear::accept()
{
    QString str = ui->textEdit->toPlainText();

    str = str.replace("\n", "").trimmed();
    QStringList rows = str.split(";");
    qDebug() << str;
    vector<double> data;// 一维数组存储滤波器矩阵
    int nCol = -1;
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].isEmpty()) {
            continue;
        }
        int tmp;
        bool ok = parseRow(rows[i], data, tmp);
        if (nCol == -1) {
            nCol = tmp;
        }
        if (!ok || nCol != tmp) {
            return;
        }
    }

    // 获取图像类型
    int imageType = 0;
    if (ui->radioButton_gray->isChecked()) {
        imageType = 0;
    }else if (ui->radioButton_rgb->isChecked()) {
        imageType = 1;
    }
    qDebug() << "image type" << imageType;

    // 获取边缘处理模式
    int borderType = 0;
    if (ui->radioButton_fillZeros->isChecked()) {
        borderType = 0;
    }else if (ui->radioButton_repeat->isChecked()) {
        borderType = 1;
    }else if (ui->radioButton_mirror->isChecked()) {
        borderType = 2;
    }
    qDebug() << "data:" << data;
    qDebug() << "nCol:" << nCol;
    emit confirmed(data, nCol, imageType, borderType);
    close();
}

bool DialogLinear::parseRow(QString row, vector<double>& data, int& nCol)
{
    nCol = 0;
    QStringList cols = row.trimmed().split(QRegExp("\\s+|\\s*,"));
    qDebug() << "parseRow";

    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].isEmpty()) {
            continue;
        }
        bool ok;
        double d = cols[i].toDouble(&ok);
        if (ok) {
            data.push_back(d);
            nCol++;
        }else {
            return false;
        }
    }
    return true;
}

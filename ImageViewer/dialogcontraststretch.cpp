#include "dialogcontraststretch.h"
#include "ui_dialogcontraststretch.h"

#include <QDebug>

DialogContrastStretch::DialogContrastStretch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogContrastStretch)
{
    ui->setupUi(this);
}

DialogContrastStretch::~DialogContrastStretch()
{
    delete ui;
}

void DialogContrastStretch::accept()
{
    qDebug() << "DialogContrastStretch::accept";
    bool ok_m, ok_E;
    double m = ui->lineEdit_m->text().toDouble(&ok_m);
    double E = ui->lineEdit_E->text().toDouble(&ok_E);
    if (ok_m && ok_E) {
        emit confirmed(m, E);// 发送信号
    }

    close();
}

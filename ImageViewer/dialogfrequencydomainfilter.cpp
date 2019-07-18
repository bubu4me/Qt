#include "dialogfrequencydomainfilter.h"
#include "ui_dialogfrequencydomainfilter.h"

DialogFrequencyDomainFilter::DialogFrequencyDomainFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFrequencyDomainFilter)
{
    ui->setupUi(this);
}

DialogFrequencyDomainFilter::~DialogFrequencyDomainFilter()
{
    delete ui;
}

void DialogFrequencyDomainFilter::accept()
{
    int filterType = 0;

    if (ui->radioButton_butterworth->isChecked()) {
        filterType = 0;
    }else if (ui->radioButton_guass->isChecked()) {
        filterType = 1;
    }

    int filterArea = 0;
    if (ui->radioButton_lowpass->isChecked()) {
        filterArea = 0;
    }else if (ui->radioButton_highpass->isChecked()) {
        filterArea = 1;
    }

    bool ok;
    double cutoff = ui->lineEdit->text().toDouble(&ok);
    qDebug() << ok << " " << cutoff << endl;
    if (ok) {
        emit confirmed(filterType, filterArea, cutoff);
    }else {
        qDebug() << "wrong cutoff" << endl;
        qDebug() << cutoff;
    }
    close();
}

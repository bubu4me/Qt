#ifndef DIALOGMEDIANFILTER_H
#define DIALOGMEDIANFILTER_H

#include <QDialog>

namespace Ui {
class DialogMedianFilter;
}

class DialogMedianFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMedianFilter(QWidget *parent = nullptr);
    ~DialogMedianFilter();

private:
    Ui::DialogMedianFilter *ui;

public slots:
    void accept();

signals:
    void confirmed(int imageType,int borderType,int row,int col);

};

#endif // DIALOGMEDIANFILTER_H

#ifndef DIALOGGAUSSFILTER_H
#define DIALOGGAUSSFILTER_H

#include <QDialog>

namespace Ui {
class DialogGaussFilter;
}

class DialogGaussFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGaussFilter(QWidget *parent = nullptr);
    ~DialogGaussFilter();

private:
    Ui::DialogGaussFilter *ui;

public slots:
    void accept();

signals:
    void confirmed(int imageType,int borderType,int row,double sigma);

};

#endif // DIALOGGAUSSFILTER_H

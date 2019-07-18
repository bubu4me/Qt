#ifndef DIALOGBILATERALFILTER_H
#define DIALOGBILATERALFILTER_H

#include <QDialog>

namespace Ui {
class DialogBilateralFilter;
}

class DialogBilateralFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBilateralFilter(QWidget *parent = nullptr);
    ~DialogBilateralFilter();

private:
    Ui::DialogBilateralFilter *ui;

public slots:
    void accept();

signals:
    void confirmed(int imageType, int borderType, int row, double sigma_filter, double sigma_image);
};

#endif // DIALOGBILATERALFILTER_H

#ifndef DIALOGLINEAR_H
#define DIALOGLINEAR_H

#include <QDialog>
#include <vector>

namespace Ui {
class DialogLinear;
}

class DialogLinear : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLinear(QWidget *parent = nullptr);
    ~DialogLinear();

public slots:
    void accept();

private:
    Ui::DialogLinear *ui;
    bool parseRow(QString,std::vector<double>&,int&);

signals:
    void confirmed(std::vector<double>,int,int,int);
};

#endif // DIALOGLINEAR_H

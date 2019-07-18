#ifndef DIALOGTRANSFORM_H
#define DIALOGTRANSFORM_H

#include <QDialog>

namespace Ui {
class DialogTransform;
}

class DialogTransform : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTransform(QWidget *parent = nullptr);
    ~DialogTransform();

private:
    Ui::DialogTransform *ui;

private slots:
    void accept();

signals:
    void confirmed(double);
};

#endif // DIALOGTRANSFORM_H

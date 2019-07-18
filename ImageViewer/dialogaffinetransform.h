#ifndef DIALOGAFFINETRANSFORM_H
#define DIALOGAFFINETRANSFORM_H

#include <QDialog>
#include <QMatrix>

namespace Ui {
class DialogAffineTransform;
}

class DialogAffineTransform : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAffineTransform(QWidget *parent = nullptr);
    ~DialogAffineTransform();

private:
    Ui::DialogAffineTransform *ui;

public slots:
    void accept();

signals:
    void confirmed(QMatrix);

};

#endif // DIALOGAFFINETRANSFORM_H

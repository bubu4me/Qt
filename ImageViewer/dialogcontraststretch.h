#ifndef DIALOGCONTRASTSTRETCH_H
#define DIALOGCONTRASTSTRETCH_H

#include <QDialog>

namespace Ui {
class DialogContrastStretch;
}

class DialogContrastStretch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContrastStretch(QWidget *parent = nullptr);
    ~DialogContrastStretch();

private:
    Ui::DialogContrastStretch *ui;

private slots:
    void accept();

signals:
    void confirmed(double m,double E);
};

#endif // DIALOGCONTRASTSTRETCH_H

#ifndef DIALOGMORPHOLOGY_H
#define DIALOGMORPHOLOGY_H

#include <QDialog>

namespace Ui {
class DialogMorphology;
}

class DialogMorphology : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMorphology(QWidget *parent = nullptr);
    ~DialogMorphology();

private:
    Ui::DialogMorphology *ui;

public slots:
    void accept();

signals:
    void confirmed(int imageType,int borderType,int morphologyType,int row,int col);

};

#endif // DIALOGMORPHOLOGY_H

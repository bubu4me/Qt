#ifndef DIALOGFREQUENCYDOMAINFILTER_H
#define DIALOGFREQUENCYDOMAINFILTER_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class DialogFrequencyDomainFilter;
}

class DialogFrequencyDomainFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFrequencyDomainFilter(QWidget *parent = nullptr);
    ~DialogFrequencyDomainFilter();

private:
    Ui::DialogFrequencyDomainFilter *ui;

public slots:
    void accept();

signals:
    void confirmed(int ,int ,double);
};

#endif // DIALOGFREQUENCYDOMAINFILTER_H

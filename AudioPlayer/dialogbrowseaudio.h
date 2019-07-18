#ifndef DIALOGBROWSEAUDIO_H
#define DIALOGBROWSEAUDIO_H

#include <QDialog>

namespace Ui {
class DialogBrowseAudio;
}

class DialogBrowseAudio : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBrowseAudio(QWidget *parent = nullptr);
    ~DialogBrowseAudio();

private:
    Ui::DialogBrowseAudio *ui;
};

#endif // DIALOGBROWSEAUDIO_H

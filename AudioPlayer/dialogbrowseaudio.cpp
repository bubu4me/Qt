#include "dialogbrowseaudio.h"
#include "ui_dialogbrowseaudio.h"

DialogBrowseAudio::DialogBrowseAudio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBrowseAudio)
{
    ui->setupUi(this);
}

DialogBrowseAudio::~DialogBrowseAudio()
{
    delete ui;
}

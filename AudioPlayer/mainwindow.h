#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AudioFile.h"
#include "fft.h"
#include "matrix.h"

#include <QMediaPlayer>
#include <QString>
#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QFileDialog>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QDialog>
#include <QMessageBox>


using std::vector;
using std::complex;
using std::real;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_browse_clicked();

    void on_button_load_clicked();

    void on_button_save_clicked();

    void on_button_play_clicked();

    void on_button_stop_clicked();

    void on_actionReverse_triggered();

    void on_actionFade_triggered();

    void on_actionResample_triggered();

    void on_actionOLA_triggered();

    void on_actionFFT_triggered();

    void updatePos(qint64);

    void updateDuration(qint64);

    void updateState(QMediaPlayer::State state);

    void on_actionNormalization_triggered();

    void on_actionAdjustByJson_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *media;
    QFileDialog *fd;
    QStringList audioList;
    AudioFile<double> audioFile;
    QFileInfo audioInfo;
    QString operationLogs;
    QString currentTime;
    uint sampleRate;
    int channels;
    int bitDepth;
    int samplesPerChannel;
    double lengthInSeconds;


    void init();
    void showLoadLog();
    void showLog(QString log);
    QJsonValue getJsonValue(QString key);
    vector<double> getHanningWnd(int windowSize);
    void ctrlBtn(bool isEnabled);
    QString getAudioTime(qint64 time);
    QDialog *dlgGrayscale;
    double** getWin(int channel);
    void setOLABuffer(double factor,AudioFile<double>::AudioBuffer ,int channel);
    void fourierTransform(const Matrix<double>& mat,int N,int channel);
    Matrix<int> nomarlized(Matrix<double> data, double min, double max);
    void saveMyImage(QImage img,int channel);
    void setVolumeInChannel(vector<double> volume);

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myframeitem.h"
#include "imageprocessor.h"
#include "dialogtransform.h"
#include "dialogcontraststretch.h"
#include "dialoglinear.h"
#include "dialoggaussfilter.h"
#include "dialogbilateralfilter.h"
#include "dialogmedianfilter.h"
#include "dialogmorphology.h"
#include "matrix.h"
#include "fft.h"
#include "dialogaffinetransform.h"
#include "dialogfrequencydomainfilter.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <vector>
#include <QMatrix>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void normalizeKernel(std::vector<double>& data);

private slots:
    void on_Btn_open_clicked();

    void on_Btn_reset_clicked();

    void on_Btn_pre_clicked();

    void on_Btn_next_clicked();

    void on_actionrgb2gray_triggered();

    void on_actionlog_triggered();

    void on_DialogTransform_confirmed(double c);

    void on_actionreverse_triggered();

    void on_DialogContrastStretch_confirmed(double m,double E);

    void on_actioncontrast_strectch_triggered();

    void on_actionhistogram_triggered();

    void on_actioncolor_histogram_triggered();

    void on_actionLinear_triggered();

    void on_DialogLinear_confirmed(std::vector<double>,int,int,int);

    void on_DialogGaussFilter_confirmed(int,int,int,double);

    void on_actionGauss_triggered();

    void on_actionBilateral_triggered();

    void on_DialogBilateralFilter_confirmed(int,int,int,double,double);

    void on_actionMedian_triggered();

    void on_DialogMedianFilter_confirmed(int,int,int,int);

    void on_actionMorphology_Operation_triggered();

    void on_DialogMorphology_confirmed(int,int,int,int,int);

    void on_actionshow_spectrum_triggered();

    void on_actionAffine_Transform_triggered();

    void on_DialogAffineTransform_confirmed(QMatrix);

    void on_actionFrequency_Domain_Filter_triggered();

    void on_DialogFrequencyDomainFilter_confirmed(int,int,double);

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

//    void on_graphicsView_customContextMenuRequested(const QPoint &pos);

//    void onTaskBoxContextMenuEvent();

private:
    Ui::MainWindow *ui;
    QFileDialog *fd;
    QGraphicsScene *scene;
    DialogTransform *dlglog;
    DialogContrastStretch *dlgCtrsStrch;
    DialogLinear *dlgLinear;
    DialogGaussFilter *dlgGaussFilter;
    DialogBilateralFilter *dlgBilateralFilter;
    DialogMedianFilter *dlgMedianFilter;
    DialogMorphology *dlgMorphology;
    DialogAffineTransform *dlgAffineTransform;
    DialogFrequencyDomainFilter *dlgFrequencyDomainFilter;

//    QGraphicsPixmapItem *pitem;
    MyFrameItem *pitem;
    int idx = -1; // 当前图片文件下标
    QFileInfoList imgs;

    void init();
    void showImage();
    void showImage(QImage img);
    void open();
    void reset();
    void repeat();
    QImage getCurrentImage() const;
    void judgeImgExistence();
    void ctrlBtnEnabled(bool isEnable);


};

#endif // MAINWINDOW_H

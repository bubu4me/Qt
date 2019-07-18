#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
/**
 * @title ImageViewer
 * @author 彭龙锐
 * @version 4.0
 * @date 2019.03.25
 * 功能：
 *  - Intensity Transformation
 *      -- rgb2gray 彩色图像转灰度图像
 *      -- reverse 颜色反相
 *      -- log 对数变换
 *      -- contrast stretch 对比度拉伸
 *      -- gray histogram 灰度图像直方图均衡
 *      -- color histogram 彩色图像直方图均衡
 *  - Spatial Filter
 *      -- Linear 线性滤波
 *          --- 包含补零、重复与镜像三种边缘填充的灰度图和彩色图的线性滤波操作
 *      -- Gauss Filter
 *      -- Bilateral Filter
 *      -- Median Filter
 *      -- Morphology Operation
 *          --- Dilate
 *          --- Corrode
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fd(new QFileDialog(this)),
    scene(nullptr),
    dlglog(new DialogTransform(this)),
    dlgCtrsStrch(new DialogContrastStretch(this)),
    dlgLinear(new DialogLinear(this)),
    dlgGaussFilter(new DialogGaussFilter(this)),
    dlgBilateralFilter(new DialogBilateralFilter(this)),
    dlgMedianFilter(new DialogMedianFilter(this)),
    dlgMorphology(new DialogMorphology(this)),
    dlgAffineTransform(new DialogAffineTransform(this)),
    dlgFrequencyDomainFilter(new DialogFrequencyDomainFilter(this)),
    pitem(nullptr)

{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete dlglog;
    delete dlgCtrsStrch;
    delete dlgLinear;
    delete dlgGaussFilter;
    delete dlgBilateralFilter;
    delete dlgMedianFilter;
    delete dlgMorphology;
    delete dlgAffineTransform;
    delete dlgFrequencyDomainFilter;
    delete fd;

    delete pitem;
    delete scene;

    delete ui;
}

void MainWindow::init()
{
    // dialog initial setting
    fd->setNameFilter("Image(*.bmp *.png *.jpg)");
    fd->setFileMode(QFileDialog::ExistingFile);

    // ui initial setting
    QFile uiFile(":/qss/Qss7.qss");
    uiFile.open(QFile::ReadOnly);
    qApp->setStyleSheet(QString::fromLatin1(uiFile.readAll()));
    ctrlBtnEnabled(false);
}

void MainWindow::on_Btn_open_clicked()
{
    qDebug() << "open";
    if (fd->exec()) {
        QStringList files = fd->selectedFiles();
        qDebug() << files[0];

        QDir dir = fd->directory();
        QStringList filters;
        filters << "*.bmp" << "*.jpg" << "*.png";
        imgs = dir.entryInfoList(filters);

        for (int i = 0; i < imgs.size(); i++) {
            if (imgs[i].absoluteFilePath() == files[0]) {
                idx = i;
                break;
            }
        }
        qDebug() << imgs.size();
        qDebug() << idx;
        showImage();
        ctrlBtnEnabled(true);
    }
}

void MainWindow::on_Btn_reset_clicked()
{
    reset();
}

void MainWindow::reset()
{
    if (pitem != nullptr) {
        QRectF rect = pitem->boundingRect();
        pitem->reset();
        scene->setSceneRect(rect);
        ui->graphicsView->fitInView(pitem, Qt::KeepAspectRatio);
        ui->graphicsView->centerOn(pitem);
    }
    qDebug() << "reset";
}

void MainWindow::on_Btn_pre_clicked()
{
    judgeImgExistence();
    if (idx != -1) {
        qDebug() << idx;
        if (idx == 0) {
            idx = imgs.size();
        }
        idx--;
        // show pre image
        showImage();
    }
}

void MainWindow::on_Btn_next_clicked()
{
    judgeImgExistence();
    if (idx != -1) {
        qDebug() << idx;
        if (idx == imgs.size() - 1) {
            idx = -1;
        }
        idx++;
        // show next image
        showImage();
    }
}

void MainWindow::showImage()
{
    QImage image(imgs[idx].absoluteFilePath());

    // 设置window title
    setWindowTitle(QFileInfo(imgs[idx]).fileName() + tr("-ImageViewer"));
    showImage(image);
}

void MainWindow::showImage(QImage img)
{
    if (pitem) {
        delete pitem;
        pitem = nullptr;
    }
    if (scene) {
        delete scene;
        scene = nullptr;
    }

    scene = new QGraphicsScene(this);
    QImage image(imgs[idx].absoluteFilePath());
    pitem = new MyFrameItem();
    pitem->setPixmap(QPixmap::fromImage(img));

    scene->addItem(pitem);
    ui->graphicsView->setScene(scene);
    reset();// 保证一打开就是自适应的
}

// RGB to Gray
void MainWindow::on_actionrgb2gray_triggered()
{
    judgeImgExistence();
    qDebug() << "rgb2gray";
    // get current image
    if (!pitem) return;
    QImage image = getCurrentImage();
    // rgb2gray
    image = ImageProcessor::rgb2gray(image);
    showImage(image);
}

// 对数变换
void MainWindow::on_actionlog_triggered()
{
    judgeImgExistence();
    qDebug() << "log dialpg";
    dlglog->exec();
}
void MainWindow::on_DialogTransform_confirmed(double c)
{
    qDebug() << "C: " << c;
    // get current image
    if (!pitem) return;
    QImage image = getCurrentImage();
    // 灰度变换
    image = ImageProcessor::logTransformation(image, c);
    showImage(image);
}

QImage MainWindow::getCurrentImage() const
{
    QList<QGraphicsItem*> items = ui->graphicsView->scene()->items();
    QGraphicsPixmapItem *item = (QGraphicsPixmapItem*)items.at(0);

    return item->pixmap().toImage();
}

// 图像反相
void MainWindow::on_actionreverse_triggered()
{
    judgeImgExistence();
    qDebug() << "reverse";
    if (!pitem) return;
    QImage image = getCurrentImage();
    // 图像反转
    image = ImageProcessor::imageReverse(image);
    showImage(image);
}

// 对比度拉伸
void MainWindow::on_actioncontrast_strectch_triggered()
{
    judgeImgExistence();
    qDebug() << "contrast_strectch dialpg";
    dlgCtrsStrch->exec();
}
void MainWindow::on_DialogContrastStretch_confirmed(double m, double E)
{
    qDebug() << "m: " << m << " E: " << E;
    // get current image
    if (!pitem) return;
    QImage image = getCurrentImage();
    // 对比度拉伸
    image = ImageProcessor::contrastStretch(image, m, E);
    showImage(image);
}

// 灰度直方图均衡
void MainWindow::on_actionhistogram_triggered()
{
    judgeImgExistence();
    qDebug() << "gray histogram";
    if (!pitem) return;
    QImage image = getCurrentImage();
    // 灰度直方图均衡
    image = ImageProcessor::grayHistogram(image);
    showImage(image);
}

// 彩色直方图均衡
void MainWindow::on_actioncolor_histogram_triggered()
{
    judgeImgExistence();
    qDebug() << "color histogram";
    if (!pitem) return;
    QImage image = getCurrentImage();
    // 彩色直方图均衡
    qDebug() << "彩色直方图均衡";
    image = ImageProcessor::colorHistogram(image);
    showImage(image);
}

// 线性滤波
void MainWindow::on_actionLinear_triggered()
{
    judgeImgExistence();
    qDebug() << "Linear";
    dlgLinear->exec();
}
void MainWindow::on_DialogLinear_confirmed(std::vector<double> data, int nCol, int imageType, int borderType)
{
    qDebug() << "linear filter";
    qDebug() << "data" << data << "nCol" << nCol;
    QImage image = getCurrentImage();
    normalizeKernel(data);
    qDebug() << "imageType" << imageType;
    qDebug() << "borderType" << borderType;
    image = ImageProcessor::linearFilter(image, data, nCol, imageType, borderType);
    qDebug() << image.size();
    showImage(image);
}
// 归一化
void MainWindow::normalizeKernel(std::vector<double>& data)
{
    double sum = 0;

    for (int i = 0; i < data.size(); i++) {
        sum += data[i];
    }
    if (sum == 0.0 || sum == 1.0) {
        return;
    }
    for (int i = 0; i < data.size(); i++) {
        data[i] /= sum;
    }
}

// 高斯滤波
void MainWindow::on_actionGauss_triggered()
{
    judgeImgExistence();
    qDebug() << "Gauss Filter";
    dlgGaussFilter->exec();
}
void MainWindow::on_DialogGaussFilter_confirmed(int imageType, int borderType, int row, double sigma)
{
    qDebug() << "Gauss filter";
    qDebug() << "imageType" << imageType;
    qDebug() << "borderType" << borderType;
    qDebug() << "row" << row << "sigma" << sigma;
    QImage image = getCurrentImage();
    image = ImageProcessor::gaussFilter(image, row, sigma, imageType, borderType);
    qDebug() << image.size();
    showImage(image);
}

// 双边滤波
void MainWindow::on_actionBilateral_triggered()
{
    judgeImgExistence();
    qDebug() << "Bilateral Filter";
    dlgBilateralFilter->exec();
}
void MainWindow::on_DialogBilateralFilter_confirmed(int imageType, int borderType, int row, double sigma_filter, double sigma_image)
{
    qDebug() << "Bilateral filter";
    qDebug() << "imageType" << imageType;
    qDebug() << "borderType" << borderType;
    qDebug() << "row" << row << "sigma_filter" << sigma_filter << "sigma_image" << sigma_image;
    QImage image = getCurrentImage();
    image = ImageProcessor::bilateralFilter(image, row, sigma_filter, sigma_image, imageType, borderType);
    qDebug() << image.size();
    showImage(image);
}

// 中值滤波
void MainWindow::on_actionMedian_triggered()
{
    judgeImgExistence();
    qDebug() << "Median Filter";
    dlgMedianFilter->exec();
}
void MainWindow::on_DialogMedianFilter_confirmed(int imageType, int borderType, int row, int col)
{
    qDebug() << "Median filter";
    qDebug() << "imageType" << imageType;
    qDebug() << "borderType" << borderType;
    qDebug() << "row" << row << "col" << col;
    QImage image = getCurrentImage();
    image = ImageProcessor::medianFilter(image, row, col, imageType, borderType);
    qDebug() << image.size();
    showImage(image);
}

// 图像形态学处理 膨胀或者腐蚀
void MainWindow::on_actionMorphology_Operation_triggered()
{
    judgeImgExistence();
    qDebug() << "Morphology Operation";
    dlgMorphology->exec();
}
void MainWindow::on_DialogMorphology_confirmed(int imageType, int borderType, int type, int row, int col)
{
    qDebug() << "Morphology Operation";
    qDebug() << "borderType" << type;
    qDebug() << "row" << row << "col" << col;
    QImage image = getCurrentImage();
    image = ImageProcessor::morphologyOperation(image, row, col, imageType, borderType, type);
    qDebug() << image.size();
    showImage(image);
}

// 显示当前图片的频谱图
void MainWindow::on_actionshow_spectrum_triggered()
{
    judgeImgExistence();
    qDebug() << "show spectrum";
    QImage myImg = ImageProcessor::rgb2gray(fftImg(getCurrentImage()));
    qDebug() << "size: " << myImg.size();
    showImage(myImg);
}

// 仿射变换
void MainWindow::on_actionAffine_Transform_triggered()
{
    judgeImgExistence();
    qDebug() << "Affine Transform trigger";
    dlgAffineTransform->exec();
}
void MainWindow::on_DialogAffineTransform_confirmed(QMatrix mat)
{
    // TODO 已经改回来
    qDebug() << "Affine Transform confirmed";
    qDebug() << "emit mat" << mat;
    QImage img = ImageProcessor::affineTransform(getCurrentImage(), mat);
    showImage(img);
}

// 频域滤波
void MainWindow::on_actionFrequency_Domain_Filter_triggered()
{
    judgeImgExistence();
    qDebug() << "Frequency Domain Filter";
    dlgFrequencyDomainFilter->exec();
}

void MainWindow::on_DialogFrequencyDomainFilter_confirmed(int filterType, int filterArea, double cutoff)
{
    qDebug() << "Frequency Domain Filter";
    qDebug() << "emit filterType" << filterType << endl;
    qDebug() << "emit filterArea" << filterArea << endl;
    qDebug() << "emit cutoff" << cutoff << endl;
    QImage img = ImageProcessor::frequencyDomainFilter(getCurrentImage(), filterType, filterArea, cutoff);
    showImage(img);
}

// 右键菜单-保存-另存为-取消
/*
   void MainWindow::on_graphicsView_customContextMenuRequested(const QPoint &pos)
   {
    if (!pitem) {
        qDebug() << "There is no picture!!!";
        return;
    }
    QMenu *pMenu = new QMenu(this);
    QAction *pSave = new QAction(tr("Save"), this);
    QAction *pSaveAs = new QAction(tr("Save As"), this);
    QAction *pCancel = new QAction(tr("Canel"), this);
    pSave->setData(1);
    pSaveAs->setData(2);
    pCancel->setData(3);
    pMenu->addAction(pSave);
    pMenu->addAction(pSaveAs);
    pMenu->addAction(pCancel);
    connect(pSave, SIGNAL(triggered()), this, SLOT(onTaskBoxContextMenuEvent()));
    connect(pSaveAs, SIGNAL(triggered()), this, SLOT(onTaskBoxContextMenuEvent()));
    connect(pCancel, SIGNAL(triggered()), this, SLOT(onTaskBoxContextMenuEvent()));
    pMenu->exec(cursor().pos());
    //删除内存
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
   }

   void MainWindow::onTaskBoxContextMenuEvent()
   {
    QAction *pEven = qobject_cast<QAction *>(this->sender()); //this->sender()就是发信号者 QAction
    int iType = pEven->data().toInt();
    QImage img = getCurrentImage();

    if (iType == 1) {
        qDebug() << "path: " << imgs[idx].absoluteFilePath() << endl;
        img.save(imgs[idx].absoluteFilePath(), "JPG", -1);
    }

    if (iType == 2) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("*.jpg;; *.png;; *.bmp;; *.tif;; *.GIF"));
        if (filename.isEmpty()) {
            return;
        }else {
            if (!(img.save(filename))) { //保存图像
                QMessageBox::information(this, tr("Failed to save the image"), tr("Failed to save the image!"));
                return;
            }
        }
    }
    if (iType == 3) {
        qDebug() << "Cancel";
    }
   }
 */

void MainWindow::on_actionSave_triggered()
{
    judgeImgExistence();

    qDebug() << "path: " << imgs[idx].absoluteFilePath() << endl;
    getCurrentImage().save(imgs[idx].absoluteFilePath(), "JPG", -1);
}

void MainWindow::on_actionSave_As_triggered()
{
    judgeImgExistence();

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("*.jpg;; *.png;; *.bmp;; *.tif;; *.GIF"));
    if (filename.isEmpty()) {
        return;
    }else {
        if (!(getCurrentImage().save(filename))) { //保存图像
            QMessageBox::information(this, tr("Failed to save the image"), tr("Failed to save the image!"));
            return;
        }
    }
}

void MainWindow::judgeImgExistence()
{
    if (!pitem) {
        qDebug() << "No Pitcure Is Opened";
        return;
    }
}

void MainWindow::ctrlBtnEnabled(bool isEnable)
{
    ui->actionAffine_Transform->setEnabled(isEnable);
    ui->actionBilateral->setEnabled(isEnable);
    ui->actionFrequency_Domain_Filter->setEnabled(isEnable);
    ui->actionGauss->setEnabled(isEnable);
    ui->actionLinear->setEnabled(isEnable);
    ui->actionMedian->setEnabled(isEnable);
    ui->actionMorphology_Operation->setEnabled(isEnable);
    ui->actionSave->setEnabled(isEnable);
    ui->actionSave_As->setEnabled(isEnable);
    ui->actioncolor_histogram->setEnabled(isEnable);
    ui->actioncontrast_strectch->setEnabled(isEnable);
    ui->actionhistogram->setEnabled(isEnable);
    ui->actionlog->setEnabled(isEnable);
    ui->actionreverse->setEnabled(isEnable);
    ui->actionrgb2gray->setEnabled(isEnable);
    ui->actionshow_spectrum->setEnabled(isEnable);

    ui->Btn_next->setEnabled(isEnable);
    ui->Btn_pre->setEnabled(isEnable);
    ui->Btn_reset->setEnabled(isEnable);
}

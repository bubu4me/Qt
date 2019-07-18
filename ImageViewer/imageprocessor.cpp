#include "imageprocessor.h"
#include "mainwindow.h"
#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <algorithm>

ImageProcessor::ImageProcessor()
{

}

QImage ImageProcessor::rgb2gray(const QImage& img)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int red = qRed(rgb);
            int green = qGreen(rgb);
            int blue = qBlue(rgb);
            int gray = (red + green + blue) / 3; // 第一种变换
            ret.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    return ret;
}

QImage ImageProcessor::imageReverse(const QImage& img)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int red = 255 - qRed(rgb);
            int green = 255 - qGreen(rgb);
            int blue = 255 - qBlue(rgb);
            ret.setPixel(i, j, qRgb(red, green, blue));
        }
    }
    return ret;
}

QImage ImageProcessor::logTransformation(const QImage& img, double c)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int gray = qGray(rgb);
            int newPixel = int (c * log(1 + gray / 255.0) * 255);
            if (newPixel > 255) newPixel = 255;
            ret.setPixel(i, j, qRgb(newPixel, newPixel, newPixel));
        }
    }
    return ret;
}

QImage ImageProcessor::contrastStretch(const QImage& img, double m, double E)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int gray = qGray(rgb);
            int newPixel = int(1 / (1 + qPow(m / gray, E)) * 255);
            if (newPixel > 255) newPixel = 255;
            ret.setPixel(i, j, qRgb(newPixel, newPixel, newPixel));
        }
    }
    return ret;
}

QImage ImageProcessor::grayHistogram(const QImage& img)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();
    int sum[256];
    int res[256];

    // 数组初始化
    memset(sum, 0, sizeof(sum));
    memset(res, 0, sizeof(res));

    // 统计每个像素出现的次数
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int gray = qGray(rgb);
            sum[gray]++;
        }
    }

    int colorSum = 0;

    // 完成映射：i->res[i]
    for (int i = 0; i < 256; i++) {
        colorSum += sum[i];                                     // 累积灰度值次数
        res[i] = int(colorSum * 255.0 / (w * h)  + 0.5);        // 计算映射值
        if (res[i] > 255) res[i] = 255;
    }

    // 将映射好的值放入矩阵
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            int gray = qGray(rgb);
            ret.setPixel(i, j, qRgb(res[gray], res[gray], res[gray]));
        }
    }
    return ret;
}

QImage ImageProcessor::colorHistogram(const QImage& img)
{
    QImage ret(img);
    int w = ret.width();
    int h = ret.height();

    QVector<QVector<float> > H(w, QVector<float>(h));
    QVector<QVector<float> > S(w, QVector<float>(h));
    QVector<QVector<float> > I(w, QVector<float>(h));

    // RGB->HSI
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = img.pixel(i, j);
            float var_R = qRed(rgb) / 255.0f;
            float var_G = qGreen(rgb) / 255.0f;
            float var_B = qBlue(rgb) / 255.0f;

            float var_Min = qMin(qMin(var_R, var_G), var_B);
            float var_Max = qMax(qMax(var_R, var_G), var_B);
            float del_Max = var_Max - var_Min;

            // I 范围:[0,1]
            I[i][j] = (var_Max + var_Min) / 2;
            int i_tmp = int(I[i][j] * 255);
            ret.setPixel(i, j, qRgb(i_tmp, i_tmp, i_tmp));

            // H 范围:[0,1]
            // S 范围:[0,1]
            if (del_Max <= 0.001f) {
                H[i][j] = 0;
                S[i][j] = 0;
            }else{
                if (I[i][j] < 0.5f) S[i][j] = del_Max / (var_Max + var_Min);
                else S[i][j] = del_Max / (2 - var_Max - var_Min);

                float del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
                float del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
                float del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max;

                if (qAbs(var_R - var_Max) <= 0.001f) H[i][j] = del_B - del_G;
                else if (qAbs(var_G - var_Max) <= 0.001f) H[i][j] = (1.0f / 3) + del_R + del_B;
                else if ( qAbs(var_B - var_Max) <= 0.001f ) H[i][j] = ( 2.0f / 3 ) + del_G - del_R;

                if (H[i][j] < 0) H[i][j] += 1;
                if (H[i][j] > 1) H[i][j] -= 1;
            }
        }
    }

    // 对I图像进行直方图均衡化,并传递回I数组
    ret = grayHistogram(ret);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb rgb = ret.pixel(i, j);
            I[i][j] = qGray(rgb);
        }
    }

    // HSI->RGB
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            I[i][j] /= 255;
            if (S[i][j] <= 0.001f) {
                int tmp = int(I[i][j] * 255);
                ret.setPixel(i, j, qRgb(tmp, tmp, tmp));
            }else{
                float var_2 = 0.0f;
                if (I[i][j] < 0.5f) {
                    var_2 = I[i][j] * (1 + S[i][j]);
                }else{
                    var_2 = (I[i][j] + S[i][j]) - (I[i][j] * S[i][j]);
                }
                float var_1 = 2 * I[i][j] - var_2;

                int R = int(255 * Hue2RGB(var_1, var_2, H[i][j] + (1.0f / 3)));
                int G = int(255 * Hue2RGB(var_1, var_2, H[i][j] ));
                int B = int(255 * Hue2RGB(var_1, var_2, H[i][j] - (1.0f / 3)));
                ret.setPixel(i, j, qRgb(R, G, B));
            }
        }
    }
    return ret;
}

float ImageProcessor::Hue2RGB(float v1, float v2, float vH)
{
    if (vH < 0.0f) {
        vH += 1;
    }
    if (vH > 1.0f) {
        vH -= 1;
    }
    if ((6 * vH) < 1.0f) {
        return (v1 + (v2 - v1) * 6 * vH);
    }
    if ((2 * vH) < 1.0f) {
        return v2;
    }
    if ((3 * vH) < 2.0f) {
        return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);
    }
    return v1;
}

// 在灰度图上做滤波
void ImageProcessor::filterOnGray(int row, int col, QImage ret, QImage& resultImage, vector<double> filterData)
{
    int centerRow = row / 2;
    int centerCol = col / 2;
    int w = ret.width();
    int h = ret.height();

    for (int i = centerCol; i < w - centerCol; i++) {
        for (int j = centerRow; j < h - centerRow; j++) {
            QRect rect(i - centerCol, j - centerRow, col, row);
            QImage patch = ret.copy(rect);
            double sum = 0;
            for (int k = 0; k < filterData.size(); k++) {
                sum += filterData[k] * qGray(patch.pixel(k / col, k % col));
            }
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            resultImage.setPixel(i - centerCol, j - centerRow, qRgb(sum, sum, sum));
        }
    }
}

// 在彩色图上做滤波
void ImageProcessor::filterOnRGB(int row, int col, QImage ret, QImage& resultImage, vector<double> filterData)
{
    int centerRow = row / 2;
    int centerCol = col / 2;
    int w = ret.width();
    int h = ret.height();

    for (int i = centerCol; i < w - centerCol; i++) {
        for (int j = centerRow; j < h - centerRow; j++) {
            QRect rect(i - centerCol, j - centerRow, col, row);
            QImage patch = ret.copy(rect);
            double sumR = 0;
            double sumG = 0;
            double sumB = 0;
            for (int k = 0; k < filterData.size(); k++) {
                sumR += filterData[k] * qRed(patch.pixel(k / col, k % col));
                sumG += filterData[k] * qGreen(patch.pixel(k / col, k % col));
                sumB += filterData[k] * qBlue(patch.pixel(k / col, k % col));
            }
            if (sumR < 0) sumR = 0;
            if (sumG < 0) sumG = 0;
            if (sumB < 0) sumB = 0;
            if (sumR > 255) sumR = 255;
            if (sumG > 255) sumG = 255;
            if (sumB > 255) sumB = 255;
            resultImage.setPixel(i - centerCol, j - centerRow, qRgb(sumR, sumG, sumB));
        }
    }
}

/**
 * @brief ImageProcessor::linearFilter
 * @param img   Image to process
 * @param data  Mask matrix
 * @param nCol  Mask matrix column
 * @param imageType
 *      0 for gray
 *      1 for RGB
 * @param type
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @return processed image
 */
QImage ImageProcessor::linearFilter(const QImage& img, std::vector<double> data, int nCol, int imageType, int borderType)
{
    // 异常处理
    if (img.isNull() || data.empty()) {
        qDebug() << "wrong image or wrong data";
        return img;
    }
    if (nCol <= 0) {
        qDebug() << "nCol <= 0";
        return img;
    }
    int nRow = int(data.size()) / nCol;
    qDebug() << "nRow:" << nRow;
    if (nRow % 2 == 0 || nCol % 2 == 0) {
        qDebug() << "nCol and nRow should be odd";
        return img;
    }
    if (imageType != 0 && imageType != 1) {
        qDebug() << "wrong image type";
        return img;
    }
    if (borderType < 0 || borderType > 2) {
        qDebug() << "no such border type";
        return img;
    }
    QImage ret = getBorderType(img, nRow, nCol, borderType);
    QImage resultImage(img);

    if (imageType == 0) {
        filterOnGray(nRow, nCol, ret, resultImage, data);       // 灰度图像线性滤波
    }else if (imageType == 1) {
        filterOnRGB(nRow, nCol, ret, resultImage, data);        // 彩色图像线性滤波
    }
    return resultImage;
}

/**
 * @brief ImageProcessor::borderType
 * @param img: Image to process
 * @param nRow: Mask matrix's row
 * @param nCol: Mask matrix's column
 * @param type:
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @return processed image
 */
QImage ImageProcessor::getBorderType(const QImage& img, int nRow, int nCol, int borderType)
{
    // 异常处理
    if (img.isNull() || nRow <= 0 || nCol <= 0) {
        qDebug() << "img is Null || nRow <= 0 || nCol <= 0";
        return img;
    }
    if (borderType > 2 || borderType < 0) {
        qDebug() << "no such type";
        return img;
    }
    int w = img.width();
    int h = img.height();
    QPainter painter;
    // 补零或者重复
    if (borderType == 0 || borderType == 1) {
        QImage ret(w + nCol - 1, h + nRow - 1, QImage::Format_RGB32);
        ret.fill(0);// 这步很重要，否则初始图像不是全黑的
        // 补零 已成功实现
        painter.begin(&ret);
        painter.drawImage(nCol / 2, nRow / 2,  img);// x=nCol/2, y=nRow/2
        if (borderType == 0) {
            qDebug() << "补零" << ret.size();
            painter.end();
            return ret;
        }
        // 重复 已成功实现
        else{
            // 重复对角像素
            unsigned int diagonalPixel_northwest = img.pixel(0, 0);// x=0,y=0
            unsigned int diagonalPixel_northeast = img.pixel(w - 1, 0);
            unsigned int diagonalPixel_southwest = img.pixel(0, h - 1);
            unsigned int diagonalPixel_southeast = img.pixel(w - 1, h - 1);
            for (int i = 0; i < nCol / 2; i++) {
                for (int j = 0; j <  nRow / 2; j++) {
                    // 左上角
                    ret.setPixel(i, j, diagonalPixel_northwest);
                    ret.setPixel(i + w + nCol / 2, j, diagonalPixel_northeast);
                    ret.setPixel(i, j + h + nRow / 2, diagonalPixel_southwest);
                    ret.setPixel(i + w + nCol / 2, j + h + nRow / 2, diagonalPixel_southeast);
                }
                // 重复左右像素
                for (int k = 0; k < h; k++) {
                    unsigned int diagonalPixel_west = ret.pixel(nCol / 2, k + nRow / 2);
                    ret.setPixel(i, k + nRow / 2, diagonalPixel_west);
                    unsigned int diagonalPixel_east = ret.pixel(nCol / 2 + w - 1, k + nRow / 2);
                    ret.setPixel(i + w + nCol / 2, k + nRow / 2, diagonalPixel_east);
                }
            }
            // 重复上下像素
            for (int i = 0; i < nRow / 2; i++) {
                for (int j = 0; j < w; j++) {
                    unsigned int diagonalPixel_north = ret.pixel(j + nCol / 2, nRow / 2);
                    ret.setPixel(j + nCol / 2, i, diagonalPixel_north);
                    unsigned int diagonalPixel_south = ret.pixel(j + nCol / 2, nRow / 2 + h - 1);
                    ret.setPixel(j + nCol / 2, i + h + nRow / 2, diagonalPixel_south);
                }
            }
            qDebug() << "重复" << ret.size();
            painter.end();
            return ret;
        }
    }
    // 镜像 已成功实现
    else if (borderType == 2) {
        QImage ret(3 * w, 3 * h, QImage::Format_RGB32);
        painter.begin(&ret);
        QImage ret1 = img.mirrored(true, true);         // 对角镜像
        QImage ret2 = img.mirrored(false, true);        // 垂直镜像
        QImage ret3 = img.mirrored(true, false);        // 水平镜像
        // 对角
        painter.drawImage(0, 0, ret1);
        painter.drawImage(2 * w, 0, ret1);
        painter.drawImage(0, 2 * h, ret1);
        painter.drawImage(2 * w, 2 * h, ret1);
        // 垂直
        painter.drawImage(w, 0, ret2);
        painter.drawImage(w, 2 * h, ret2);
        // 水平
        painter.drawImage(0, h, ret3);
        painter.drawImage(2 * w, h, ret3);
        // 中间
        painter.drawImage(w, h, img);
        painter.end();
        QRect rect(w - nRow / 2, h - nCol / 2, w + nRow - 1, h + nCol - 1);// 截取图像
        ret = ret.copy(rect);
        return ret;
    }
    qDebug() << "返回原图";
    return img;
}

// 高斯滤波器
void ImageProcessor::setGaussData(vector<double>& gaussData, int row, double sigma)
{
    int center = row / 2;
    double den = 2 * sigma * sigma;// 分母

    for (int i = 0; i < row; i++) {
        double index1 = (i - center) * (i - center) / den;
        for (int j = 0; j < row; j++) {
            double index2 = (j - center) * (j - center) / den;
            double k = qExp(-(index1 + index2));
            gaussData.push_back(k);
        }
    }
    MainWindow::normalizeKernel(gaussData);// 归一化
}
/**
 * @brief ImageProcessor::gaussFilter
 * @param img -- image to process
 * @param row -- filter matrix's row
 * @param sigma -- standard deviation
 * @param imageType
 *      0 for gray
 *      1 for RGB
 * @param type
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @return processed image
 */
QImage ImageProcessor::gaussFilter(const QImage& img, int row, double sigma, int imageType, int borderType)
{
    // 异常处理
    if (img.isNull() || row <= 0 || sigma == 0.0) {
        qDebug() << "img.isNull() || row <= 0 || sigma == 0.0";
        return img;
    }
    if (imageType != 0 && imageType != 1) {
        qDebug() << "wrong image type";
        return img;
    }
    if (borderType < 0 || borderType > 2) {
        qDebug() << "no such border type";
        return img;
    }

    QImage ret = getBorderType(img, row, row, borderType);
    QImage resultImage(img);
    // 获取高斯滤波器
    std::vector<double> gaussData;
    setGaussData(gaussData, row, sigma);
    qDebug() << "gaussData" << gaussData;

    if (imageType == 0) {
        filterOnGray(row, row, ret, resultImage, gaussData);    // 灰度图像高斯滤波
    }else if (imageType == 1) {
        filterOnRGB(row, row, ret, resultImage, gaussData);     // 彩色图像高斯滤波
    }
    return resultImage;
}

// 双边滤波
void ImageProcessor::setBilateralData(QImage patch, vector<double>& bilateralData,
                      vector<double> index3, int row, double sigma_filter)
{
    int center = row / 2;
    double den_filter = 2 * sigma_filter * sigma_filter;

    for (int m = 0; m < row; m++) {
        double index1 = (m - center) * (m - center) / den_filter;
        double ijGray = qGray(patch.pixel(m / row, m % row));
        double centerGray = qGray(patch.pixel( center, center));
        int i = abs(ijGray - centerGray);
        for (int n = 0; n < row; n++) {
            double index2 = (n - center) * (n - center) / den_filter;
            double k = qExp(-(index1 + index2 + index3[i]));
            bilateralData.push_back(k);
        }
    }
}
/**
 * @brief ImageProcessor::bilateralFilter
 * @param img -- image to process
 * @param row -- filter matrix's row
 * @param sigma_filter -- standard deviation
 * @param sigma_image -- standard deviatipon based on image
 * @param imageType
 *      0 for gray
 *      1 for RGB(Not recommand)
 * @param type
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @return processed image
 */
QImage ImageProcessor::bilateralFilter(const QImage& img, int row, double sigma_filter,
                       double sigma_image, int imageType, int borderType)
{
    // 异常处理
    if (img.isNull() || row <= 0 || sigma_filter == 0.0 || sigma_image == 0.0) {
        qDebug() << "img.isNull() || row <= 0 || sigma_filter == 0.0 || sigma_image == 0.0";
        return img;
    }
    if (imageType != 0 && imageType != 1) {
        qDebug() << "wrong image type";
        return img;
    }
    if (borderType < 0 || borderType > 2) {
        qDebug() << "no such border type";
        return img;
    }

    QImage ret = getBorderType(img, row, row, borderType);
    QImage resultImage(img);
    std::vector<double> bilateralData;
    int center = row / 2;
    int w = ret.width();
    int h = ret.height();

    // 计算双边滤波第二个e为底数的值，并建立索引
    double den_image = 2 * sigma_image * sigma_image;
    std::vector<double> index3;
    for (int i = 0; i < 256; i++) {
        index3.push_back(i * i / den_image);
    }

    // 灰度图像双边滤波
    if (imageType == 0) {
        for (int i = center; i < w - center; i++) {
            for (int j = center; j < h - center; j++) {
                QRect rect(i - center, j - center, row, row);
                QImage patch = ret.copy(rect);
                setBilateralData(patch, bilateralData, index3, row, sigma_filter);      // 计算双边滤波器
                MainWindow::normalizeKernel(bilateralData);                             // 归一化
                double sum = 0;
                for (int k = 0; k < bilateralData.size(); k++) {
                    sum += bilateralData[k] * qGray(patch.pixel(k / row, k % row));
                }
                if (sum < 0) sum = 0;
                if (sum > 255) sum = 255;
                resultImage.setPixel(i - center, j - center, qRgb(sum, sum, sum));
                bilateralData.clear();// 每次运算完成必须清空
            }
        }
    }
    // 彩色图像双边滤波
    else if (imageType == 1) {
        for (int i = center; i < w - center; i++) {
            for (int j = center; j < h - center; j++) {
                QRect rect(i - center, j - center, row, row);
                QImage patch = ret.copy(rect);
                setBilateralData(patch, bilateralData, index3, row, sigma_filter);      // 计算双边滤波器
                MainWindow::normalizeKernel(bilateralData);                             // 归一化
                double sumR = 0;
                double sumG = 0;
                double sumB = 0;
                for (int k = 0; k < bilateralData.size(); k++) {
                    sumR += bilateralData[k] * qRed(patch.pixel(k / row, k % row));
                    sumG += bilateralData[k] * qGreen(patch.pixel(k / row, k % row));
                    sumB += bilateralData[k] * qBlue(patch.pixel(k / row, k % row));
                }
                if (sumR < 0) sumR = 0;
                if (sumG < 0) sumG = 0;
                if (sumB < 0) sumB = 0;
                if (sumR > 255) sumR = 255;
                if (sumG > 255) sumG = 255;
                if (sumB > 255) sumB = 255;
                resultImage.setPixel(i - center, j - center, qRgb(sumR, sumG, sumB));
                bilateralData.clear();// 每次运算完成必须清空
            }
        }
    }
    return resultImage;
}

/**
 * @brief ImageProcessor::medianFilter
 * @param img -- image to process
 * @param row -- filter matrix's row
 * @param col -- filter matrix's col
 * @param imageType
 *      0 for gray
 *      1 for RGB
 * @param type
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @return processed image
 */
QImage ImageProcessor::medianFilter(const QImage& img, int row, int col, int imageType, int borderType)
{
    // 异常处理
    if (img.isNull() || row <= 0 || col <= 0) {
        qDebug() << "img.isNull() || row <= 0 || col <= 0";
        return img;
    }
    if (row % 2 == 0 || col % 2 == 0) {
        qDebug() << "row or col should be odd";
        return img;
    }
    if (imageType != 0 && imageType != 1) {
        qDebug() << "wrong image type";
        return img;
    }
    if (borderType < 0 || borderType > 2) {
        qDebug() << "no such border type";
        return img;
    }

    QImage ret = getBorderType(img, row, col, borderType);
    QImage resultImage(img);
    int centerRow = row / 2;
    int centerCol = col / 2;
    int filterSize = row * col;
    int w = ret.width();
    int h = ret.height();

    // 灰度图像中值滤波
    if (imageType == 0) {
        vector<int> medianData;
        for (int i = centerCol; i < w - centerCol; i++) {
            for (int j = centerRow; j < h - centerRow; j++) {
                QRect rect(i - centerCol, j - centerRow, col, row);
                QImage patch = ret.copy(rect);
                for (int k = 0; k < filterSize; k++) {
                    medianData.push_back(qGray(patch.pixel(k / col, k % col)));
                }
                std::sort(medianData.begin(), medianData.end());        // 排序
                int midGray = medianData.at(filterSize / 2);            // 获取中值
                resultImage.setPixel(i - centerCol, j - centerRow, qRgb(midGray, midGray, midGray));
                medianData.clear();
            }
        }
    }
    // 彩色图像中值滤波
    else if (imageType == 1) {
        vector<int> medianDataR;
        vector<int> medianDataG;
        vector<int> medianDataB;
        for (int i = centerCol; i < w - centerCol; i++) {
            for (int j = centerRow; j < h - centerRow; j++) {
                QRect rect(i - centerCol, j - centerRow, col, row);
                QImage patch = ret.copy(rect);
                for (int k = 0; k < filterSize; k++) {
                    medianDataR.push_back( qRed(patch.pixel(k / col, k % col)));
                    medianDataG.push_back( qGreen(patch.pixel(k / col, k % col)));
                    medianDataB.push_back( qBlue(patch.pixel(k / col, k % col)));
                }
                std::sort(medianDataR.begin(), medianDataR.end());      // 排序
                std::sort(medianDataG.begin(), medianDataG.end());      // 排序
                std::sort(medianDataB.begin(), medianDataB.end());      // 排序
                int midR = medianDataR.at(filterSize / 2);              // 获取中值
                int midG = medianDataG.at(filterSize / 2);              // 获取中值
                int midB = medianDataB.at(filterSize / 2);              // 获取中值
                resultImage.setPixel(i - centerCol, j - centerRow, qRgb(midR, midG, midB));
                medianDataR.clear();
                medianDataG.clear();
                medianDataB.clear();
            }
        }
    }
    return resultImage;
}

/**
 * @brief ImageProcessor::morphologyOperation
 * @param img -- image to process
 * @param row -- filter matrix's row
 * @param col -- filter matrix's col
 * @param imageType
 *      0 for gray
 *      1 for RGB
 * @param type
 *      0 for filling zeros
 *      1 for repetition
 *      2 for mirror
 * @param morphologyType
 *      0 for dilate
 *      1 for corrode
 * @return
 */
QImage ImageProcessor::morphologyOperation(const QImage& img, int row, int col, int imageType,
                       int borderType, int morphologyType)
{
    // 异常处理
    if (img.isNull() || row <= 0 || col <= 0) {
        qDebug() << "img.isNull() || row <= 0 || col <= 0";
        return img;
    }
    if (row % 2 == 0 || col % 2 == 0) {
        qDebug() << "row or col should be odd";
        return img;
    }
    if (imageType != 0 && imageType != 1) {
        qDebug() << "wrong image type";
        return img;
    }
    if (borderType < 0 || borderType > 2) {
        qDebug() << "no such border type";
        return img;
    }
    if (morphologyType != 0 && morphologyType != 1) {
        qDebug() << "no such type";
        return img;
    }

    QImage ret = getBorderType(img, row, col, borderType);
    QImage resultImage(img);
    int centerRow = row / 2;
    int centerCol = col / 2;
    int filterSize = row * col;
    int w = ret.width();
    int h = ret.height();
    // 灰度图像膨胀和腐蚀
    if (imageType == 0) {
        vector<int> filterData;
        for (int i = centerCol; i < w - centerCol; i++) {
            for (int j = centerRow; j < h - centerRow; j++) {
                QRect rect(i - centerCol, j - centerRow, col, row);
                QImage patch = ret.copy(rect);
                for (int k = 0; k < filterSize; k++) {
                    filterData.push_back(qGray(patch.pixel(k / col, k % col)));
                }
                if (morphologyType == 0) {
                    auto maxGray = std::max_element(filterData.begin(), filterData.end());
                    resultImage.setPixel(i - centerCol, j - centerRow, qRgb(*maxGray, *maxGray, *maxGray));
                }else if (morphologyType == 1) {
                    auto minGray = std::min_element(filterData.begin(), filterData.end());
                    resultImage.setPixel(i - centerCol, j - centerRow, qRgb(*minGray, *minGray, *minGray));
                }
                filterData.clear();
            }
        }
    }
    // 彩色图像膨胀和腐蚀
    else if (imageType == 1) {
        vector<int> filterDataR;
        vector<int> filterDataG;
        vector<int> filterDataB;
        for (int i = centerCol; i < w - centerCol; i++) {
            for (int j = centerRow; j < h - centerRow; j++) {
                QRect rect(i - centerCol, j - centerRow, col, row);
                QImage patch = ret.copy(rect);
                for (int k = 0; k < filterSize; k++) {
                    filterDataR.push_back( qRed(patch.pixel(k / col, k % col)));
                    filterDataG.push_back( qGreen(patch.pixel(k / col, k % col)));
                    filterDataB.push_back( qBlue(patch.pixel(k / col, k % col)));
                }
                if (morphologyType == 0) {
                    auto maxR = std::max_element(filterDataR.begin(), filterDataR.end());
                    auto maxG = std::max_element(filterDataG.begin(), filterDataG.end());
                    auto maxB = std::max_element(filterDataB.begin(), filterDataB.end());
                    resultImage.setPixel(i - centerCol, j - centerRow, qRgb(*maxR, *maxG, *maxB));
                }else if (morphologyType == 1) {
                    auto minR = std::min_element(filterDataR.begin(), filterDataR.end());
                    auto minG = std::min_element(filterDataG.begin(), filterDataG.end());
                    auto minB = std::min_element(filterDataB.begin(), filterDataB.end());
                    resultImage.setPixel(i - centerCol, j - centerRow, qRgb(*minR, *minG, *minB));
                }
                filterDataR.clear();
                filterDataG.clear();
                filterDataB.clear();
            }
        }
    }
    return resultImage;
}

// 仿射变换
QImage ImageProcessor::affineTransform(const QImage& img, const QMatrix& mat)
{
    // 判断可逆
    if (!mat.isInvertible()) {
        throw std::range_error("Matrix is not invertible");
    }
    int width = img.width();
    int height = img.height();
    // 原图像顶点
    Matrix<double> originCornerMat(4, 2, 0);
    originCornerMat(0, 0) = 0;
    originCornerMat(0, 1) = 0;
    originCornerMat(1, 0) = width - 1.0;
    originCornerMat(1, 1) = 0;
    originCornerMat(2, 0) = 0;
    originCornerMat(2, 1) = height - 1.0;
    originCornerMat(3, 0) = width - 1.0;
    originCornerMat(3, 1) = height - 1.0;

    Matrix<double> newCornerMat(4, 2, 0);
    double minX = 0;
    double minY = 0;
    double maxX = 0;
    double maxY = 0;

    for (uint i = 0; i < 4; ++i) {
        mat.map(originCornerMat(i, 0), originCornerMat(i, 1), &newCornerMat(i, 0), &newCornerMat(i, 1));
        if (newCornerMat(i, 0) > maxX) maxX = newCornerMat(i, 0);
        if (newCornerMat(i, 0) < minX) minX = newCornerMat(i, 0);
        if (newCornerMat(i, 1) > maxY) maxY = newCornerMat(i, 1);
        if (newCornerMat(i, 1) < minY) minY = newCornerMat(i, 1);
    }

    // 获取新图像大小
    int newWidth = int(maxX - minX);
    int newHeight = int(maxY - minY);
    QImage resImg = QImage(newWidth, newHeight, QImage::Format_RGB32);

    QMatrix inverseMat = mat.inverted();
    for (int x = int(minX); x < maxX; x++) {
        for (int y = int(minY); y < maxY; y++) {
            double w = x * inverseMat.m11() + y * inverseMat.m21() + inverseMat.dx();
            double z = x * inverseMat.m12() + y * inverseMat.m22() + inverseMat.dy();
            if (w <= 0) w = 0;
            if (z <= 0) z = 0;
            if (w >= width) w = width - 1;
            if (z >= height) z = height - 1;

            double fw = floor(w);
            double cw = ceil(w);
            double fz = floor(z);
            double cz = ceil(z);

            QRgb rgb[4] = {
                img.pixel(int(fw), int(fz)),
                img.pixel(int(cw), int(fz)),
                img.pixel(int(fw), int(cz)),
                img.pixel(int(cw), int(cz))
            };

            double deltaW = w - fw;
            double deltaZ = z - fz;

            double pRgb[4][3];
            for (int i = 0; i < 4; ++i) {
                pRgb[i][0] = double(qRed(rgb[i]));
                pRgb[i][1] = double(qGreen(rgb[i]));
                pRgb[i][2] = double(qBlue(rgb[i]));
            }

            int resRgb[3];
            for (int i = 0; i < 3; ++i) {
                resRgb[i] = int( (pRgb[0][i] * (1 - deltaW) + deltaW * pRgb[2][i]) * (1 - deltaZ)
                         + (pRgb[1][i] * (1 - deltaW) + deltaW * pRgb[3][i]) * deltaZ);
            }
            int deltaX = x;
            int deltaY = y;
            if (minX < 0) deltaX = x - int(minX);
            if (minY < 0) deltaY = y - int(minY);
            if (w <= 0 || z <= 0 || w >= width - 1 || z >= height - 1)
                resImg.setPixel(deltaX, deltaY, qRgb(0, 0, 0));
            else
                resImg.setPixel(deltaX, deltaY, qRgb(resRgb[0], resRgb[1], resRgb[2]));
        }
    }
    return resImg;
}

// 频域滤波
QImage ImageProcessor::frequencyDomainFilter(const QImage& img, int filterType, int filterArea, double cutoff)
{
    if (img.isNull()) {
        qDebug() << "There is no image.";
        return img;
    }

    if (filterType != 0 && filterType != 1) {
        qDebug() << "Wrong filterType";
        return img;
    }

    if (filterArea != 0 && filterArea != 1) {
        qDebug() << "Wrong filterArea";
        return img;
    }

    qDebug() << "img pro" << endl;
    qDebug() << img.size() << endl;
    qDebug() << "filterType: " << filterType << endl;
    qDebug() << "filterArea: " << filterArea << endl;

    uint width = uint(img.width());         // COL
    uint height = uint(img.height());       // ROW
    Matrix<int> matR = fromQImage(img, 'R');
    Matrix<int> matG = fromQImage(img, 'G');
    Matrix<int> matB = fromQImage(img, 'B');

    Matrix<complex<double> > fftMatR = fft2d(matR, height, width);
    Matrix<complex<double> > fftMatG = fft2d(matG, height, width);
    Matrix<complex<double> > fftMatB = fft2d(matB, height, width);

    int n = 2;// 巴特沃斯滤波器阶数

    uint nRow = fftMatR.getNRow();
    uint nCol = fftMatR.getNCol();
    Matrix<complex<double> > filterMat(nRow, nCol, 0);
    for (uint i = 0; i < nRow; ++i) {
        for (uint j = 0; j < nCol; ++j) {
            double distance = sqrt((i - nRow / 2) * (i - nRow / 2) + (j - nCol / 2) * (j - nCol / 2));
            // 巴特沃斯滤波器
            if (0 == filterType) {
                filterMat(i, j) = 1 / (1 + pow(distance / cutoff, 2 * n));// TODO 设为 2阶
            }
            // 高斯滤波器
            else if (1 == filterType) {
                filterMat(i, j) = exp(-1 * distance / (2 * cutoff * cutoff));
            }
            if (1 == filterArea) {
                filterMat(i, j) = complex<double>(1, 0) - filterMat(i, j);
            }
        }
    }

    filterMat.fftshift();
    for (uint i = 0; i < nRow; ++i) {
        for (uint j = 0; j < nCol; ++j) {
            fftMatR(i, j) *= filterMat(i, j);
            fftMatG(i, j) *= filterMat(i, j);
            fftMatB(i, j) *= filterMat(i, j);
        }
    }

    Matrix<complex<double> > ifftMatR = ifft2d(fftMatR, nRow, nCol);
    Matrix<complex<double> > ifftMatG = ifft2d(fftMatG, nRow, nCol);
    Matrix<complex<double> > ifftMatB = ifft2d(fftMatB, nRow, nCol);

    for (uint i = 0; i < height; i++) {
        for (uint j = 0; j < width; j++) {
            matR(i, j) = int(real(ifftMatR(i, j)));
            matG(i, j) = int(real(ifftMatG(i, j)));
            matB(i, j) = int(real(ifftMatB(i, j)));
        }
    }
    return toQImage(matR, matG, matB);
}


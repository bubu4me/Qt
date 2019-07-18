#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QVector>
#include <vector>

using std::vector;

class ImageProcessor
{
public:
    ImageProcessor();

    // const+引用传递：节省时间但会彻底改掉原变量的值，所以用const限制引用的修改
    static QImage rgb2gray(const QImage& img);
    static QImage imageReverse(const QImage& img);
    static QImage logTransformation(const QImage& img,double c);
    static QImage contrastStretch(const QImage& img,double m,double E);
    static QImage grayHistogram(const QImage& img);
    static QImage colorHistogram(const QImage& img);
    static float Hue2RGB(float,float,float);
    static void filterOnGray(int,int,QImage,QImage&,vector<double>);
    static void filterOnRGB(int,int,QImage,QImage&,vector<double>);
    static QImage getBorderType(const QImage&,int,int,int);
    static QImage linearFilter(const QImage&,vector<double>,int,int,int);
    static void setGaussData(vector<double>&,int,double);
    static QImage gaussFilter(const QImage&,int,double,int,int);
    static void setBilateralData(QImage,vector<double>&,vector<double>,int,double);
    static QImage bilateralFilter(const QImage&,int,double,double,int,int);
    static QImage medianFilter(const QImage&,int,int,int,int);
    static QImage morphologyOperation(const QImage&,int,int,int,int,int);
    static QImage affineTransform(const QImage&, const QMatrix&);
    static QImage frequencyDomainFilter(const QImage& img,int filterType,int filterArea,double cutoff);
};

#endif // IMAGEPROCESSOR_H

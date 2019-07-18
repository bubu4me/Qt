#include "fft.h"
#include "matrix.h"

#define PI 3.1415926

using std::complex;
using std::vector;

size_t calcN(size_t length)
{
    size_t t = length;
    size_t res = 0;

    // check if length is power of 2
    // if it is, just return length
    if ((length & (length - 1)) == 0)
        return length;
    // if not, get the correct N and return
    while (t != 0) {
        res |= t;
        t = t >> 1;
    }
    if ((res + 1) - length == length)
        return length;
    else
        return res + 1;
}

/**
 * @brief i_fft
 * @param data
 * @param N
 * @param fftORifft
 *      true for fft
 *      false for ifft
 * @return
 */
template <typename T>
vector<complex<double> > i_fft(vector<T> data, size_t N, bool fftORifft)
{
    // convert data to vector<complex<double> >
    // call:
    // vector<complex<double> >
    vector<complex<double> > complexData;
    size_t size = data.size();

    for (size_t i = 0; i < size; i++) {
        complexData.push_back(complex<double>(data.at(i), 0));
    }
    if (fftORifft == true)
        return fft(complexData, N);
    else
        return ifft(complexData, N);
}

/**
 * @brief i_fft
 * @param data
 * @param N
 * @param head
 *      if head == 1, fft
 *      if head == N, ifft
 * @param fftORifft
 *      if fftORifft == 1, fft
 *      if fftORifft == -1, ifft
 * @return
 */
vector<complex<double> > i_fft(vector<complex<double> > data, size_t N, size_t head, int fftORifft)
{
    size_t len = calcN(data.size());
    vector<complex<double> > res(len);

    // start fft
    // check if N is 0, 1, 2
    // if N is 0 or 1, just return data
    if (0 == N || 1 == N) {
        return data;
    }
    // if N is 2, do dft on them
    else if (2 == N) {
        res.at(0) = 1.0 / head * ((data.at(0) + data.at(1)) * complex<double>(1, 0));
        res.at(1) = 1.0 / head * (data.at(0) * complex<double>(1, 0) + data.at(1)
                      * complex<double>( cos(2 * PI * 1 / 2), -fftORifft * sin(2 * PI * 1 / 2)));
        return res;
    }

    // check length of data, and append 0 if neccessary
    while (len > data.size()) {
        data.push_back(complex<double>(0, 0));
    }
    // if N > 2, do fft
    // 1. split input into two parts
    vector<complex<double> > evenIndexData;
    vector<complex<double> > oddIndexData;
    for (size_t i = 0; i < len / 2; i++) {
        evenIndexData.push_back(data.at(i * 2));
        oddIndexData.push_back(data.at(i * 2 + 1));
    }
    // TODO 提示之前一直出现的bug就在这里，因为之前一直在调用fft而不是i_fft
    // i_fft中的形参N为1，不然每次都会除以N
    vector<complex<double> > X1_k = i_fft(evenIndexData, len / 2, 1, fftORifft);
    vector<complex<double> > X2_k = i_fft(oddIndexData, len / 2, 1, fftORifft);
    // 2. do fft on them seperately
    // 3. construct result from output
    for (size_t k = 0; k < len / 2; k++) {
        double angle = fftORifft * (-2 * PI * k / N);
        res.at(k) = 1.0 / head * (X1_k.at(k) + X2_k.at(k) * complex<double>(cos(angle), sin(angle)));
        res.at(k + len / 2) = 1.0 / head * (X1_k.at(k) -
                            X2_k.at(k) * complex<double>(cos(angle), sin(angle)));
    }
    return res;
}

vector<complex<double> > fft(vector<int> data, size_t N)
{
    return i_fft(data, N, true);
}
vector<complex<double> > fft(vector<double> data, size_t N)
{
    return i_fft(data, N, true);
}
vector<complex<double> > fft(vector<complex<double> > data, size_t N)
{
    return i_fft(data, N, 1, 1);
}
vector<complex<double> > ifft(vector<int> data, size_t N)
{
    return i_fft(data, N, false);
}
vector<complex<double> > ifft(vector<double> data, size_t N)
{
    return i_fft(data, N, false);
}
vector<complex<double> > ifft(vector<complex<double> > data, size_t N)
{
    return i_fft(data, N, N, -1);
}

/**
 * @brief i_fft2d
 * @param data
 * @param r == row
 * @param c == col
 * @param fftORifft
 *      if fftORifft == true, fft
 *      if fftORifft == false, ifft
 * @return
 */
template <typename T>
Matrix<complex<double> > i_fft2d(const Matrix<T>& data, size_t r, size_t c, bool fftORifft)
{
    Matrix<complex<double> > myData(r, c, 0);

    for (size_t row = 0; row < r; ++row) {
        vector<complex<double> > complexData;
        for (size_t i = 0; i < c; ++i) {
            complexData.push_back(complex<double>(data.getRow(row).at(i), 0));
        }
        myData.setRow(row, complexData);
    }

    if (fftORifft == true)
        return fft2d(myData, r, c);
    else
        return ifft2d(myData, r, c);
}

Matrix<complex<double> > fft2d(const Matrix<int>& data, size_t r, size_t c)
{
    return i_fft2d(data, r, c, true);
}
Matrix<complex<double> > fft2d(const Matrix<double>& data, size_t r, size_t c)
{
    return i_fft2d(data, r, c, true);
}
Matrix<complex<double> > fft2d(const Matrix<complex<double> >& data, size_t r, size_t c)
{
    size_t NRow = calcN(r);
    size_t NCol = calcN(c);
    Matrix<complex<double> > myData(NRow, NCol, 0);

    for (size_t row = 0; row < r; ++row) {
        for (size_t col = 0; col < c; ++col) {
            myData(row, col) = data(row, col);
        }
    }
    myData = fftRow(myData, NRow, NCol);
    myData.transpose();
    myData = fftRow(myData, NCol, NRow);
    myData.transpose();
    return myData;
}
Matrix<complex<double> > fftRow(const Matrix<complex<double> >& data, size_t r, size_t c)
{
    Matrix<complex<double> > myData(r, c, 0);

    for (size_t row = 0; row < r; ++row) {
        vector<complex<double> > rowData = data.getRow(row);
        rowData = fft(rowData, c);
        myData.setRow(row, rowData);
    }
    return myData;
}
Matrix<complex<double> > ifft2d(const Matrix<int>& data, size_t r, size_t c)
{
    return i_fft2d(data, r, c, false);
}
Matrix<complex<double> > ifft2d(const Matrix<double>& data, size_t r, size_t c)
{
    return i_fft2d(data, r, c, false);
}
Matrix<complex<double> > ifft2d(const Matrix<complex<double> >& data, size_t r, size_t c)
{
    size_t NRow = calcN(r);
    size_t NCol = calcN(c);
    Matrix<complex<double> > myData(NRow, NCol, 0);

    for (size_t row = 0; row < r; ++row) {
        for (size_t col = 0; col < c; ++col) {
            myData(row, col) = data(row, col);
        }
    }
    myData = ifftRow(myData, NRow, NCol);
    myData.transpose();
    myData = ifftRow(myData, NCol, NRow);
    myData.transpose();
    return myData;
}
Matrix<complex<double> > ifftRow(const Matrix<complex<double> >& data, size_t r, size_t c)
{
    Matrix<complex<double> > myData(r, c, 0);

    for (size_t row = 0; row < r; ++row) {
        vector<complex<double> > rowData = data.getRow(row);
        rowData = ifft(rowData, c);
        myData.setRow(row, rowData);
    }
    return myData;
}

// 对图片的单通道做fft
Matrix<int> fftImg1Channel(const QImage& srcImg, double min, double max, char channel)
{
    size_t r = size_t(srcImg.height());
    size_t c = size_t(srcImg.width());
    Matrix<int> qImg = fromQImage(srcImg, channel);
    Matrix<complex<double> > fftImg1C = fft2d(fromQImage(srcImg, channel), r, c);

    fftImg1C.fftshift();

    r = fftImg1C.getNRow();
    c = fftImg1C.getNCol();
    Matrix<int> fftImg(r, c, 0);

    for (size_t row = 0; row < r; ++row) {
        for (size_t col = 0; col < c; ++col) {
            fftImg(row, col) = int(log(1 + abs(fftImg1C(row, col))) + 0.5);
        }
    }
    fftImg = nomarlized(fftImg, min, max);

    return fftImg;
}

// 归一化
Matrix<int> nomarlized(Matrix<int> data, double min, double max)
{
    int maxValue = 0;
    int minValue = 999;

    for (size_t i = 0; i < data.getNRow(); ++i) {
        for (size_t j = 0; j < data.getNCol(); ++j) {
            if (maxValue < data(i, j)) {
                maxValue = data(i, j);
            }
            if (minValue > data(i, j)) {
                minValue = data(i, j);
            }
        }
    }

    for (size_t i = 0; i < data.getNRow(); ++i) {
        for (size_t j = 0; j < data.getNCol(); ++j) {
            data(i, j) = int((data(i, j) - minValue) * (max - min) / (maxValue - minValue));
        }
    }
    return data;
}

// 返回频谱图
QImage fftImg(const QImage& srcImg)
{
    Matrix<int> matR = fftImg1Channel(srcImg, 0, 255, 'R');
    Matrix<int> matG = fftImg1Channel(srcImg, 0, 255, 'G');
    Matrix<int> matB = fftImg1Channel(srcImg, 0, 255, 'B');

    return toQImage(matR, matG, matB);
}

#ifndef FFT__H
#define FFT__H

#include <complex>

#include "matrix.h"

using std::vector;
using std::complex;

const double PI = 3.1415926;

size_t calcN(size_t length);
//std::complex<double> pow(std::complex<double> base, int exponent);

template <typename T>
vector<complex<double> > i_fft(vector<T>, size_t, bool);
// different function with different input
vector<complex<double> > i_fft(vector<complex<double> >, size_t, size_t, int );
vector<complex<double> > fft(vector<int>, size_t);
vector<complex<double> > fft(vector<double>, size_t);
vector<complex<double> > fft(vector<complex<double> >, size_t);
vector<complex<double> > ifft(vector<int>, size_t);
vector<complex<double> > ifft(vector<double>, size_t);
vector<complex<double> > ifft(vector<complex<double> >, size_t);

template <typename T>
Matrix<complex<double> > i_fft2d(const Matrix<T>&, size_t, size_t, bool);
Matrix<complex<double> > fft2d(const Matrix<int>&, size_t, size_t);
Matrix<complex<double> > fft2d(const Matrix<double>&, size_t, size_t);
Matrix<complex<double> > fft2d(const Matrix<complex<double> >&, size_t, size_t);
Matrix<complex<double> > fftRow(const Matrix<complex<double> >&, size_t, size_t);
Matrix<complex<double> > ifft2d(const Matrix<int>&, size_t, size_t);
Matrix<complex<double> > ifft2d(const Matrix<double>&, size_t, size_t);
Matrix<complex<double> > ifft2d(const Matrix<complex<double> >&, size_t, size_t);
Matrix<complex<double> > ifftRow(const Matrix<complex<double> >&, size_t, size_t);

Matrix<int> fftImg1Channel(const QImage& srcImg, double min, double max, char channel);
QImage fftImg(const QImage& srcImg);
Matrix<int> nomarlized(Matrix<int>, double min, double max);

#endif

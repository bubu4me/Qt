#ifndef MATRIX__H
#define MATRIX__H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <QDebug>
#include <QImage>

using std::range_error;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

template<typename T>
class Matrix
{
private:
// point to the data
T** _data;
// number of row and column
size_t _nRow, _nCol;
// flag to indicate row-first or column-first
int _t;
// start point of row and col

int _startR = 0;
int _startC = 0;

bool checkRange(size_t r, size_t c) const;
void swap(Matrix<T>& mat);

public:
// construct a r-row and c-column matrix with all init element
Matrix(size_t r, size_t c, T init);
// copy constructor
Matrix(const Matrix<T>& mat);
Matrix<T>& operator=(Matrix<T> mat);

~Matrix();
// get element at r-row and c-column
// return reference to make it modifiable
T& operator()(size_t r, size_t c) const;
// get a submatrix [rs:re,cs:ce]
Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;
// output the matrix
template<typename U>
friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);

size_t getNRow() const;
size_t getNCol() const;
// todo
std::vector<T> getRow(size_t r) const;
std::vector<T> getCol(size_t c) const;
bool setRow(size_t r, const std::vector<T>& d);
bool setCol(size_t c, const std::vector<T>& d);
// transpose it in-place
Matrix<T>& transpose();
Matrix<T> fftshift();

// Matrix and Matrix
Matrix<T> operator+(const Matrix<T> mat);                               // addition
Matrix<T> operator-(const Matrix<T> mat);                               // subtraction
Matrix<T> operator*(const Matrix<T> mat);                               // multiplication

// Variable--Matrix
// addition
friend Matrix<T> operator+(const T& v, const Matrix<T>& mat)
{
    Matrix<T> vMat(mat.getNRow(), mat.getNCol(), v);

    return vMat.operator+(mat);
}
// substraction
friend Matrix<T> operator-(const T& v, const Matrix<T>& mat)
{
    Matrix<T> vMat(mat.getNRow(), mat.getNCol(), v);

    return vMat.operator-(mat);
}
// multiplication
friend Matrix<T> operator*(const T& v, const Matrix<T>& mat)
{
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);

    for (int i = 0; i < mat.getNRow(); i++) {
        for (int j = 0; j < mat.getNCol(); j++) {
            thisMat.operator()(i, j) = v * mat.operator()(i, j);
        }
    }
    return thisMat;
}

// Matrix--Variable
// addition
friend Matrix<T> operator+(const Matrix<T>& mat, const T& v)
{
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);

    for (int i = 0; i < mat.getNRow(); i++) {
        for (int j = 0; j < mat.getNRow(); j++) {
            thisMat.operator()(i, j) = mat.operator()(i, j) + v;
        }
    }
    return thisMat;
}
// substraction
friend Matrix<T> operator-(const Matrix<T>& mat, const T& v)
{
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);

    for (int i = 0; i < mat.getNRow(); i++) {
        for (int j = 0; j < mat.getNRow(); j++) {
            thisMat.operator()(i, j) = mat.operator()(i, j) - v;
        }
    }
    return thisMat;
}
// multiplication
friend Matrix<T> operator*(const Matrix<T>& mat, const T& v)
{
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);

    for (int i = 0; i < mat.getNRow(); i++) {
        for (int j = 0; j < mat.getNCol(); j++) {
            thisMat.operator()(i, j) = mat.operator()(i, j) * v;
        }
    }
    return thisMat;
}
// division
friend Matrix<T> operator/(const Matrix<T>& mat, const T& v)
{
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);

    for (int i = 0; i < mat.getNRow(); i++) {
        for (int j = 0; j < mat.getNCol(); j++) {
            thisMat.operator()(i, j) = mat.operator()(i, j) / v;
        }
    }
    return thisMat;
}
};

template <typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init) :
    _nRow(r), _nCol(c), _t(0)
{
    // new _data
    _data = new T * [r];
    // new _data[i] within loop
    for (size_t i = 0; i < r; i++) {
        _data[i] = new T[c];
        for (size_t j = 0; j < c; j++) {
            _data[i][j] = init;
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& mat) :
    _nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t)
{
    // new _data
    _data = new T * [_nRow];
    // new and assign _data[i] within loop
    for (size_t i = 0; i < _nRow; i++) {
        _data[i] = new T[_nCol];
        for (size_t j = 0; j < _nCol; j++) {
            _data[i][j] = mat._data[i][j];
        }
    }
}

template <typename T>
void Matrix<T>::swap(Matrix<T>& mat)
{
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_t, mat._t);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}

template <typename T>
size_t Matrix<T>::getNRow() const
{
    // return the number of row
    return this->_nRow;
}

template <typename T>
size_t Matrix<T>::getNCol() const
{
    // return the number of column
    return this->_nCol;
}

template <typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    // check if r-row and c-column is out of range
    if (r >= _nRow || c >= _nCol) {
        return false;
    }
    return true;
}

template <typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    // get element in (r,c)
    // remember check first
    if (!checkRange(r, c)) {
        throw std::range_error("row or col out of range");
    }
    // different of _t has different order
    if (_t == 0) {
        return _data[(r + _startR) % _nRow][(c + _startC) % _nCol];
    }else {
        return _data[(c + _startC) % _nCol][(r + _startR) % _nRow];
    }
}

template <typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    // note the validity of value:rs, re, cs, ce
    // try to use "except" to broadcast the exception
    if (rs <= re && cs <= ce && checkRange(re, ce)) {
        Matrix mat(re - rs + 1, ce - cs + 1, 0);
        for (size_t i = rs; i <= re; i++) {
            for (size_t j = cs; j <= ce; j++) {
                mat._data[i - rs][j - cs] = this->operator()(i, j);
            }
        }
        return mat;
    }else {
        qDebug() << "row or col out of range";
        qDebug() << "return source matrix";
        return *this;
    }
}

template <typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    // get all element in r-row and push it back in a vector
    // remember check range first
    checkRange(r, 0);
    vector<T> _currentRowData;
    for (size_t i = 0; i < _nCol; i++) {
//        cout << this->getNRow() << this->getNCol() << endl;
//        cout << this->operator()(r, i) << endl;

        _currentRowData.push_back(this->operator()(r, i));
    }
    return _currentRowData;
}

template <typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    // get all element in c-column and push it back in a vector
    // remember check range first
    checkRange(0, c);
    vector<T> _currentColData;
    for (size_t i = 0; i < _nRow; i++) {
        _currentColData.push_back(this->operator()(i, c));
    }
    return _currentColData;
}

template <typename T>
bool Matrix<T>::setRow(size_t r, const vector<T>& d)
{
    // set the element of the r-row
    // remember check range first
    if (!checkRange(r, 0)) {
        qDebug() << "row out of range";
        return false;
    }
    if (d.size() < this->_nCol) {
        qDebug() << "d's size not enough";
        return false;
    }
    for (size_t i = 0; i < this->_nCol; i++) {
        this->operator()(r, i) = d.at(i);
    }
    return true;
}

template <typename T>
bool Matrix<T>::setCol(size_t c, const vector<T>& d)
{
    // set the element of the c-column
    // remember check range first
    if (!checkRange(0, c)) {
        qDebug() << "col out of range";
        return false;
    }
    if (d.size() < this->_nRow) {
        qDebug() << "d's size not enough";
        return false;
    }
    for (size_t i = 0; i < this->_nRow; i++) {
        this->operator()(i, c) = d.at(i);
    }
    return true;
}

template <typename T>
Matrix<T>::~Matrix()
{
    // delete _data[i] first
    if (_t == 0) {
        for (size_t i = 0; i < _nRow; i++) {
            delete [] _data[i];
        }
    }else{
        for (size_t i = 0; i < _nCol; i++) {
            delete [] _data[i];
        }
    }
    // then delete _data
    delete _data;
    // note the value of _t
}

template <typename T>
Matrix<T>& Matrix<T>::transpose()
{
    // change _t
    // swap _nRow and _nCol
    _t = !_t;
    size_t tmp = _nRow;
    _nRow = _nCol;
    _nCol = tmp;
    return *this;
}

template <typename T>
ostream& operator<<(ostream& out, const Matrix<T>& rhs)
{
    for (size_t i = 0; i < rhs._nRow; ++i) {
        for (size_t j = 0; j < rhs._nCol; ++j) {
            out << rhs(i, j) << "\t";
        }
        out << endl;
    }
    return out;
}

// Matrix--Matrix
// addition
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> mat)
{
    // TODO Exception: maybe both matrix are not the same size
    if (this->getNRow() != mat.getNRow() || this->getNCol() != mat.getNCol()) {
        qDebug() << "wrong size";
        return mat;
    }
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);
    for (int i = 0; i < this->getNRow(); i++) {
        for (int j = 0; j < this->getNCol(); j++) {
            thisMat.operator()(i, j) = this->operator()(i, j) + mat.operator()(i, j);
        }
    }
    return thisMat;
}

// subtraction
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> mat)
{
    // TODO Exception: maybe both matrix are not the same size
    if (this->getNRow() != mat.getNRow() || this->getNCol() != mat.getNCol()) {
        qDebug() << "wrong size";
        return mat;
    }
    Matrix<T> thisMat(mat.getNRow(), mat.getNCol(), 0);
    for (int i = 0; i < this->getNRow(); i++) {
        for (int j = 0; j < this->getNCol(); j++) {
            thisMat.operator()(i, j) = this->operator()(i, j) - mat.operator()(i, j);
        }
    }
    return thisMat;
}

// multiplication
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> mat)
{
    // TODO Exception: maybe both matrix are not the same size
    if (this->getNCol() != mat.getNRow()) {
        qDebug() << "wrong size";
        return mat;
    }
    Matrix<T> thisMat(this->getNRow(), mat.getNCol(), 0);
    vector<T> thisMatRow;
    for (int i = 0; i < this->getNRow(); i++) {
        vector<T> thisRow = this->getRow(i);
        for (int j = 0; j < mat.getNCol(); j++) {
            vector<T> thisCol = mat.getCol(j);
            T sum = 0;
            for (int k = 0; k < this->getNCol(); k++) {
                sum += thisRow.at(k) * thisCol.at(k);
            }
            thisMatRow.push_back(sum);
        }
        thisMat.setRow(i, thisMatRow);
    }
    return thisMat;
}


// transfer image to matrix
static Matrix<int> fromQImage(const QImage& img, char channel)
{
    if (img.isNull()) {
        cout << "none img" << endl;
    }
    if (channel != 'R' & channel != 'G' & channel != 'B') {
        cout << "wrong channel input" << endl;
    }
    size_t row = size_t(img.height());
    size_t col = size_t(img.width());
    qDebug() << row << " " << col << endl;

    Matrix<int> mat(row, col, 0);
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            QRgb rgb = img.pixel(int(j), int(i));
            switch (channel) {
            case 'R':
                mat(i, j) = qRed(rgb);
                break;
            case 'G':
                mat(i, j) = qGreen(rgb);
                break;
            case 'B':
                mat(i, j) = qBlue(rgb);
                break;
            default:
                mat(i, j) = size_t(999);
                break;
            }
        }
    }
    return mat;
}

static QImage toQImage(const Matrix<int>& mat)
{
    size_t w = size_t(mat.getNCol());
    size_t h = size_t(mat.getNRow());
    QImage img(int(w), int(h), QImage::Format_RGB32);

    for (size_t x = 0; x < w; x++) {
        for (size_t y = 0; y < h; y++) {
            img.setPixel(int(x), int(y), qRgb(mat(y, x), mat(y, x), mat(y, x)));
        }
    }
    return img;
}

static QImage toQImage(const Matrix<int>& matR, const Matrix<int>& matG, const Matrix<int>& matB)
{
    size_t w = size_t(matR.getNCol());
    size_t h = size_t(matR.getNRow());
    QImage img(int(w), int(h), QImage::Format_RGB32);

    for (size_t x = 0; x < w; x++) {
        for (size_t y = 0; y < h; y++) {
            img.setPixel(int(x), int(y), qRgb(matR(y, x), matG(y, x), matB(y, x)));
        }
    }
    return img;
}

template <typename T>
Matrix<T> Matrix<T>:: fftshift()
{
    _startR = _nRow / 2;
    _startC = _nCol / 2;
    cout << "sR" << _startR << " sC " << _startC << endl;
    return *this;
}

#endif

#pragma once

#include <vector>
#include <ostream>
#include <istream>
#include <algorithm>

#include <util.h>

using std::vector;
using std::istream;
using std::ostream;

/*!
	\brief Matrix of doubles
*/
class Matrix
{
private:
    /*!
		\brief One-dimensional vector of values
	*/
    vector<double> values;

    /*!
		\brief Width of matrix
	*/
    size_t width;

public:
    Matrix():
        values(),
        width(0)
    {
    }

    Matrix(size_t height, size_t width):
        values(height * width),
        width(height == 0 ? 0 : width)
    {
    }

    Matrix(size_t height, size_t width, double value):
        values(height * width, value),
        width(height == 0 ? 0 : width)
    {
    }

    /*!
		\brief Height of matrix
	*/
    size_t Height() const
    {
        return width == 0 ? 0 : values.size() / width;
    }

    /*!
		\brief Width of matrix
	*/
    size_t Width() const
    {
        return width;
    }

    /*!
		\brief If matrix is empty
	*/
    bool IsEmpty() const
    {
        return width == 0;
    }

    /*!
		\brief If matrix is square
	*/
    bool IsSquare() const
    {
        return Height() == Width();
    }

    /*!
		\brief Resize matrix
	*/
    void Resize(size_t newHeight, size_t newWidth)
    {
        values.resize(newHeight * newWidth);
        width = newHeight == 0 ? 0 : newWidth;
    }

    /*!
		\brief Read-only access to matrix elements
	*/
    double operator()(size_t rowIndex, size_t columnIndex) const
    {
        if (rowIndex >= Height() || columnIndex >= Width())
            ThrowIndexOutOfRange();
        return values[width * rowIndex + columnIndex];
    }

    /*!
		\brief Access to matrix elements
	*/
    double& operator()(size_t rowIndex, size_t columnIndex)
    {
        if (rowIndex >= Height() || columnIndex >= Width())
            ThrowIndexOutOfRange();
        return values[width * rowIndex + columnIndex];
    }

    /*!
		\brief Diagonal matrix of the given size
	*/
    static void GetDiagonal(size_t size, double value, Matrix* outMatrix);

    /*!
		\brief Matrix identity of the given size
	*/
    static void GetIdentity(size_t size, Matrix* outMatrix);

    /*!
		\brief One-row matrix
	*/
    static void FromRow(const vector<double>& row, Matrix* outMatrix);

    /*!
		\brief One-column matrix
	*/
    static void FromVector(const vector<double>& vector, Matrix* outMatrix);
    
    /*!
		\brief Row of the matrix
	*/
    void GetRow(size_t rowIndex, vector<double>* outRow) const;

    /*!
		\brief Column of the matrix
	*/
    void GetColumn(size_t columnIndex, vector<double>* outColumn) const;

    /*!
		\brief Select several rows of the matrix
	*/
    template<class TIterator>
    void SelectRows(TIterator firstIndex, TIterator lastIndex, Matrix* outMatrix) const;

    /*!
		\brief Selects several columns of the matrix
	*/
    template<class TIterator>
    void SelectColumns(TIterator firstIndex, TIterator lastIndex, Matrix* outMatrix) const;

    /*!
		\brief Selects the minor (i.e., several rows and columns) of the matrix
	*/
    template<class TIterator1, class TIterator2>
    void GetMinor(
        TIterator1 firstRowIndex, TIterator1 lastRowIndex,
        TIterator2 firstColumnIndex, TIterator2 lastColumnIndex,
        Matrix* outMatrix) const;

    /*!
		\brief Append the row to the bottom of the matrix
	*/
    void AppendRow(const vector<double>& row);

    /*!
		\brief Append the column to the right of the matrix
	*/
    void AppendVector(const vector<double>& vec);

    /*!
		\brief Append another matrix to the bottom of this matrix
	*/
    void AppendDown(const Matrix& matrix);

    /*!
		\brief Append another matrix to the right of this matrix
	*/
    void AppendRight(const Matrix& matrix);

    /*!
		\brief Transpose the matrix
	*/
    void Transpose(Matrix* outMatrix) const;
    
    /*!
		\brief Arithmetical operators
	*/
    Matrix& operator +=(const Matrix& matrix);
    Matrix& operator -=(const Matrix& matrix);
    Matrix& operator *=(const Matrix& matrix);
    Matrix& operator *=(double scalar);
    Matrix& operator /=(double scalar);
};

const Matrix operator +(const Matrix& left, const Matrix& right);
const Matrix operator -(const Matrix& left, const Matrix& right);
const Matrix operator *(const Matrix& left, const Matrix& right);
const Matrix operator *(double scalar, const Matrix& matrix);
const Matrix operator *(const Matrix& matrix, double scalar);
const Matrix operator /(const Matrix& matrix, double scalar);

/*!
	\brief To multiply not only by vector<double>, but also by vector<int>
*/
template<class T>
vector<double> operator *(const Matrix& matrix, const vector<T>& vec);

/*!
	\brief IO operators
*/
ostream& operator <<(ostream& output, const Matrix& matrix);
istream& operator >>(istream& input, Matrix& matrix);


// --- Implementation ---

template<class TIterator>
void Matrix::SelectRows(TIterator firstIndex, TIterator lastIndex, Matrix* outMatrix) const
{
    outMatrix->Resize(lastIndex - firstIndex, Width());
    size_t i = 0;
    for (TIterator it = firstIndex; it != lastIndex; ++it, ++i)
        copy(
            values.begin() + (*it) * width,
            values.begin() + (*it + 1) * width,
            outMatrix->values.begin() + i * outMatrix->width);
}

template<class TIterator>
void Matrix::SelectColumns(TIterator firstIndex, TIterator lastIndex, Matrix* outMatrix) const
{
    outMatrix->Resize(Height(), lastIndex - firstIndex);
    for (size_t i = 0; i < Height(); ++i)
    {
        size_t j = 0;
        for (TIterator it = firstIndex; it != lastIndex; ++it, ++j)
            (*outMatrix)(i, j) = (*this)(i, it);
    }
}

template<class TIterator1, class TIterator2>
void Matrix::GetMinor(
    TIterator1 firstRowIndex, TIterator1 lastRowIndex,
    TIterator2 firstColumnIndex, TIterator2 lastColumnIndex,
    Matrix* outMatrix) const
{
    outMatrix->Resize(lastRowIndex - firstRowIndex, lastColumnIndex - firstColumnIndex);
    size_t i = 0;
    for (TIterator1 it1 = firstRowIndex; it1 != lastRowIndex; ++it1, ++i)
    {
        size_t j = 0;
        for (TIterator2 it2 = firstColumnIndex; it2 != lastColumnIndex; ++it2, ++j)
            (*outMatrix)(i, j) = (*this)(*it1, *it2);
    }
}

template<class T>
vector<double> operator *(const Matrix& matrix, const vector<T>& vec)
{
    if (vec.size() != matrix.Width())
        ThrowMatrixIncorrectSize();
    vector<double> res(matrix.Height());
    for (size_t i = 0; i < matrix.Height(); ++i)
    {
        for (size_t k = 0; k < matrix.Width(); ++k)
            res[i] += matrix(i, k) * vec[k];
    }
    return res;
}

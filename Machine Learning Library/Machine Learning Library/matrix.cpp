#include <matrix.h>

#include <algorithm>

using std::endl;

void Matrix::GetDiagonal(size_t size, double value, Matrix* outMatrix)
{
    outMatrix->Resize(size, size);
    for (size_t i = 0; i < size; ++i)
        (*outMatrix)(i, i) = value;
}

void Matrix::GetIdentity(size_t size, Matrix* outMatrix)
{
    outMatrix->Resize(size, size);
    for (size_t i = 0; i < size; ++i)
        (*outMatrix)(i, i) = 1;
}

void Matrix::FromVector(const vector<double>& vector, Matrix* outMatrix)
{
    outMatrix->Resize(vector.size(), 1);
    outMatrix->values = vector;
}

void Matrix::FromRow(const vector<double>& row, Matrix* outMatrix)
{
    outMatrix->Resize(1, row.size());
    outMatrix->values = row;
}

void Matrix::GetRow(size_t rowIndex, vector<double>* outRow) const
{
    outRow->assign(
        values.begin() + width * rowIndex, 
        values.begin() + width * (rowIndex + 1));
}

void Matrix::GetColumn(size_t columnIndex, vector<double>* outColumn) const
{
    outColumn->resize(Height());
    for (size_t i = 0; i < Height(); ++i)
        (*outColumn)[i] = (*this)(i, columnIndex);
}

void Matrix::AppendRow(const vector<double>& row)
{
    if (IsEmpty())
    {
        values = row;
        width = row.size();
	}
    else
    {
		if (row.size() != Width())
			ThrowMatrixIncorrectSize();
		values.insert(values.end(), row.begin(), row.end());
	}
}

void Matrix::AppendVector(const vector<double>& vec)
{
    if (IsEmpty())
    {
        values = vec;
        width = 1;
	}
    else
    {
		if (vec.size() != Height())
			ThrowMatrixIncorrectSize();

		for (int i = Height() - 1; i >= 0; --i)
			values.insert(values.begin() + width * (i + 1), vec[i]);
		++width;
	}
}

void Matrix::AppendDown(const Matrix& matrix)
{
    if (matrix.IsEmpty())
    if (IsEmpty()) 
    {
        *this = matrix;
        return;
    }
    if (matrix.Width() != Width())
        ThrowMatrixIncorrectSize();

    size_t initialSize = values.size();
    values.resize(values.size() + matrix.values.size());
    copy(matrix.values.begin(), matrix.values.end(), values.begin() + initialSize);
}

void Matrix::AppendRight(const Matrix& matrix)
{
    if (matrix.IsEmpty())
        return;
    if (IsEmpty()) 
    {
        *this = matrix;
    }
    if (matrix.Height() != Height())
        ThrowMatrixIncorrectSize();

    for (int i = Height() - 1; i >= 0; --i)
    {
        values.insert(
            values.begin() + width * (i + 1), 
            matrix.values.begin() + matrix.width * i,
            matrix.values.begin() + matrix.width * (i + 1));
    }
    width += matrix.width;
}

void Matrix::Transpose(Matrix* outMatrix) const
{
    outMatrix->Resize(Width(), Height());
    for (size_t i = 0; i < Height(); ++i)
    {
        for (size_t j = 0; j < Width(); ++j)
            (*outMatrix)(j, i) = (*this)(i, j);
    }
}

Matrix& Matrix::operator +=(const Matrix& matrix)
{
    if (matrix.Height() != Height() || matrix.Width() != Width())
        ThrowMatrixIncorrectSize();
    for (size_t i = 0; i < Height(); ++i)
    {
        for (size_t j = 0; j < Width(); ++j)
            (*this)(i, j) += matrix(i, j);
    }
    return *this;
}

Matrix& Matrix::operator -=(const Matrix& matrix)
{
    if (matrix.Height() != Height() || matrix.Width() != Width())
        ThrowMatrixIncorrectSize();
    for (size_t i = 0; i < Height(); ++i)
    {
        for (size_t j = 0; j < Width(); ++j)
            (*this)(i, j) -= matrix(i, j);
    }
    return *this;
}

Matrix& Matrix::operator *=(const Matrix& matrix)
{
    return *this = *this * matrix;
}

Matrix& Matrix::operator *=(double scalar)
{
    for (size_t i = 0; i < Height(); ++i)
    {
        for (size_t j = 0; j < Width(); ++j)
            (*this)(i, j) *= scalar;
    }
    return *this;
}

Matrix& Matrix::operator /=(double scalar)
{
    return (*this) *= 1 / scalar;
}

const Matrix operator +(const Matrix& left, const Matrix& right)
{
    Matrix res(left);
    return res += right;
}

const Matrix operator -(const Matrix& left, const Matrix& right)
{
    Matrix res(left);
    return res -= right;
}

const Matrix operator *(const Matrix& left, const Matrix& right)
{
    if (left.Width() != right.Height())
        ThrowMatrixIncorrectSize();
    Matrix res(right.Height(), left.Width());
    for (size_t i = 0; i < res.Height(); ++i)
    {
        for (size_t j = 0; j < res.Width(); ++j)
        {
            for (size_t k = 0; k < left.Width(); ++k)
                res(i, j) += left(i, k) * right(k, j);
        }
    }
    return res;
}

const Matrix operator *(double scalar, const Matrix& matrix)
{
    Matrix res(matrix);
    return res *= scalar;
}

const Matrix operator *(const Matrix& matrix, double scalar)
{
    Matrix res(matrix);
    return res *= scalar;
}

const Matrix operator /(const Matrix& matrix, double scalar)
{
    Matrix res(matrix);
    return res /= scalar;
}

ostream& operator <<(ostream& output, const Matrix& matrix)
{
    output << matrix.Height() << "\t" << matrix.Width() << endl;
    for (size_t i = 0; i < matrix.Height(); ++i)
    {
        for (size_t j = 0; j < matrix.Width(); ++j)
            (j == 0 ? output : output << "\t") << matrix(i, j);
        output << endl;
    }
    return output;
}

istream& operator >>(istream& input, Matrix& matrix)
{
    size_t height, width;
    input >> height >> width;
    matrix.Resize(height, width);
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
            input >> matrix(i, j);
    }
    return input;
}

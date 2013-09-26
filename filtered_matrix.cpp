#include <iostream>
#include <vector>

#include <boost/iterator/permutation_iterator.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>
#include <boost/assign.hpp>

using boost::assign::operator+=;

#if 0
template<class T>
class Matrix {
private:
    typedef std::vector< std::vector<T> > Data;

public:
    typedef std::vector<T> Row;
    typedef typename Data::const_iterator IteratorByRows;

    Matrix()
    {}

    Matrix(size_t nRows, size_t nColumns)
        : data_(nRows, Row(nColumns, static_cast<T>(0)))
    {}


    size_t nRows() const
    {
        return data_.size();
    }

    size_t nColumns() const
    {
        if (nRows == 0) {
            return 0;
        } else {
            return data_.at(0).size();
        }
    }

    IteratorByRows rowsBegin() const
    {
        return data_.begin();
    }

    IteratorByRows rowsEnd() const
    {
        return data_.end();
    }
    
    const Row& rowAt(size_t index) const
    {
        return data_.at(index);
    }

    void addRow(const Row& row)
    {
        if (!data_.empty()) {
            assert(row.size() == nColumns());
        }
        data_.push_back(row);
    }

private:
    Data data_;
};
#endif

namespace ublas = boost::numeric::ublas;
typedef double ValueType;
typedef ublas::matrix<ValueType> Matrix;
typedef ublas::vector<ValueType> Vector;
typedef Matrix::const_iterator1 ConstIteratorByRows;
typedef Matrix::const_iterator2 ConstIteratorByColumns;
typedef boost::iterator_range<ConstIteratorByColumns> ColumnsRange;

typedef std::vector<size_t> IndicesVector;
typedef IndicesVector::const_iterator IndexIterator;
typedef boost::permutation_iterator<
            ConstIteratorByRows,
            IndexIterator
        > SelectedRowsIteratorBase;

class SelectedRowsIterator : public SelectedRowsIteratorBase {
public:
    SelectedRowsIterator()
        : itByRows_()
    {}

    SelectedRowsIterator(ConstIteratorByRows itByRows, IndexIterator itIndex)
        : SelectedRowsIteratorBase(itByRows, itIndex),
          itByRows_(itByRows)
    {}

    ConstIteratorByRows rowsIterator() const
    {
        return itByRows_ + *(base());
    }

private:
    ConstIteratorByRows itByRows_;
};

typedef boost::iterator_range<SelectedRowsIterator> SelectedRowsRange;

void prepareIndices(IndicesVector* indices,
    size_t maxPossibleIndex = std::numeric_limits<size_t>::max())
{
    BOOST_ASSERT(!indices->empty());
    std::sort(indices->begin(), indices->end());
    indices->erase(std::unique(indices->begin(), indices->end()), indices->end());
    BOOST_ASSERT(indices->back() < maxPossibleIndex);
}

class MatrixWithSelection {
public:
    MatrixWithSelection(const Matrix& matrix, const IndicesVector& rowsIndices)
        : matrix_(matrix), rowsIndices_(rowsIndices)
    {
        prepareIndices(&rowsIndices_);
    }

    void addRow(const Vector&)
    {
        // add row
    }

    SelectedRowsIterator rowBegin() const
    {
        return SelectedRowsIterator(matrix_.begin1(), rowsIndices_.begin());
    }

    SelectedRowsIterator rowEnd() const
    {
        return SelectedRowsIterator(matrix_.begin1(), rowsIndices_.end());
    }

    const Matrix& matrix() const
    {
        return matrix_;
    }

private:
    Matrix matrix_;
    IndicesVector rowsIndices_;
};

Matrix filterColumns(const Matrix& matrix, IndicesVector indicesToSave)
{
    if (indicesToSave.empty()) {
        return Matrix();
    }

    prepareIndices(&indicesToSave);

    Matrix filteredMatrix(matrix.size1(), indicesToSave.size());
    // perform filtering
    return filteredMatrix;
}

template<class Iterator>
std::ostream& printRange(std::ostream& os, Iterator begin, Iterator end)
{
    // std::copy(begin(), end(),
    //    std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(os, " "));
    for (Iterator it(begin); it != end; ++it) {
        os << *it << " ";
    }
    os << "\n";
    return os;
}

template<class Range>
std::ostream& printRange(std::ostream& os, const Range& range)
{
    return printRange(os, range.begin(), range.end());
}

namespace {

int main_()
{
    Matrix matrix(4, 3);
    std::vector<double> linearData;
    for (size_t i = 0; i < matrix.size1() * matrix.size2(); ++i) {
        matrix(i / matrix.size2(), i % matrix.size2()) = static_cast<double>(i);
    }

    IndicesVector indices;
    indices += 0, 1, 3;


    MatrixWithSelection matrixWithSelection(matrix, indices);
    std::cout << "The whole matrix is: \n";
    for (ConstIteratorByRows it(matrixWithSelection.matrix().begin1());
        it != matrixWithSelection.matrix().end1();
        ++it)
    {
        printRange(std::cout, it);
    }
    std::cout << "\n";
    
    for (SelectedRowsIterator it(matrixWithSelection.rowBegin());
        it != matrixWithSelection.rowEnd();
        ++it)
    {
        const ConstIteratorByRows itRow = it.rowsIterator();
        std::cout << "Row index = " << itRow.index1() << "; ";
        printRange(std::cout, itRow);

        if (itRow.index1() != 0) {
            std::cout << "Previous row index is " << (itRow.index1() - 1) << std::endl;
            std::cout << "Here it is: ";
            printRange(std::cout, itRow - 1);
        }
    }

    return 0;
}
}
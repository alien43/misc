#include <vector>
#include <iostream>


struct IndexedElement {
    int element;
    size_t index;
};

template<class T>
class Matrix;

template<class T>
class MatrixLeftProxy {
public:
    MatrixLeftProxy(Matrix<T>* matrix, size_t column)
        : matrix_(matrix), column_(column)
    {}

    T& operator > (size_t row) {
        return matrix_->table_.at(column_).at(row);
    }

    const T& operator > (size_t row) const {
        return matrix_->table_.at(column_).at(row);
    }

private:
    Matrix<T>* matrix_;
    size_t column_;
};

template<class T>
class Matrix {
    typedef MatrixLeftProxy<T> LeftProxy;
public:
    Matrix(size_t nColumns, size_t nRows)
        : table_(nColumns, std::vector<T>(nRows))
    {}

    LeftProxy operator < (size_t column) {
        return LeftProxy(this, column);
    }

private:
    friend class LeftProxy;

    std::vector< std::vector<T> > table_;
};


int main_()
{
    Matrix<int> matrix(3, 4);
    matrix<1>2 = 4;
    std::cout << (matrix<1>1) << " " << (matrix<1>2) << std::endl;
    return 0;

    std::vector<int> a(5, 2);
    for (size_t i = 0; i < 7; ++i) {
        try {
            std::cout << a.at(i) << std::endl;
        } catch (std::exception& ex) {
            std::cerr << "Sorry, no index " << i << "\n";
            std::cerr << ex.what() << "\n";
        }
    }

    return 0;
}

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <exception>
#include <functional>
#include <chrono>
#include <unordered_map>

using std::chrono::system_clock;

std::vector<int> range(size_t size) {
    std::vector<int> result(size);
    std::iota(result.begin(), result.end(), 0);
    return result;
}

struct Cell {
    Cell()
    {}

    Cell(int x, int y) :
        x(x), y(y)
    {}

    int x, y;
};

int l1Distance(const Cell& first, const Cell& second) {
    return std::abs(first.x - second.x) + std::abs(first.y - second.y);
}

template<class T>
bool isWithin(T value, T rangeBegin, T rangeEnd) {
    return rangeBegin <= value && value < rangeEnd;
}

enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
    DIRECTION_END = 4
};

Direction opposite(Direction direction) {
    return Direction((static_cast<int>(direction) + 2) % 4);
}

class Field {
public:
    Field(int width, int height, const std::vector<int>& cells) :
        width_(width),
        height_(height),
        cells_(cells)
    {
        if (cells.size() != width * height) {
            throw std::runtime_error("Cells array size does not correspond to the matrix dimensions");
        }
        if (!std::is_permutation(cells.begin(), cells.end(), range(cells.size()).begin())) {
            throw std::runtime_error("Given cells array is not a permutation");
        }

        std::ptrdiff_t emptyLinearPosition = std::distance(
            cells_.begin(),
            std::find(cells_.begin(), cells_.end(), 0)
            );
        emptyPosition_.x = emptyLinearPosition % height_;
        emptyPosition_.y = emptyLinearPosition / height_;
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    int size() const {
        return width_ * height_;
    }

    int operator() (int x, int y) const {
        return cells_.at(y * height_ + x);
    }

    int operator() (const Cell& cell) const {
        return (*this)(cell.x, cell.y);
    }

    Cell emptyPosition() const {
        return emptyPosition_;
    }

    bool moveEmpty(Direction direction) {
        if (direction >= DIRECTION_END) {
            throw std::runtime_error("Invalid direction");
        }

        Cell newEmptyPosition = emptyPosition_;
        newEmptyPosition.x += DX[direction];
        newEmptyPosition.y += DY[direction];
        if (!isWithin(newEmptyPosition.x, 0, width_) ||
            !isWithin(newEmptyPosition.y, 0, height_))
        {
            return false;
        }

        std::swap(at(emptyPosition_), at(newEmptyPosition));
        emptyPosition_ = newEmptyPosition;
        return true;
    }

    int toPosition(const Cell& cell) const {
        return cell.x + cell.y * height();
    }

    typedef std::vector<int>::const_iterator const_iterator;

    const_iterator begin() const {
        return cells_.begin();
    }

    const_iterator end() const {
        return cells_.end();
    }

private:
    static const int DX[4];
    static const int DY[4];

    int& at(size_t x, size_t y) {
        return cells_.at(y * height_ + x);
    }

    int& at(const Cell& cell) {
        return at(cell.x, cell.y);
    }

    int width_;
    int height_;
    std::vector<int> cells_;
    Cell emptyPosition_;
};

bool operator == (const Field& first, const Field& second) {
    return first.width() == second.width() && first.height() == second.height() &&
        std::equal(first.begin(), first.end(), second.begin());
}


std::ostream& operator << (std::ostream& os, const Field& field) {
    for (int y = 0; y < field.height(); ++y) {
        for (int x = 0; x < field.width(); ++x) {
            os << field(x, y) << " ";
        }
        os << "\n";
    }
    return os;
}

const int Field::DX[4] = {0, 1, 0, -1};
const int Field::DY[4] = {-1, 0, 1, 0};



Field generateRandomField(int width, int height) {
    std::vector<int> cells = range(width * height);
    std::random_shuffle(cells.begin(), cells.end());
    return Field(width, height, cells);
}

class PuzzleSolver {
public:
    explicit PuzzleSolver(const Field& field) :
        field_(field), coeffs_(field.size())
    {
        std::generate(coeffs_.begin(), coeffs_.end(), std::rand);
    }

    std::vector<Direction> solve() {
        estimations_.push_back(estimate(field_));
        searchStatus_ = IN_PROGRESS;
        startTime_ = system_clock::now();

        hashValue_ = hash(field_);
        visit();
        tryImprove();
        if (searchStatus_ == IN_PROGRESS) {
            searchStatus_ = NOT_FOUND;
        }
        return moves_;
    }

private:
    void tryImprove() {
        if (estimations_.back() == 0) {
            searchStatus_ = FOUND;
        }

        ++operationsPerformed_;
        stopOnTimeLimit();
        for (int intDirection = 0; intDirection < DIRECTION_END; ++intDirection) {
            if (searchStatus_ != IN_PROGRESS) {
                break;
            }

            const Direction direction = static_cast<Direction>(intDirection);
            const Cell emptyPosition = field_.emptyPosition();
            if (field_.moveEmpty(direction)) {
                

                const int value = field_(emptyPosition);
                const Cell targetCell = solutionPosition(value, field_);
                estimations_.push_back(
                    estimations_.back()
                    - l1Distance(targetCell, field_.emptyPosition())
                    + l1Distance(targetCell, emptyPosition)
                    );
                moves_.push_back(direction);
                tryImprove();
                if (searchStatus_ == FOUND) {
                    return;
                }
                moves_.pop_back();
                estimations_.pop_back();
                field_.moveEmpty(opposite(direction));
            }
        }
    }

    void stopOnTimeLimit() {
        if (operationsPerformed_ % 100000 != 0) {
            return;
        }
        if (startTime_ + std::chrono::seconds(10) >= system_clock::now()) {
            searchStatus_ = TIME_LIMIT_EXCEEDED;
        }
    }

    static Cell solutionPosition(int value, const Field& field) {
        if (value == 0) {
            value = field.size();
        }
        --value;
        return Cell(value % field.height(), value / field.height());
    }

    static int estimate(const Field& field) {
        int estimation = 0;
        for (int y = 0; y < field.height(); ++y) {
            for (int x = 0; x < field.width(); ++x) {
                const int value = field(x, y);
                if (value != 0) {
                    estimation += l1Distance(Cell(x, y), solutionPosition(value, field));
                }
            }
        }
        return estimation;
    }

    int hash(const Field& field) const {
        return std::inner_product(field.begin(), field.end(), coeffs_.begin(), 0);
    }

    bool visit() {
        auto& list = visited_[hashValue_];
        if (std::find(list.begin(), list.end(), field_) == list.end()) {
            list.push_back(field_);
            return true;
        }
        return false;
    }

    Field field_;
    std::vector<int> coeffs_;
    std::vector<Direction> moves_;
    std::vector<int> estimations_;
    int hashValue_;

    std::unordered_map<int, std::list<Field>> visited_;

    enum SearchStatus {
        IN_PROGRESS,
        FOUND,
        NOT_FOUND,
        TIME_LIMIT_EXCEEDED
    } searchStatus_;
    int operationsPerformed_;
    system_clock::time_point startTime_;

};

std::vector<Direction> solvePuzzle(const Field& field) {
    return PuzzleSolver(field).solve();
}

void printSolution(Field field, const std::vector<Direction>& moves) {
    for (Direction move: moves) {
        field.moveEmpty(move);
        std::cout << field << std::endl;
    }
}

int main() {
    std::srand(1543);
    Field field = generateRandomField(4, 4);

    std::cout << field << std::endl;
    printSolution(field, solvePuzzle(field));

    return 0;
}
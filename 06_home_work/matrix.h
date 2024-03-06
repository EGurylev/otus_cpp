#include <memory>
#include <unordered_map>

template <typename T, T DefaultValue>
class MatrixImpl
    : public std::enable_shared_from_this<MatrixImpl<T, DefaultValue>> {
  private:
    class Element {
      public:
        Element(T value, size_t row, size_t col,
                std::shared_ptr<MatrixImpl> matrix)
            : value_{value}, row_{row}, col_{col}, matrix_{matrix} {}

        void operator=(const T &new_value) {
            if (value_ != DefaultValue && new_value != DefaultValue) {
                matrix_->rows_[row_].get(col_) = new_value;
            } else if (value_ != DefaultValue && new_value == DefaultValue) {
                matrix_->erase(row_, col_);
            } else if (value_ == DefaultValue && new_value != DefaultValue) {
                if (!matrix_->has_row(row_)) {
                    matrix_->add_row(row_);
                }
                matrix_->push(row_, col_, new_value);
            }
        }

        operator T() { return value_; }

      private:
        T value_{};
        size_t row_{};
        size_t col_{};
        std::shared_ptr<MatrixImpl> matrix_{};
    };

  public:
    class Row {
      public:
        Row() = default;

        Row(size_t row, std::shared_ptr<MatrixImpl> matrix)
            : row_{row}, matrix_{matrix} {}

        Element operator[](size_t col) {
            return Element(matrix_->get(row_, col), row_, col, matrix_);
        }

        void insert(size_t col, T value) { cols_[col] = value; }

        void set_ind(size_t row) { row_ = row; }

        T &get(size_t col) { return cols_[col]; }

        void erase(size_t col) { cols_.erase(col); }

        bool empty() const { return cols_.empty(); }

        auto find(size_t col) const { return cols_.find(col); }

        auto begin() { return cols_.begin(); }

        auto end() { return cols_.end(); }

        bool has_matrix() { return matrix_.get() != nullptr; }

      private:
        size_t row_{};
        std::shared_ptr<MatrixImpl> matrix_;
        std::unordered_map<size_t, T> cols_;
    };

    MatrixImpl() { static_assert(std::is_integral_v<std::decay_t<T>>); }

    auto rows_begin() { return rows_.begin(); }
    auto cols_begin() {
        if (rows_.empty()) {
            return dummy_row_.begin();
        }
        return rows_.begin()->second.begin();
    }

    auto rows_end() { return rows_.end(); }
    auto cols_end() {
        if (rows_.empty()) {
            return dummy_row_.end();
        }
        return rows_.end()->second.end();
    }

    size_t size() const { return sz_; }

    bool empty() const { return sz_ == 0; }

    Row &get_row(size_t row) {
        if (auto row_it{rows_.find(row)}; row_it != rows_.end()) {
            return row_it->second;
        }
        if (!dummy_row_.has_matrix()) {
            dummy_row_ = Row(row, this->shared_from_this());
        }
        dummy_row_.set_ind(row);
        return dummy_row_;
    }

  private:
    bool has_row(size_t row) const {
        if (rows_.find(row) != rows_.end()) {
            return true;
        }
        return false;
    }

    void add_row(size_t row) {
        rows_[row] = Row(row, this->shared_from_this());
    }

    void erase(size_t row, size_t col) {
        if (auto row_it{rows_.find(row)}; row_it != rows_.end()) {
            if (auto col_it{row_it->second.find(col)};
                col_it != row_it->second.end()) {
                row_it->second.erase(col);
                --sz_;
                if (row_it->second.empty()) {
                    rows_.erase(row);
                }
            }
        }
    }

    T get(size_t row, size_t col) {
        if (auto row_it{rows_.find(row)}; row_it != rows_.end()) {
            if (auto col_it{row_it->second.find(col)};
                col_it != row_it->second.end()) {
                return col_it->second;
            }
        }
        return DefaultValue;
    }

    void push(size_t row, size_t col, T value) {
        if (auto row_it{rows_.find(row)}; row_it != rows_.end()) {
            row_it->second.insert(col, value);
        } else {
            rows_[row] = Row(row, this->shared_from_this());
            rows_[row].insert(col, value);
        }
        ++sz_;
    }
    size_t sz_{};
    std::unordered_map<size_t, Row> rows_;
    Row dummy_row_{};
};

template <typename T, T DefaultValue> class Matrix {

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::tuple<size_t, size_t, T>;

        using RowIter = typename std::unordered_map<
            size_t, typename MatrixImpl<T, DefaultValue>::Row>::iterator;
        using ColIter = typename std::unordered_map<size_t, T>::iterator;
        using MatrixIter = std::pair<RowIter, ColIter>;

        iterator(MatrixIter it,
                 std::shared_ptr<MatrixImpl<T, DefaultValue>> matrix)
            : it_{it}, matrix_{matrix} {}

        const value_type operator*() const noexcept {
            return {it_.first->first, it_.second->first, it_.second->second};
        }
        value_type operator*() noexcept {
            return {it_.first->first, it_.second->first, it_.second->second};
        }
        const iterator &operator++() {
            auto &row_it{it_.first};
            auto &col_it{it_.second};
            ++col_it;
            if (col_it == row_it->second.end()) {
                ++row_it;
                if (row_it != matrix_->rows_end()) {
                    col_it = row_it->second.begin();
                }
            }
            return *this;
        }
        friend bool operator==(iterator a, iterator b) {
            return a.it_ == b.it_;
        }
        friend bool operator!=(iterator a, iterator b) { return !(a == b); }

      private:
        MatrixIter it_;
        std::shared_ptr<MatrixImpl<T, DefaultValue>> matrix_;
    };

  public:
    Matrix() : matrix_impl_{std::make_shared<MatrixImpl<T, DefaultValue>>()} {}

    auto &operator[](size_t row) { return matrix_impl_->get_row(row); }

    size_t size() const { return matrix_impl_->size(); }

    bool empty() const { return matrix_impl_->empty(); }

    iterator begin() {
        auto it_pair{std::make_pair(matrix_impl_->rows_begin(),
                                    matrix_impl_->cols_begin())};
        return iterator(it_pair, matrix_impl_);
    }

    iterator end() {
        auto it_pair{
            std::make_pair(matrix_impl_->rows_end(), matrix_impl_->cols_end())};
        return iterator(it_pair, matrix_impl_);
    }

  private:
    std::shared_ptr<MatrixImpl<T, DefaultValue>> matrix_impl_;
};
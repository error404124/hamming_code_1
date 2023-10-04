#ifndef UNTITLED2_MATRIX_HPP
#define UNTITLED2_MATRIX_HPP

#include <iomanip>
#include <iostream>
#include <vector>
#include <cmath>


template<typename T>
class Matrix {
    size_t rows;                      // количество строк
    size_t cols;                      // количество столбцов
    std::vector<std::vector<T>> data; // данные матрицы

public:
    Matrix() : rows(0), cols(0) { // Конструктор пустой матрицы
    }

    Matrix(std::size_t rows, std::size_t cols)
            : cols(cols), rows(rows) { // Конструктор матрицы заданных размеров
        data.resize(rows);
        for (std::size_t i = 0; i < rows; ++i) {
            data[i].resize(cols);
        }
    }

    Matrix(const std::vector<std::vector<T>> &v) { // Конструктор матрицы из вектора векторов
        data = v;
        rows = v.size();
        if (rows != 0) {
            cols = v[0].size();
        } else {
            cols = 0;
        }
        for (std::size_t i = 0; i < v.size(); i++) {
            if (cols != v[i].size()) {
                throw std::runtime_error("Размеры не совпадают");
            }
        }
    }

    Matrix(const Matrix<T> &other) : data(other.data), cols(other.cols), rows(other.rows) { // Конструктор копирования
    }

    Matrix<T> &operator=(const Matrix<T> &other) { // Оператор присваивания
        data = other.data;
        rows = other.rows;
        cols = other.cols;
        return *this;
    }

    T &operator()(std::size_t i, std::size_t j) { // Доступ к элементу (i, j)
        if (i >= rows || j >= cols) {
            throw std::runtime_error("Вышли за предел матрицы");
        }
        return data[i][j];
    }

    const T &operator()(std::size_t i, std::size_t j)
    const { // Доступ к элементу (i, j) для константной матрицы
        if (i >= rows || j >= cols) {
            throw std::runtime_error("Вышли за предел матрицы");
        }
        return data[i][j];
    }

    size_t get_rows() const { // Получение количества строк
        return rows;
    }

    size_t get_cols() const { // Получение количества столбцов
        return cols;
    }

    bool operator==(const Matrix<T> &other) const { // Сравнение матриц
        return (rows == other.rows && cols == other.cols && data == other.data);
    }

    bool operator!=(const Matrix<T> &other) const { // Сравнение матриц
        return !(*this == other);
    }

    Matrix<T> operator*(const T &x) const { // Умножение матрицы на элемент
        Matrix<T> mult_const(rows, cols);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; j++) {
                mult_const.data[i][j] = data[i][j] * x;
            }
        }
        return mult_const;
    }

    Matrix<T> operator+(const Matrix<T> &other) const { // Сложение матриц
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Размеры не совпадают");
        }
        Matrix<T> sum(rows, cols);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                sum.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return sum;
    }

    Matrix<T> operator-(const Matrix<T> &other) const { // Вычитание матриц
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Размеры не совпадают");
        }
        Matrix<T> sub(rows, cols);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                sub.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return sub;
    }

    Matrix<T> operator-() const { // Отрицание матрицы
        Matrix<T> negation(rows, cols);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                negation.data[i][j] = data[i][j] * (-1);
            }
        }
        return negation;
    }

    Matrix<T> operator*(const Matrix<T> &other) const { // Умножение матриц
        if (cols != other.rows) {
            throw std::runtime_error("Размеры не совпадают");
        }
        Matrix<T> mult(rows, other.cols);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < other.cols; ++j) {
                for (std::size_t k = 0; k < cols; k++) {
                    mult.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return mult;
    }

    Matrix<T> transpose() const { // Транспонирование матрицы
        Matrix<T> transp(cols, rows);
        for (std::size_t i = 0; i < rows; ++i) {
            for (std::size_t j = 0; j < cols; ++j) {
                transp.data[j][i] = data[i][j];
            }
        }
        return transp;
    }

    // Получение подматрицы, начиная с позиции (row, col) и размерами (rows,  cols)
    Matrix<T> submatrix(std::size_t row, std::size_t col, std::size_t rows, std::size_t cols) const {
        if (row + rows > this->rows || col + cols > this->cols) {
            throw std::runtime_error("Подматрица выходит за границы");
        }

        Matrix<T> sub(rows, cols);
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                sub.data[i][j] = data[i + row][j + col];
            }
        }

        return sub;
    }

    static Matrix<T> eye(std::size_t n) { // Создание единичной матрицы
        Matrix<T> single(n, n);
        for (std::size_t i = 0; i < n; i++) {
            single.data[i][i] = 1;
        }
        return single;
    }

    static Matrix<T> zeros(std::size_t rows,
                           std::size_t cols) { // Создание нулевой матрицы
        return Matrix<T>(rows, cols);
    }

    Matrix<T> horizontal_concatenate(
            const Matrix<T> &other) const { // Конкатенация матриц по горизонтали
        if (rows != other.rows) {
            throw std::runtime_error("Размеры не совпадают");
        }
        Matrix<T> hor_conc(rows, cols + other.cols);
        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols + other.cols; j++) {
                if (j < cols) {
                    hor_conc.data[i][j] = data[i][j];
                } else {
                    hor_conc.data[i][j] = other.data[i][j - cols];
                }
            }
        }
        return hor_conc;
    }

    Matrix<T> vertical_concatenate(
            const Matrix<T> &other) const { // Конкатенация матриц по вертикали
        if (cols != other.cols) {
            throw std::runtime_error("Размеры не совпадают");
        }
        Matrix<T> vert_conc(rows + other.rows, cols);
        for (std::size_t i = 0; i < rows + other.rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                if (i < rows) {
                    vert_conc.data[i][j] = data[i][j];
                } else {
                    vert_conc.data[i][j] = other.data[i - rows][j];
                }
            }
        }
        return vert_conc;
    }

public:  // «Красивый» вывод матрицы
    template<typename Tstream>
    friend std::ostream &operator<<(std::ostream &out, const Matrix<Tstream> &m);

};

template<typename Tstream>
std::ostream &operator<<(std::ostream &out, const Matrix<Tstream> &m) {
    const int MAX_NUM_DIGITS = 5;
    out << std::endl;
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            out << std::setw(MAX_NUM_DIGITS) << m(i, j) << " ";
        }
        out << std::endl;
    }
    return out;
}

#endif


#ifndef UNTITLED2_HAMMING_CODE_HPP
#define UNTITLED2_HAMMING_CODE_HPP

#include <iomanip>
#include <iostream>
#include <vector>
#include <cmath>
#include "matrix.hpp"
#include "Fp.hpp"

template<int p>
class HammingCode {
public:
    explicit HammingCode(size_t m) : m(m) { // Конструктоp
        auto A = create_A();
        k = A.get_cols();
        n = m + k;
        H = create_H(A);
        G = create_G(A);
    }

    const Matrix<Fp<p>> &get_H() {
        return H;
    }

    const Matrix<Fp<p>> &get_G() {
        return G;
    }

    // Методы
    Matrix<Fp<p>> encode(const Matrix<Fp<p>> &data) const {
        return data * G;
    }

    Matrix<Fp<p>> decode(const Matrix<Fp<p>> &codeword) const {
        Matrix<Fp<p>> syndrome = calculate_syndrome(codeword); //Вычисляю синдром

        size_t i = 0;
        for (; i < H.get_cols(); ++i) {
            if (isCollinear(H, i, syndrome)) {  //Нахожу в А коллинеарный с синдромом стобец запсиываю в i
                break;
            }
        }

        size_t j = 0;
        for (; j < syndrome.get_rows(); ++j) {  //Нахожу первый ненулевой элемент в синдроме и записываю его в j
            if (syndrome(j, 0) != 0) {
                break;
            }
        }
        if (j == syndrome.get_rows()) {
            return codeword.submatrix(0, 0, 1, k); //если не нашел ненулевой, то ошибки нет
        }

        Fp<p> alpha = syndrome(j, 0); //В столбцах H первый ненулей элемент будет 1, alpha = коэфф коллинеарности со стобцом H

        Matrix<Fp<p>> result(codeword); //Копирую кодовое слово как матрицу

        result(0, i) -= alpha; //По лабе вычитаю альфа из результата

        return result.submatrix(0, 0, 1, k); //вырезаю дополнительные проверочные столбцы
    }

    // Добавляет случайную ненулевую ошибку в случайную или error_pos позицию
    static Matrix<Fp<p>> add_error(const Matrix<Fp<p>> &codeword, size_t error_pos = -1) {
        if (error_pos == -1) {
            error_pos = rand() % codeword.get_cols(); //Если не передается, то мы ставим в рандомное место ошибку
        }

        Matrix<Fp<p>> copy(codeword);

        Fp<p> error(rand() % (p - 1) + 1); //добавляем ошибку минимум 1

        copy(0, error_pos) += error; //добавляем ошибку к столбцу
        return copy;
    }

private:
    // Атрибуты
    size_t n;    // Количество символов в кодовом слове
    size_t k;    // Количество информационных символов
    size_t m;    // Количество проверочных символов
    Matrix<Fp<p>> H; // Проверочная матрица
    Matrix<Fp<p>> G; // Порождающая матрица


    // Вспомогательные методы

    Matrix<Fp<p>> create_A() {
        Matrix<Fp<p>> A(m, 0);
        Matrix<Fp<p>> V(m, 1);

        do {
            bool flag = true;
            for (size_t i = 0; i < A.get_cols(); ++i) {
                if (isCollinear(A, i, V)) {
                    flag = false;
                    break;
                }
            }

            if (count_zeros(V) == m || count_zeros(V) == m - 1) {
                flag = false;
            }

            if (flag) {
                A = A.horizontal_concatenate(V);
            }

        } while (plus_one(V, 0));

        return A;
    }

    Matrix<Fp<p>> create_H(Matrix<Fp<p>> &A) {
        return A.horizontal_concatenate(Matrix<Fp<p>>::eye(m));
    }

    Matrix<Fp<p>> create_G(Matrix<Fp<p>> &A) {
        return Matrix<Fp<p>>::eye(k).horizontal_concatenate(-A.transpose());
    }

    static bool plus_one(Matrix<Fp<p>> &U, size_t cur_row) {
        if (cur_row == U.get_rows()) {
            return false;
        }

        if (plus_one(U, cur_row + 1)) {
            return true;
        }

        U(cur_row, 0) += Fp<p>(1);

        if (U(cur_row, 0) == Fp<p>(0)) {
            return false;
        }

        return true;
    }

    bool isCollinear(const Matrix<Fp<p>> &A, size_t col, const Matrix<Fp<p>> &B) const {
        for (size_t i = 1; i < p; ++i) {
            bool flag = true;
            for (size_t j = 0; j < A.get_rows(); ++j) {
                if (B(j, 0) * Fp<p>(i) != A(j, col)) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                return true;
            }
        }

        return false;
    }

    int count_zeros(Matrix<Fp<p>> &U) {
        int total = 0;
        for (size_t i = 0; i < U.get_rows(); ++i) {
            if (U(i, 0) == 0) {
                ++total;
            }
        }
        return total;
    }

    // Вспомогательные методы
    Matrix<Fp<p>> calculate_syndrome(const Matrix<Fp<p>> &codeword) const {
        return H * codeword.transpose();
    }
};

#endif

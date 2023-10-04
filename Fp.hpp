#ifndef UNTITLED2_FP_HPP
#define UNTITLED2_FP_HPP

#include <iomanip>
#include <iostream>
#include <vector>
#include <cmath>

template<int p>
class Fp {
public:
    Fp(int x = 0) { // Конструктор
        if (x < 0) {
            value = p + x % p;
        } else {
            value = x % p;
        }
    }

    int val() const { // Возвращает значение элемента поля
        return value;
    }

    Fp<p> &operator=(int x) { // Операция присваивания от целого числа
        if (x < 0) {
            value = p + x % p;
        } else {
            value = x % p;
        }
        return *this;
    }

    Fp<p> operator+(const Fp<p> &other) const { // Операция сложения
        return Fp<p>(value + other.value);
    }

    Fp<p> operator-(const Fp<p> &other) const { // Операция вычитания
        return Fp<p>(value - other.value);
    }

    Fp<p> operator*(const Fp<p> &other) const { // Операция умножения
        return Fp<p>(value * other.value);
    }

    Fp<p> operator/(const Fp<p> &other) const { // Операция деления
        Fp<p> reverse;
        if (other.value == 0) {
            throw std::runtime_error("Нет обратного");
        }
        for (int i = 0; i < p; ++i) {
            if ((other.value * i) % p == 1) {
                reverse = i;
            }
        }
        return reverse;
    }

    Fp<p> operator-() const { // Операция смены знака
        return Fp<p>(-value);
    }

    Fp<p> &operator+=(const Fp<p> &other) { // Операция += (сложение с присваиванием)
        return *this = (value + other.value);
    }

    Fp<p> &operator-=(const Fp<p> &other) { // Операция -= (вычитание с присваиванием)
        return *this = (value - other.value);
    }

    Fp<p> &operator*=(const Fp<p> &other) { // Операция *= (умножение с присваиванием)
        return *this = (value * other.value);
    }

    Fp<p> &operator/=(const Fp<p> &other) { // Операция /= (деление с присваиванием)
        return *this = *this / other;
    }

    bool operator==(const Fp<p> &other) const { // Операция сравнения на равенство
        return value == other.value;
    }

    bool operator!=(const Fp<p> &other) const { // Операция сравнения на неравенство
        return value != other.value;
    }

    template<int Tstream>
    friend std::ostream &operator<<(std::ostream &out, const Fp<Tstream> &m);

private:
    int value; // Значение элемента поля
};

template<int Tstream>
std::ostream &operator<<(std::ostream &out, const Fp<Tstream> &m) {
    out << m.val();
    return out;
}

#endif
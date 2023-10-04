#include "Fp.hpp"
#include "matrix.hpp"
#include "Hamming_code.hpp"
#include <vector>

int main() {
    {
        HammingCode<2> Test(3);
        Matrix<Fp<2>> a{
                std::vector<std::vector<Fp<2>>>{
                        {1, 0, 0, 0}
                }
        };

        for (int i = 0; i < 100; i++) {
            a = Test.add_error(a);
            auto res = Test.encode(a);
            res = Test.add_error(res);
            if (Test.decode(res) != a) {
                throw std::runtime_error("Не раскодировалось");
            }
        }
    }

    {
        HammingCode<3> Test(2);
        Matrix<Fp<3>> a{
                std::vector<std::vector<Fp<3>>>{
                        {1, 0}
                }
        };

        for (int i = 0; i < 100; i++) {
            a = Test.add_error(a);
            auto res = Test.encode(a);
            res = Test.add_error(res);
            if (Test.decode(res) != a) {
                throw std::runtime_error("Не раскодировалось");
            }
        }
    }


}
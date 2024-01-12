#include "ip_adress_pool.h"

int main(int argc, char const *argv[])
{
    try {
        IPAdressPool pool;
        pool.read();
        pool.sort_reverse().print();

        auto pool1(pool);
        pool1.filter([](const IPAdress& adress) {return adress.front() == 1U;}).print();

        auto pool2(pool);
        pool2.filter([](const IPAdress& adress) {
            if (adress[0] == 46U && adress[1] == 70U) {
                return true;
            }
            return false;
        }).print();

        auto pool3(pool);
        pool3.filter([](const IPAdress& adress) {
            return std::find(adress.begin(), adress.end(), 46) != adress.end();
        }).print();
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }    

    return 0;
}

#include "ip_adress_pool.h"

int main(int argc, char const *argv[])
{
    try {
        IPAdressPool pool;
        pool.read(std::cin);
        std::cout << pool.sort_reverse();

        std::cout << pool.filter([](const IPAdress& adress)
            {return adress.front() == 1U;});

        std::cout << pool.filter([](const IPAdress& adress) {
            if (adress[0] == 46U && adress[1] == 70U) {
                return true;
            }
            return false;
        });

        std::cout << pool.filter([](const IPAdress& adress) {
            return std::find(adress.begin(), adress.end(), 46) != adress.end();
        });
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }    

    return 0;
}

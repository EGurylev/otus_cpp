#include <sstream>
#include <limits>
#include "ip_adress_pool.h"

std::ostream& operator<<(std::ostream& os, const IPAdress& arr) {
    for (int i = 0; i < 3; ++i) {
        os << static_cast<int>(arr[i]) << ".";
    }
    os << static_cast<int>(arr.back());
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<IPAdress>& pool) {
    for (const auto& adress : pool) {
        os << adress << "\n";
    }
    return os;
}

void IPAdressPool::read(std::istream& is) {
    for(std::string line; std::getline(is, line);)
    {
        const auto adress = convert(line.substr(0, line.find_first_of("\t")));
        pool_.push_back(adress);
    }
}

const std::vector<IPAdress>& IPAdressPool::sort_reverse() {
    std::sort(pool_.begin(), pool_.end(), std::greater<IPAdress>());
    return pool_;
}

std::vector<IPAdress>
IPAdressPool::filter(std::function<bool(const IPAdress&)> pred) {
    std::vector<IPAdress> pool_filtered;
    pool_filtered.reserve(pool_.size());
    for (const auto& adress : pool_) {
        if (pred(adress)) {
            pool_filtered.push_back(adress);
        }
    }
    return pool_filtered;
}

IPAdress IPAdressPool::convert(std::string&& str) {
    IPAdress adress;
    std::istringstream is(str);
    std::string octet;
    for (int i = 0; i < 4; ++i) {
        getline(is, octet, '.');
        int number = stoi(octet);
        if (number > std::numeric_limits<uint8_t>::max()) {
            throw std::overflow_error("Value " + std::to_string(number) + \
             " is outside of 0-255 range");
        }
        adress[i] = static_cast<uint8_t>(number);
    }
    return adress;
}

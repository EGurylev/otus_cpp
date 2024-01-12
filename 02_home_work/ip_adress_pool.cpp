#include "ip_adress_pool.h"

std::ostream& operator<<(std::ostream& os, const IPAdress& arr) {
    for (int i = 0; i < 3; ++i) {
        os << static_cast<int>(arr[i]) << ".";
    }
    os << static_cast<int>(arr.back());
    return os;
}

void IPAdressPool::read() {
    for(std::string line; std::getline(std::cin, line);)
    {
        const auto adress = convert(line);
        push(adress);
    }
}

IPAdressPool& IPAdressPool::sort_reverse() {
    std::sort(pool_.begin(), pool_.end(), [](IPAdress& a, IPAdress& b) {
        return a > b;
    });
    return *this;
}

IPAdressPool& IPAdressPool::filter(std::function<bool(const IPAdress&)> pred) {
    std::vector<IPAdress> pool_filtered;
    pool_filtered.reserve(pool_.size());
    for (const auto& adress : pool_) {
        if (pred(adress)) {
            pool_filtered.push_back(adress);
        }
    }
    std::swap(pool_filtered, pool_);
    return *this;
}

void IPAdressPool::print() {
    for (const auto& adress : pool_) {
        std::cout << adress << "\n";
    }
}

IPAdress IPAdressPool::convert(std::string& str) {
    auto ip = str.substr(0, str.find_first_of("\t"));

    IPAdress r;

    std::string::size_type start = 0;
    char delimiter {'.'};
    std::string::size_type stop = str.find_first_of(delimiter);
    size_t i {};
    for (size_t i = 0; i < 3; ++i)
    {
        r[i] = static_cast<uint8_t>(stoi(ip.substr(start, stop - start)));

        start = stop + 1;
        stop = str.find_first_of(delimiter, start);
    }

    r.back() = static_cast<uint8_t>(stoi(ip.substr(start)));

    return r;
}

void IPAdressPool::push(const IPAdress& adress) {
    pool_.push_back(adress);
}
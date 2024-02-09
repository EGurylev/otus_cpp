#pragma once
#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <vector>

using IPAdress = std::array<uint8_t, 4>;

std::ostream& operator<<(std::ostream& os, const IPAdress& arr);

std::ostream& operator<<(std::ostream& os, const std::vector<IPAdress>& pool);

class IPAdressPool {
   public:
    void read(std::istream& is);
    const std::vector<IPAdress>& sort_reverse();
    std::vector<IPAdress> filter(std::function<bool(const IPAdress&)> pred);
    IPAdressPool& print();

   private:
    std::optional<IPAdress> convert(std::string&& str);
    void push(const IPAdress& adress);
    std::vector<IPAdress> pool_;
};

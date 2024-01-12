#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

using IPAdress = std::array<uint8_t, 4>;

std::ostream& operator<<(std::ostream& os, const IPAdress& arr);

class IPAdressPool {
public:
    void read();
    IPAdressPool& sort_reverse();
    IPAdressPool& filter(std::function<bool(const IPAdress&)> pred);
    void print();  

private:
    IPAdress convert(std::string& str);
    void push(const IPAdress& adress);
    std::vector<IPAdress> pool_;
};
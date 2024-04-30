#include "async.h"
#include <thread>

void process_connection1() {
    auto h1 {async::connect(5)};
    async::receive(h1, "h1", 2);
    async::receive(h1, "\nh1_2\nh1_3\nh1_4\nh1_5\nh1_6\n{\nh1_a\n", 33);
    async::receive(h1, "h1_b\nh1_c\nh1_d\n}\nh1_89\n", 23);
    async::disconnect(h1);
}

void process_connection2() {
    auto h2 {async::connect(5)};
    async::receive(h2, "h2_2", 4);
    async::receive(h2, "h2_12\nh2_13\nh2_14\nh2_15\nh2_16\n{\nh2_a2\n", 38);
    auto h3 {async::connect(2)};
    async::receive(h3, "h3_bh2\nh3_ch2\nh3_dh2\n{\nh3_dd\n", 29);
    async::receive(h2, "h2_b2\nh2_c2\nh2_d2\n}\nh2_892\n", 27);
    async::receive(h3, "}\nh3_ee\nh3_rr\n", 14);
    async::disconnect(h3);
    async::disconnect(h2);
}

int main(int, char *[]) {
    
    auto th1 {std::thread(process_connection1)};
    auto th2 {std::thread(process_connection2)};

    th1.join();
    th2.join();

    return 0;
}
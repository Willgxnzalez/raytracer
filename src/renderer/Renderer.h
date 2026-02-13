#pragma once

#include <iostream>
#include <thread>

int run() {
    int threads = std::thread::hardware_concurrency();
    std::cout << "thread count: " << threads << std::endl;
}
#ifndef GENIITEAMBUILDER_TIMER_H
#define GENIITEAMBUILDER_TIMER_H

#include <chrono>
#include <functional>
#include <iostream>

std::chrono::milliseconds  timeFunction(
        auto function
){
    auto start = std::chrono::high_resolution_clock::now();
    function();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    return duration;
}

void logFunctionTime(
        auto function,
        const std::string& messageHeader
){
    auto duration = timeFunction<>(function);
    std::cout << messageHeader << duration.count() << std::endl;
}

#endif //GENIITEAMBUILDER_TIMER_H

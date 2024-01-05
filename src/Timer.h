#ifndef GENIITEAMBUILDER_TIMER_H
#define GENIITEAMBUILDER_TIMER_H

#include <chrono>
#include <functional>

template <typename R>
std::pair<std::chrono::milliseconds, R>  timeFunction(
        auto function
){
    auto start = std::chrono::high_resolution_clock::now();
    R returnValue = function();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    return {duration, returnValue};
}

auto logFunctionTime(
        auto function,
        const std::string& messageHeader
){
    auto result = timeFunction<std::string>(function);
    auto duration = result.first;
    std::cout << messageHeader << duration.count() << std::endl;
    return result.second;
}

#endif //GENIITEAMBUILDER_TIMER_H

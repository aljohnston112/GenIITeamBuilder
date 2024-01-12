#ifndef GENIITEAMBUILDER_THREADPOOL_H
#define GENIITEAMBUILDER_THREADPOOL_H

#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>

namespace BattleConfig{
    using BattleFunction = std::function<void(void)>;
}

using namespace BattleConfig;

class ThreadPool {

    const static unsigned int numThreads;
    std::vector<std::jthread> threads{};

    std::mutex mutex;
    std::queue<BattleFunction> tasks;

    std::condition_variable task_condition;
    bool stop = false;

public:
    ThreadPool();

    void addTasks(const std::vector<BattleFunction>& functions);

    ~ThreadPool();

};


#endif //GENIITEAMBUILDER_THREADPOOL_H

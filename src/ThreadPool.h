#ifndef GENIITEAMBUILDER_THREADPOOL_H
#define GENIITEAMBUILDER_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>
#include "Timer.h"

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
    ThreadPool() {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    BattleFunction task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        task_condition.wait(
                                lock, [this] {
                                    return stop || !tasks.empty();
                                }
                                );

                        if (tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    };

    void addTasks(const std::vector<BattleFunction>& functions){
        {
            std::unique_lock<std::mutex> lock(mutex);
            for(const BattleFunction& function: functions) {
                tasks.emplace(function);
            }
        }
        task_condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        task_condition.notify_all();
        for (std::jthread &thread : threads) {
            thread.join();
        }
    }

};


#endif //GENIITEAMBUILDER_THREADPOOL_H

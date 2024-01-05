#ifndef GENIITEAMBUILDER_THREADPOOL_H
#define GENIITEAMBUILDER_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>

namespace BattleConfig{
    using BattleFunction = std::function<void(void)>;
}

using namespace BattleConfig;

class ThreadPool {

    std::vector<std::jthread> threads{std::thread::hardware_concurrency()};

    std::mutex mutex;
    std::queue<BattleFunction> tasks;

    std::condition_variable work_condition;
    bool stop = false;

public:
    ThreadPool() {
        for (size_t i = 0; i < threads.size(); ++i) {
            threads.emplace_back([this] {
                while (true) {
                    BattleFunction task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        work_condition.wait(
                                lock, [this] {
                                    return stop && tasks.empty();
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

    void addTask(BattleFunction function){
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace(std::move(function));
        work_condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        work_condition.notify_all();
        for (std::jthread &thread : threads) {
            thread.join();
        }
    }

};


#endif //GENIITEAMBUILDER_THREADPOOL_H

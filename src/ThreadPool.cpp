#include "ThreadPool.h"

const unsigned int ThreadPool::numThreads = std::thread::hardware_concurrency();


ThreadPool::ThreadPool() {
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

void ThreadPool::addTasks(const std::vector<BattleFunction>& functions){
    {
        std::lock_guard<std::mutex> lock(mutex);
        for(const BattleFunction& function: functions) {
            tasks.emplace(function);
        }
    }
    task_condition.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }
    task_condition.notify_all();
    for (std::jthread &thread : threads) {
        thread.join();
    }
}
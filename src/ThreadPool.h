#ifndef GENIITEAMBUILDER_THREADPOOL_H
#define GENIITEAMBUILDER_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <future>

template<typename F>
class ThreadPool {

    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::jthread> threads{numThreads};
    std::mutex mutex;
    std::queue<F> tasks;
    std::condition_variable work_condition;

    std::condition_variable stop_condition;
    bool stop = false;

public:
    ThreadPool() {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    F task;

                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        work_condition.wait(lock, [this] { return stop && tasks.empty(); });

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

    void addTask(F function){
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

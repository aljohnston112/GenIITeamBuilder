#include "ThreadPool.h"

const unsigned int ThreadPool::numThreads = std::thread::hardware_concurrency();

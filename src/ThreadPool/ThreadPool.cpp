#include <iostream>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(uint32_t thread_number) {
    if(thread_number > std::thread::hardware_concurrency()) {
        throw std::runtime_error("Error : thread_number too high");
    }
    this->thread_number = thread_number;
    stop_thread = false;
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::start() {
    std::cout << "Starting thread pool" << std::endl;
    for(uint32_t i = 0; i < thread_number; i++) {
        std::cout << "Adding thread " << i << std::endl;
        worker_thread.emplace_back(&ThreadPool::ThreadPoolEngine, this);
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_queue);
        stop_thread = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : worker_thread) {
        active_thread.join();
    }
    worker_thread.clear();
}

void ThreadPool::ThreadPoolEngine() {
    while(true) {
        std::function<void()> current_job;
        {
            std::unique_lock<std::mutex> lock(mutex_queue);
            mutex_condition.wait(lock, [this] {
                return !jobs_queue.empty() || stop_thread;
            });
            if(stop_thread && jobs_queue.empty()) {
                return;
            }
            current_job = jobs_queue.front();
            jobs_queue.pop();
        }
        current_job();
    }
}

void ThreadPool::addJob(const std::function<void()>& new_job) {
    {
        std::cout << "Job " << jobs_queue.size() <<  " added to thread pool" << std::endl;
        std::unique_lock<std::mutex> lock(mutex_queue);
        jobs_queue.push(new_job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::isPoolBusy() {
    std::unique_lock<std::mutex> lock(mutex_queue);
    return !jobs_queue.empty();
}

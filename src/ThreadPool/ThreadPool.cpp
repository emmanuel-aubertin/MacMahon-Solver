#include <iostream>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>   // Include for std::function
#include <atomic>       // Include for std::atomic
#include <condition_variable>  // Include for std::condition_variable
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(uint32_t thread_number) {
    if(thread_number > std::thread::hardware_concurrency()){
        throw std::runtime_error("Error : thread_number too high");
    }
    this->thread_number = thread_number;
    stop_thread = false;
}

void ThreadPool::start() {
    for(uint32_t i = 0; i < thread_number; i++){
        worker_thread.push_back(std::thread(&ThreadPool::ThreadPoolEngine, this));
    }
}

void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_queue);
        stop_thread = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : worker_thread) {
        active_thread.join();
    }
    worker_thread.clear();
}

void ThreadPool::ThreadPoolEngine() {
    while (true) {
        std::function<void()> current_job;
        {
            std::unique_lock<std::mutex> lock(mutex_queue);
            mutex_condition.wait(lock, [this] {
                return !jobs_queue.empty() || stop_thread;
            });
            if (stop_thread) {
                return;
            }
            current_job = jobs_queue.front();
            jobs_queue.pop();

            
            
        }
        current_job(); 
       
    }
}

void ThreadPool::addJob(std::function<void()> new_job) {
    {
        std::lock_guard<std::mutex> lock(mutex_queue);
        jobs_queue.push(new_job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::isPoolBusy() {
    bool output;
    {
        std::lock_guard<std::mutex> lock(mutex_queue);
        output = !jobs_queue.empty();
    }
    return output;
}

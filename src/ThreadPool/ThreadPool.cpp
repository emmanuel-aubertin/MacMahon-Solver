#include <iostream>
#include <queue>
#include <thread>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(uint32_t thread_number){
    if(thread_number > thread::hardware_concurrency()){
        std::cerr << "❌ \033[1;31m Error : thread_number too high\033[0m ❌" << endl;
        exit(-1);
    }
    this->thread_number = thread_number;
}

void ThreadPool::start() {
    for(uint32_t i = 0; i < thread_number; i++){
        worker_thread.push_back(thread(&ThreadPool::ThreadPoolEngine,this));
    }

}

void ThreadPool::stop() {
    {
        unique_lock<mutex> lock(mutex_queue);
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
        function<void()> current_job;
        {
            unique_lock<mutex> lock(mutex_queue);
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

void ThreadPool::addJob(const std::function<void()>& new_job){
    {
        std::unique_lock<std::mutex> lock(mutex_queue);
        jobs_queue.push(new_job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::isPoolBusy() {
    bool output;
    {
        unique_lock<mutex> lock(mutex_queue);
        output = !jobs_queue.empty();
    }
    return output;
}
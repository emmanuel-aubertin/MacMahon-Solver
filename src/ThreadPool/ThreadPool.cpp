// Inpiration scource : https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
// Understanding condition_variable : https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/thread/condition_variable.html#:~:text=The%20condition_variable%20class%20is%20a,a%20spurious%20wakeup%20occurs
// It is used to block multiple threads simultaneously. Useful for when a solution is found, allowing you to stop all threads more easily.

#include <iostream>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include "ThreadPool.hpp"

/**
 * @brief Construct a new Thread Pool:: Thread Pool object
 * 
 * @param thread_number 
 * @author Aubertin Emmanuel
 */
ThreadPool::ThreadPool(uint32_t thread_number) {
    if(thread_number > std::thread::hardware_concurrency()) {
        throw std::runtime_error("Error : thread_number too high");
    }
    this->thread_number = thread_number;
    stop_thread = false;
}

/**
 * @brief Destroy the Thread Pool:: Thread Pool object, and join all thread before
 * 
 * @author Aubertin Emmanuel
 */
ThreadPool::~ThreadPool() {
    join();
}

/**
 * @brief Start to execute all thread.
 * 
 * @author Aubertin Emmanuel
 */
void ThreadPool::start() {
    for(uint32_t i = 0; i < thread_number; i++) {
        worker_thread.emplace_back(&ThreadPool::ThreadPoolEngine, this);
    }
}

/**
 * @brief Stopping & join all thread using condition_variable
 * 
 * @author Aubertin Emmanuel
 */
void ThreadPool::join() {
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

/**
 * @brief Asign job to thread.
 * 
 * @author Aubertin Emmanuel
 */
void ThreadPool::ThreadPoolEngine() {
    while(true) {
        std::function<void()> current_job;
        {
            std::unique_lock<std::mutex> lock(mutex_queue);
            // Use to stop all thread at the time, more info in the bellow doc :)
            mutex_condition.wait(lock, [this] {
                return !jobs_queue.empty() || stop_thread.load();
            });
            if(stop_thread && jobs_queue.empty()) {
                return;
            }
            current_job = jobs_queue.front();
            jobs_queue.pop();
        }
        busy_threads_count++;
        current_job();
        busy_threads_count--;
    }
}

/**
 * @brief Add a new job to execute
 * 
 * @param new_job funciton to execute
 */
void ThreadPool::addJob(const std::function<void()>& new_job) {
    {
        std::unique_lock<std::mutex> lock(mutex_queue);
        jobs_queue.push(new_job);
    }
    mutex_condition.notify_one();
}

/**
 * @brief Know if it steal job to process in the queue
 * 
 * @return true if the pool is busy
 * @return false if the pool finish all job
 */
bool ThreadPool::isPoolBusy() {
    std::unique_lock<std::mutex> lock(mutex_queue);
    return !jobs_queue.empty();
}

/**
 * @brief Know if there is some unused thread
 * 
 * @return true if there is some unused thread
 * @return false if all thread are busy
 */
bool ThreadPool::isAllThreadBusy() {
    return busy_threads_count.load() == thread_number;
}
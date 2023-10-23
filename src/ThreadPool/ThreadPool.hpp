#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <condition_variable>


using namespace std;

class ThreadPool
{
    private:
        vector <thread> worker_thread;
        std::atomic<uint32_t> busy_threads_count{0};
        condition_variable mutex_condition;
        uint32_t thread_number;
        std::atomic<bool> stop_thread;
        queue< function<void()> > jobs_queue;
        void ThreadPoolEngine(); // While true listen for events in jobs queue
    public:
        ~ThreadPool();
        mutex mutex_queue;
        ThreadPool(uint32_t);
        void stop();
        void start();
        void addJob(const std::function<void()>&);
        bool isPoolBusy();
        bool isAllThreadBusy() ;
};

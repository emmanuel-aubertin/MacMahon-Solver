#include <iostream>
#include <vector>
#include <queue>
#include <thread>

using namespace std;

class ThreadPool
{
    private:
        vector <thread> worker_thread;
        queue<function<void()>> jobs_queue;
        mutex mutex_queue;
        condition_variable mutex_condition;
        uint32_t thread_number;
        bool stop_thread = false;

        void ThreadPoolEngine(); // While true listen for events in jobs queue
    public:
        ThreadPool(uint32_t);
        void stop();
        void start();
        void addJob(const std::function<void()>&);
        bool isPoolBusy();

};
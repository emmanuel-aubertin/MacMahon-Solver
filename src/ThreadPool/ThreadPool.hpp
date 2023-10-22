#include <iostream>
#include <vector>
#include <queue>
#include <thread>
using namespace std;

class ThreadPool
{
    private:
        vector <thread> worker_thread;
        queue< function<void()> > jobs_queue;
        condition_variable mutex_condition;
        uint32_t thread_number;
        std::atomic<bool> stop_thread = false;

        void ThreadPoolEngine(); // While true listen for events in jobs queue
    public:
        mutex mutex_queue;
        ThreadPool(uint32_t);
        void stop();
        void start();
        void addJob(std::function<void()>);
        bool isPoolBusy();

};
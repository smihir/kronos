#include <chrono>
#include <functional>
#include <future>
#include <module.h>
#include <mutex>
#include <queue>
#include <thread>

#pragma once
namespace kronos {
typedef module::ModuleInterface TaskInterface;
//class TaskInterface : public module::ModuleInterface{
//    public:
//        virtual void run() = 0;
//        virtual ~TaskInterface();
//};

class Scheduler {
    private:
        struct SchedulerTask {
            TaskInterface *task;
            std::chrono::system_clock::time_point exec_time;
            bool is_recurring;
            std::chrono::system_clock::duration interval;
            bool operator<(const struct SchedulerTask &t) const {
                return exec_time < t.exec_time;
            }
            bool operator>(const struct SchedulerTask &t) const {
                return exec_time > t.exec_time;
            }
        };
    public:
        Scheduler() : running(false), stop(false) {}
        ~Scheduler();
        // FIXME: dont use c style pointers, indenitify the transfer of ownership and fix this
        void runEvery(TaskInterface *task, std::chrono::system_clock::duration interval);
        void runAt(TaskInterface *task, std::chrono::system_clock::time_point time);
        void start();
        void schedule();
    private:
        std::thread main_thread;
        bool running;
        bool stop;

        // FIXME: use lock to protect the priority_queue
        //std::mutex wait_lock;
        std::priority_queue<struct SchedulerTask, std::vector<struct SchedulerTask>, std::less<struct SchedulerTask> > wait_q;

        std::vector<std::pair<std::thread, std::future<void> > > active_tasks;
};

} /* namespace kron */

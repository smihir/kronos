#include <chrono>
#include <db.h>
#include <functional>
#include <future>
#include <module.h>
#include <mutex>
#include <queue>
#include <thread>

#pragma once
namespace kronos {
// TaskInterface is implemented by every module and
// provides a set of methods which scheduler executes
// to collect the measurement numbers and persist the
// measured values.
typedef module::ModuleInterface TaskInterface;

class Scheduler {
    private:
        struct SchedulerTask {
            // FIXME: try to use reference instead of pointer
            TaskInterface *task;
            std::chrono::system_clock::time_point exec_time;
            // FIXME: make schedulertask a class and initialize
            // is recurring. also add appropriate constructors
            // to satisfy the init routies in runEvery() and
            // runAt()
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
        // FIXME: dont use c style pointers,
        // identify the transfer of ownership and fix this
        void runEvery(TaskInterface *task,
                std::chrono::system_clock::duration interval);
        void runAt(TaskInterface *task,
                std::chrono::system_clock::time_point time);
        void start();
        void schedule();
    private:
        std::thread main_thread;
        bool running;
        bool stop;

        // FIXME: use lock to protect the priority_queue.
        // write a new class called locked_pq, dont worry about its performance.
        //std::mutex wait_lock;
        std::priority_queue<struct SchedulerTask,
            std::vector<struct SchedulerTask>,
            std::less<struct SchedulerTask> > wait_q;

        std::vector<std::pair<std::thread, std::future<void> > > active_tasks;
};

} /* namespace kron */

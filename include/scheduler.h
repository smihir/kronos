#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#pragma once
namespace kron {
class TaskInterface {
    public:
        virtual void run() = 0;
        virtual ~TaskInterface();
};

class Scheduler {
    private:
        struct SchedulerTask {
            TaskInterface *task;
            std::chrono::system_clock::time_point exec_time;
            bool is_recurring;
            bool operator<(const struct SchedulerTask &t) const {
                return exec_time < t.exec_time;
            }
            bool operator>(const struct SchedulerTask &t) const {
                return exec_time > t.exec_time;
            }
        };
    public:
        Scheduler() : running(false) {}
        ~Scheduler();
        void runEvery(TaskInterface &task, std::chrono::system_clock::duration interval);
        void runAt(TaskInterface &task, std::chrono::system_clock::time_point time);
        void start();
        void schedule();
    private:
        std::thread main_thread;
        bool running;

        std::mutex wait_lock;
        std::priority_queue<struct SchedulerTask, std::vector<struct SchedulerTask>, std::less<struct SchedulerTask> > wait_q;

};

} /* namespace kron */

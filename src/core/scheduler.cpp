#include <scheduler.h>
#include <spdlog/spdlog.h>

using namespace kronos;

auto schedlogger = spdlog::stdout_color_mt("scheduler");

Scheduler::~Scheduler() {
    main_thread.join();

    // wait for worker threads to exit
    while (active_tasks.size() != 0) {
        auto it = active_tasks.begin();
        while(it != active_tasks.end()) {
            if(it->second.wait_for(std::chrono::milliseconds(0)) ==
                    std::future_status::ready) {
                it->first.join();
                it = active_tasks.erase(it);
            } else {
                it++;
            }
        }
    }
}

void Scheduler::start() {
    if (!running) {
        stop = false;
        main_thread = std::thread(&Scheduler::schedule, this);
        running = true;
    }
}

void Scheduler::runEvery(TaskInterface *task,
        std::chrono::system_clock::duration interval) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.is_recurring = true;
    stask.interval = interval;
    stask.exec_time = std::chrono::system_clock::now() + interval;
    wait_q.push(stask);
}

void Scheduler::runAt(TaskInterface *task,
        std::chrono::system_clock::time_point time) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.exec_time = time;
    wait_q.push(stask);
}

// Schedules a task on a new thread of its own when it is time to
// execute the task. We use packaged_task and future to track if
// the task is completed. If it is, we call thread_join() to clean
// up the thread resources. The pacakged_task also takes care of
// rescheduling the recurring tasks *AFTER* they complete the current
// round of execution.
void Scheduler::schedule() {
    while(!stop) {
        if (!wait_q.empty()) {
            struct SchedulerTask task = wait_q.top();
            if (task.exec_time >= std::chrono::system_clock::now()) {
                std::packaged_task<void()> ptask([this, task]() {
                        // this exception handling makes sure that a single
                        // bad module does not bring down the entire scheduler.
                        // The task which throws an exception is not scheduled
                        // again.
                        try {
                            auto data = task.task->run();
                            task.task->persist(data);
                            if (!this->stop && task.is_recurring) {
                                this->runEvery(task.task, task.interval);
                            }
                        } catch (const char *e) {
                            schedlogger->critical("exception: " + std::string(e));
                        } catch (...) {
                            schedlogger->critical("default unhandled exception");
                        }
                        return;
                    });
                std::future<void> future = ptask.get_future();

                active_tasks.push_back(
                        make_pair(std::move(std::thread(std::move(ptask))),
                            std::move(future)));
                wait_q.pop();
                continue;
            }
            break;
        }

        // cleanly exit the completed tasks
        // TODO: maybe this needs to be a new function which can be
        // used to wait till all tasks are complete
        auto it = active_tasks.begin();
        while(it != active_tasks.end()) {
            if(it->second.wait_for(std::chrono::milliseconds(0)) ==
                    std::future_status::ready) {
                it->first.join();
                it = active_tasks.erase(it);
            } else {
                it++;
            }
        }
        
        // scheduler tick. The tick is 1s so we cannot execute tasks
        // at frequency higher than 1HZ
        // TODO: make sure that the runEvery() function asserts this
        // invariant.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}

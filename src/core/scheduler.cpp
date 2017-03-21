#include <scheduler.h>
#include <iostream>

using namespace kronos;

Scheduler::~Scheduler() {
    main_thread.join();
}

void Scheduler::start() {
    if (!running) {
        stop = false;
        main_thread = std::thread(&Scheduler::schedule, this);
        running = true;
    }
}

void Scheduler::runEvery(TaskInterface *task, std::chrono::system_clock::duration interval) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.is_recurring = true;
    stask.interval = interval;
    stask.exec_time = std::chrono::system_clock::now() + interval;
    wait_q.push(stask);
}

void Scheduler::runAt(TaskInterface *task, std::chrono::system_clock::time_point time) {
    struct SchedulerTask stask;
    stask.task = task;
    stask.exec_time = time;
    wait_q.push(stask);
}

void Scheduler::schedule() {
    while(!stop) {
        if (!wait_q.empty()) {
            struct SchedulerTask task = wait_q.top();
            if (task.exec_time >= std::chrono::system_clock::now()) {
                std::packaged_task<void()> ptask([this, task]() {
                        task.task->run();
                        if (task.is_recurring) {
                            this->runEvery(task.task, task.interval);
                        }
                        return;
                    });
                std::future<void> future = ptask.get_future();

                active_tasks.push_back(make_pair(std::move(std::thread(std::move(ptask))), std::move(future)));
                wait_q.pop();
                continue;
            }
            break;
        }

        // cleanly exit the completed tasks
        // TODO: maybe this needs to be a new function which can be used to wait till all tasks
        // are complete
        auto it = active_tasks.begin();
        while(it != active_tasks.end()) {
            if(it->second.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                it->first.join();
                it = active_tasks.erase(it);
            } else {
                it++;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}
#include <scheduler.h>
#include <iostream>

using namespace kron;

Scheduler::~Scheduler() {
    main_thread.join();
}

void Scheduler::start() {
    if (!running)
         main_thread = std::thread(&Scheduler::schedule, this);
    running = true;
}

void Scheduler::runEvery(TaskInterface &task, std::chrono::system_clock::duration interval) {
    return;
}

void Scheduler::runAt(TaskInterface &task, std::chrono::system_clock::time_point time) {
    return;
}

void Scheduler::schedule() {
    std::cout << "yolo" << std::endl; 
    return;
}

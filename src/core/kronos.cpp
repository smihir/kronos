#include <scheduler.h>
#include <iostream>

using namespace kronos;

int main(int argc, char **argv) {
    Scheduler sched;
    module::ModuleFactory &factory = module::ModuleFactory::Get();
    module::ModuleInterface *module = factory.GetModule("PrintTask");
    if (module == nullptr)
        std::cout << "module is null" << std::endl;
    //sched.runAt(&pt, std::chrono::system_clock::now() + std::chrono::seconds(1));
    sched.runEvery(module, std::chrono::seconds(2));

    sched.start();
    return 0;
}

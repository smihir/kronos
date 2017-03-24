#include <scheduler.h>
#include <vector>
#include <iostream>

using namespace kronos;

int main(int argc, char **argv) {
    Scheduler sched;
    module::ModuleFactory &factory = module::ModuleFactory::Get();
    std::vector<std::string> moduleNames = factory.GetModuleNames();

    for (auto &name : moduleNames) {
        module::ModuleInterface *module = factory.GetModule(name);
        unsigned int interval = factory.GetModuleInterval(name);

        sched.runEvery(module, std::chrono::seconds(interval));
    }
    sched.start();
    return 0;
}

#include <scheduler.h>

using namespace kronos;

int main(int argc, char **argv) {
    Scheduler sched;
    module::ModuleFactory &factory = module::ModuleFactory::Get();
    module::ModuleInterface *module = factory.GetModule("PrintTask");

    sched.runEvery(module, std::chrono::seconds(2));

    sched.start();
    return 0;
}

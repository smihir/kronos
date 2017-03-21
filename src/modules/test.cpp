#include <module.h>
#include <spdlog/spdlog.h>

using namespace kronos::module;

auto logger = spdlog::stdout_color_mt("printtask");

class PrintTask : public ModuleInterface {
    void run() {
        logger->info("hello world");
    }
    
    ~PrintTask(){}
};

REGISTER(PrintTask);

#include <module.h>
#include <spdlog/spdlog.h>

using namespace kronos::module;

auto logger = spdlog::stdout_color_mt("printtask");

class PrintTask : public PersistModule {
    public:
        PrintTask() : PersistModule("printtask") {};
        float run() {
            logger->info("hello world");
            return 0;
        }
        
        ~PrintTask(){}
};

REGISTER(PrintTask,10);

/*
 * Logs the memory of this application to the datastore
 */
#include <fstream>
#include <module.h>
#include <spdlog/spdlog.h>
#include <sstream>

using namespace kronos::module;

auto memlogger = spdlog::stdout_color_mt("memtask");

class MemoryTask : public PersistModule {
    public:
        MemoryTask() : PersistModule("memtask") {};
        float run() {
            std::ifstream statf("/proc/self/statm");
            if (statf.is_open()) {
                std::string line;
                std::getline(statf, line);
                std::istringstream memline(line);
                int mem;

                memline >> mem;;
                memlogger->info("memory: {}", mem);
                statf.close();
                return mem;
            }
            memlogger->error("unable to open file");
            return -1;
        }
        
        ~MemoryTask(){}
};

REGISTER(MemoryTask,5);

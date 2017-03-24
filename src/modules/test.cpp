/*
 * Reference Module
 */
#include <module.h>
#include <spdlog/spdlog.h>

using namespace kronos::module;

auto logger = spdlog::stdout_color_mt("printtask");

// extends PersistModule so that the data is persisted to a datastore
// currently sqlite is being used and we will create a database called
// datastore.db in current working directory
class PrintTask : public PersistModule {
    public:
        // constructor will contain the name of the table in which
        // the tdata for this task is stored;
        PrintTask() : PersistModule("printtask") {};
        // run methoed is executed by the scheduler every fixed
        // interval, the return value is logged to the datastore
        // along with a timestamp
        float run() {
            logger->info("hello world");
            return 0;
        }
        
        ~PrintTask(){}
};

// register to run this module at every 10 seconds;
REGISTER(PrintTask,10);

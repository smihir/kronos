#include <module.h>
#include <iostream>

using namespace kronos::module;

class PrintTask : public ModuleInterface {
    void run() {
        std::cout << "hello world" << std::endl;
    }
    
    ~PrintTask(){}
};

REGISTER(PrintTask);

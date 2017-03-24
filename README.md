# Kronos
Kronos is a simple periodic task scheduler. Each task is run in a separate thread. Task is written as a C++ class in a Kronos module, example of a simple module is [here](src/modules/test.cpp). The scheduler has been tested on Ubuntu 14.04. The scheduler can be compiled and run on OSX, but some modules will not work on OSX.

## Compiling
```
cmake . && make
```
The binary will be generated in src/core directory and it will be called kronos.

## Running
From root of the tree run 
```
./src/core/kronos
```
The scheduler will start running the default modules:
- **test.cpp:** just prints "hello world" every 10s
- **memory.cpp:** prints and stores VM size of the process in database every 5s
- **tcp_connect.cpp:** stores time to estabish tcp connection to google.com every 5s

To undrestand where the data is stored please go through the code for the [test.cpp](src/modules/test.cpp) module.

## Writing a new module
Writing a new module is simple. You just need to add a new .cpp file in the modules directory and extend the **ModuleInterface** or
the **PersistModule** class and provide a *run()* method. In Case of PersistModule you also need to specify the name of the database table
where you want to persist the data. Please refer [test.cpp](src/modules/test.cpp) for further details and comments.

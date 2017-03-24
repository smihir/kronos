/*
 * Module package enables us to add new modules to the scheduler easily.
 * This uses the abstract factory methods to instantiate the registered
 * classes. The work is inspired partly by facebook's osquery plugin mechanism,
 * but is much simpler. I encountered a lot of problems to make this work.
 * But thanks to google and SO user Cilyan(who had already done a plugin
 * implementation before), I was able to work around my problems. I am still
 * not satisfied with the implementation as evident by the number of FIXMEs
 * and GOTCHAs. But it works! 
 */
#include <db.h>
#include <map>
#include <string>

#pragma once
namespace kronos {
namespace module {
// Base module class, the scheduler's task is
// inherited from this abstract class.
class ModuleInterface {
    public:
        virtual float run() = 0;
        virtual ~ModuleInterface() {};
        virtual void persist(float &data) {};
};

class PersistModule : public ModuleInterface {
    public:
        PersistModule(std::string name) : store(name) {
            kronos::db::DatastoreFactory factory("sqlite3");
            db = factory.getDatastore();
            if (!db->connect()) {
                throw "failed to connect to the database";
            }
            if (!db->do_query("CREATE TABLE IF NOT EXISTS " + store +
                    " (val REAL, "
                    " Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);")) {
                throw "failed to create table";
            }
        }
        ~PersistModule() {
            db->disconnect();
            delete db;
        }
        void persist(float& data);
    private:
        std::string store;
        kronos::db::DatastoreInterface *db;
};


// This interface is required to resolve cyclic dependencies
// between ModuleFactory and ModuleRegistrar
class ModuleRegistrarInterface {
    public:
        virtual ModuleInterface* GetModule() = 0;
};

class ModuleFactory {
    public:
        // Get Singleton instance
        static ModuleFactory& Get();
        // FIXME: dont use pointers, figure out a way to fix the 2 funcs
        void Register(ModuleRegistrarInterface *registrar, std::string name);
        ModuleInterface* GetModule(std::string name);

        ModuleFactory(ModuleFactory const&) = delete;
        void operator=(ModuleFactory const&) = delete;

    private:
        ModuleFactory() {}
        // FIXME: no pointers
        std::map<std::string, ModuleRegistrarInterface*> registry;
};

template<class T>
class ModuleRegistrar : public ModuleRegistrarInterface {
    public:
        ModuleRegistrar(std::string classname);
        // FIXME: no pointers
        ModuleInterface* GetModule();
};

// TODO: check why the compilation fails with symbol not found
// when these 2 functions are moved to module.cpp. There errors
// (and time) are preventing me from adding support for dynamically
// loaded modules(much fun!). BTW, because of these errors all the
// object files are linked together into the executable instead
// of creating a static archive for all the modules and linking
// them to the executable.
template<class T>
ModuleRegistrar<T>::ModuleRegistrar(std::string classname) {
    ModuleFactory &factory = ModuleFactory::Get();
    factory.Register(this, classname);
}

template<class T>
ModuleInterface* ModuleRegistrar<T>::GetModule() {
    return new T();
}

} /* namespace module */
} /* namespace kronos */

#define REGISTER(CLASSNAME) \
    namespace { \
        static kronos::module::ModuleRegistrar<CLASSNAME> \
        CLASSNAME##_registrar(#CLASSNAME); \
    };

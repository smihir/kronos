#include <list>
#include <map>
#include <string>

#pragma once
namespace kronos {
namespace module {
/* Base class for modules */
class ModuleInterface {
    public:
        virtual void run() = 0;
        virtual ~ModuleInterface() {};
};

class ModuleRegistrarInterface {
    public:
        virtual ModuleInterface* GetModule() = 0;
};

class ModuleFactory {
    public:
        /* Get Singleton instance */
        static ModuleFactory& Get();
        void Register(ModuleRegistrarInterface *registrar, std::string name);
        ModuleInterface* GetModule(std::string name);

        ModuleFactory(ModuleFactory const&) = delete;
        void operator=(ModuleFactory const&) = delete;

    private:
        ModuleFactory() {}
        std::map<std::string, ModuleRegistrarInterface*> registry;
};

template<class TModule>
class ModuleRegistrar : public ModuleRegistrarInterface {
    public:
        ModuleRegistrar(std::string classname);
        ModuleInterface* GetModule();
};

template<class TModule>
ModuleRegistrar<TModule>::ModuleRegistrar(std::string classname) {
    ModuleFactory &factory = ModuleFactory::Get();
    factory.Register(this, classname);
}

template<class TModule>
ModuleInterface* ModuleRegistrar<TModule>::GetModule() {
    return new TModule();
}

} /* namespace module */
} /* namespace kronos */

#define REGISTER(CLASSNAME) \
    namespace { \
        static kronos::module::ModuleRegistrar<CLASSNAME> \
        CLASSNAME##_registrar( #CLASSNAME ); \
    };

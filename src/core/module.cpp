#include <module.h>

using namespace kronos::module;

ModuleFactory& ModuleFactory::Get() {
    static ModuleFactory factory;
    return factory;
}

void ModuleFactory::Register(ModuleRegistrarInterface *registrar, std::string name) {
    registry[name] = registrar;
}

ModuleInterface* ModuleFactory::GetModule(std::string name) {
    ModuleRegistrarInterface* registrar;
    auto entry = registry.find(name);
    if (entry == registry.end()) {
        throw std::out_of_range("Module Not Foud");
    }
    return entry->second->GetModule();
}

#include <module.h>
#include <chrono>
#include <ctime>

using namespace kronos::module;

ModuleFactory& ModuleFactory::Get() {
    static ModuleFactory factory;
    return factory;
}

void ModuleFactory::Register(ModuleRegistrarInterface *registrar,
        std::string name, unsigned int sec) {
    registry[name] = std::make_pair(registrar, sec);
}

ModuleInterface* ModuleFactory::GetModule(std::string name) {
    ModuleRegistrarInterface* registrar;
    auto entry = registry.find(name);
    if (entry == registry.end()) {
        throw std::out_of_range("Module Not Foud");
    }
    return entry->second.first->GetModule();
}

unsigned int ModuleFactory::GetModuleInterval(std::string name) {
    ModuleRegistrarInterface* registrar;
    auto entry = registry.find(name);
    if (entry == registry.end()) {
        throw std::out_of_range("Module Not Foud");
    }
    return entry->second.second;
}

std::vector<std::string> ModuleFactory::GetModuleNames() {
    std::vector<std::string> vs;
    for (auto &mod : registry) {
        vs.push_back(mod.first);
    }
    return vs;
}

void PersistModule::persist(float& data) {
    if (!db->do_query("INSERT INTO " + store + " VALUES (" +
                std::to_string(data) + "," + "datetime('now')" + ")")) {
        throw "failed to insert in store";
    }
}

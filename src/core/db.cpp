#include <db.h>

using namespace kronos::db;

bool Sqlite3Datastore::connect() {
    int rc = sqlite3_open(dbname.c_str(), &db);
    if (rc) {
        // not sure why this close is required on failure
        // but it is mentioned in the sqlite3 documentation.
        sqlite3_close(db);
        return false;
    }
    return true;
}

bool Sqlite3Datastore::disconnect() {
    sqlite3_close(db);
    return true;
}

bool Sqlite3Datastore::do_query(std::string query) {
    char *err;
    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, &err);
    if (rc != SQLITE_OK) {
        sqlite3_free(err);
        return false;
    }
    return true;
}

DatastoreInterface* DatastoreFactory::getDatastore() {
    if (_backend == "sqlite3") {
        return new Sqlite3Datastore("datastore.db");
    } else {
        throw "db driver not implemented";
    }
    return nullptr;
}

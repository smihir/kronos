#include <sqlite3.h>
#include <string>

#pragma once

namespace kronos {
namespace db {
class DatastoreInterface {
    public:
        virtual bool connect() {return false;}
        virtual bool disconnect() {return false;}
        virtual bool do_query(std::string query) {return false;}
        virtual bool begin_txn() {return false;}
        virtual bool commit_txn() {return false;}
        virtual bool rollback_txn() {return false;}
        virtual ~DatastoreInterface() {};
};

class Sqlite3Datastore : public DatastoreInterface {
    public:
        Sqlite3Datastore() = delete;
        Sqlite3Datastore(std::string name) : dbname(name) {}

        // move assignment
        Sqlite3Datastore& operator=(Sqlite3Datastore&& that) {
            if (this != &that) {
                dbname = std::move(that.dbname);
            }
            return *this;
        }

        // move constructor
        Sqlite3Datastore(Sqlite3Datastore&& that) {
            dbname = std::move(that.dbname);
        }

        // copy assignment
        Sqlite3Datastore& operator=(Sqlite3Datastore& that) {
            if (this != &that) {
                dbname = that.dbname;
            }
            return *this;
        }

        // copy constructor
        Sqlite3Datastore(Sqlite3Datastore& that) : dbname(that.dbname) {}

        bool connect();
        bool disconnect();
        bool do_query(std::string query);
    private:
        std::string dbname;
        sqlite3 *db;
};

class DatastoreFactory {
    public:
        DatastoreFactory() = delete;
        DatastoreFactory(std::string backend) : _backend(backend) {}
        // need to make sure that Datastore classes have the move
        // constructor and move assignment constructor
        DatastoreInterface* getDatastore();
    private:
        std::string _backend;
};

} /* namespace db */
} /* namespace kronos */

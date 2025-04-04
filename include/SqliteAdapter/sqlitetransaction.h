#pragma once

#include <DatabaseAdapter/databaseadapter.h>
#include <external/sqlite-amalgamation/sqlite3.h>

namespace DatabaseAdapter {

class SqliteTransaction final : public ITransaction
{
public:
    explicit SqliteTransaction(sqlite3* database);

    Models::QueryResult exec(const std::string& query) override;
    bool commit() override;
    bool rollback(const std::string& save_point) override;
    bool add_save_point(const std::string& save_point) override;

private:
    sqlite3* _database;
};

} // namespace DatabaseAdapter
#pragma once

#include <DatabaseAdapter/databaseadapter.h>
#include <external/sqlite-amalgamation/sqlite3.h>

namespace database_adapter {

class sqlite_transaction final : public ITransaction
{
public:
    explicit sqlite_transaction(sqlite3* database);

    models::query_result exec(const std::string& query) override;
    bool commit() override;
    bool rollback(const std::string& save_point) override;
    bool add_save_point(const std::string& save_point) override;

private:
    sqlite3* _database;
};

} // namespace database_adapter
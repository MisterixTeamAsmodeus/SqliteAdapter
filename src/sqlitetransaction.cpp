#include "SqliteAdapter/sqlitetransaction.h"

#include "SqliteAdapter/helpers/sqliteexechelper.h"

// https://www.sqlite.org/cintro.html
// https://www.book2s.com/tutorials/sqlite-cpp.html

namespace database_adapter {

sqlite_transaction::sqlite_transaction(sqlite3* database)
    : _database(database)
{
}

models::query_result sqlite_transaction::exec(const std::string& query)
{
    return helpers::exec_sqlite_script(_database, query);
}

bool sqlite_transaction::commit()
{
    return sqlite3_exec(_database, "COMMIT;", nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool sqlite_transaction::rollback(const std::string& save_point)
{
    const auto sql = save_point.empty() ? "ROLLBACK;" : "ROLLBACK TO " + save_point;
    return sqlite3_exec(_database, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool sqlite_transaction::add_save_point(const std::string& save_point)
{
    if(save_point.empty())
        return false;

    const auto sql = "SAVEPOINT " + save_point;
    return sqlite3_exec(_database, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK;
}

} // namespace database_adapter
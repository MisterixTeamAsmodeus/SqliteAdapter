#include "SqliteAdapter/sqlitetransaction.h"

#include "SqliteAdapter/helpers/sqliteexechelper.h"

// https://www.sqlite.org/cintro.html
// https://www.book2s.com/tutorials/sqlite-cpp.html

namespace DatabaseAdapter {

SqliteTransaction::SqliteTransaction(sqlite3* database)
    : _database(database)
{
}

Models::QueryResult SqliteTransaction::exec(const std::string& query)
{
    return Helpers::exec_sqlite_script(_database, query);
}

bool SqliteTransaction::commit()
{
    return sqlite3_exec(_database, "COMMIT;", nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool SqliteTransaction::rollback(const std::string& save_point)
{
    const auto sql = save_point.empty() ? "ROLLBACK;" : "ROLLBACK TO " + save_point;
    return sqlite3_exec(_database, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool SqliteTransaction::add_save_point(const std::string& save_point)
{
    if(save_point.empty())
        return false;

    const auto sql = "SAVEPOINT " + save_point;
    return sqlite3_exec(_database, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK;
}

} // namespace DatabaseAdapter
#include "SqliteAdapter/sqlitedatabaseadapter.h"

#include "SqliteAdapter/helpers/sqliteexechelper.h"
#include "SqliteAdapter/sqlitetransaction.h"
#include "SqliteAdapter/transactiontype.h"

#include <DatabaseAdapter/opendatabaseexception.h>

// https://www.sqlite.org/cintro.html
// https://www.book2s.com/tutorials/sqlite-cpp.html

namespace DatabaseAdapter {

SqliteDatabaseAdapter::SqliteDatabaseAdapter(const Models::DatabaseSettings& settings)
    : IDataBaseDriver(settings)
{
}

SqliteDatabaseAdapter::~SqliteDatabaseAdapter()
{
    sqlite3_close(_database);
}

bool SqliteDatabaseAdapter::connect()
{
    if(sqlite3_open(_settings.url.c_str(), &_database) != SQLITE_OK) {
        std::string _last_error = "Can't open database: ";
        _last_error.append(sqlite3_errmsg(_database));
        throw open_database_exception(std::move(_last_error));
    }

    return true;
}

bool SqliteDatabaseAdapter::is_open()
{
    return _database != nullptr;
}

bool SqliteDatabaseAdapter::disconnect() const
{
    return sqlite3_close(_database) == SQLITE_OK;
}

Models::QueryResult SqliteDatabaseAdapter::exec(const std::string& query)
{
    return Helpers::exec_sqlite_script(_database, query);
}

std::shared_ptr<ITransaction> SqliteDatabaseAdapter::open_transaction(int type) const
{
    const auto sql = [&type]() {
        switch(type) {
            case static_cast<int>(TransactionType::DEFERRED):
                return "BEGIN DEFERRED;";
            case static_cast<int>(TransactionType::IMMEDIATE):
                return "BEGIN IMMEDIATE;";
            case static_cast<int>(TransactionType::EXCLUSIVE):
                return "BEGIN EXCLUSIVE;";
            default:
                return "BEGIN;";
        }
    }();

    if(sqlite3_exec(_database, sql, nullptr, nullptr, nullptr) != SQLITE_OK)
        return nullptr;

    return std::make_shared<SqliteTransaction>(_database);
}

} // namespace DatabaseAdapter
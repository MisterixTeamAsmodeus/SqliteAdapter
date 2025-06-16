#include "SqliteAdapter/sqlitedatabaseadapter.h"

#include "SqliteAdapter/helpers/sqliteexechelper.h"
#include "SqliteAdapter/sqlitetransaction.h"
#include "SqliteAdapter/transactiontype.h"

#include <DatabaseAdapter/exception/opendatabaseexception.h>

// https://www.sqlite.org/cintro.html
// https://www.book2s.com/tutorials/sqlite-cpp.html

namespace database_adapter {

sqlite_database_adapter::sqlite_database_adapter(const models::database_settings& settings)
    : IDataBaseDriver(settings)
{
}

sqlite_database_adapter::~sqlite_database_adapter()
{
    if(_database == nullptr)
        return;

    sqlite3_close(_database);
}

std::shared_ptr<IDataBaseDriver> sqlite_database_adapter::inject()
{
    return std::make_shared<sqlite_database_adapter>(_settings);
}

bool sqlite_database_adapter::connect()
{
    if(sqlite3_open_v2(_settings.url.c_str(), &_database, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
        std::string _last_error = "Can't open database: ";
        _last_error.append(sqlite3_errmsg(_database));
        throw open_database_exception(std::move(_last_error));
    }

    return true;
}

bool sqlite_database_adapter::is_open()
{
    return _database != nullptr;
}

bool sqlite_database_adapter::disconnect() const
{
    if(_database == nullptr)
        return true;

    if(sqlite3_close(_database) != SQLITE_OK) {
        std::string _last_error = "Can't open database: ";
        _last_error.append(sqlite3_errmsg(_database));
        throw open_database_exception(std::move(_last_error));
    }

    return true;
}

models::query_result sqlite_database_adapter::exec(const std::string& query)
{
    return helpers::exec_sqlite_script(_database, query);
}

std::shared_ptr<ITransaction> sqlite_database_adapter::open_transaction(int type) const
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

    return std::make_shared<sqlite_transaction>(_database);
}

} // namespace database_adapter
#include "SqliteAdapter/sqlitedatabaseadapter.h"

#include "SqliteAdapter/sqliteconnection.h"
#include "SqliteAdapter/sqlitetransaction.h"
#include "SqliteAdapter/transactiontype.h"

namespace database_adapter {

sqlite_database_adapter::sqlite_database_adapter(const models::database_settings& settings)
    : IDataBaseDriver(std::make_shared<sqlite_connection>(settings))
{
}

sqlite_database_adapter::sqlite_database_adapter(const std::shared_ptr<sqlite_connection>& connection)
    : IDataBaseDriver(connection)
{
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

    try {
        _connection->exec(sql);
    } catch(...) {
        return nullptr;
    }

    return std::make_shared<sqlite_transaction>(_connection);
}

} // namespace database_adapter
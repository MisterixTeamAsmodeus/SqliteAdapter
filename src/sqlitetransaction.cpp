#include "SqliteAdapter/sqlitetransaction.h"

#include "SqliteAdapter/sqliteconnection.h"

namespace database_adapter {

sqlite_transaction::sqlite_transaction(std::shared_ptr<sqlite_connection> connection)
    : ITransaction(std::move(connection))
{
}

sqlite_transaction::sqlite_transaction(std::shared_ptr<IConnection> connection)
    : ITransaction(std::move(connection))
{
}

bool sqlite_transaction::commit()
{
    try {
        _connection->exec("COMMIT;");
        return true;
    } catch(...) {
        return false;
    }
}

bool sqlite_transaction::rollback_to_save_point(const std::string& save_point)
{
    const auto sql = save_point.empty() ? "ROLLBACK;" : "ROLLBACK TO " + save_point;
    try {
        _connection->exec(sql);
        return true;
    } catch(...) {
        return false;
    }
}

bool sqlite_transaction::add_save_point(const std::string& save_point)
{
    if(save_point.empty())
        return false;

    const auto sql = "SAVEPOINT " + save_point;
    try {
        _connection->exec(sql);
        return true;
    } catch(...) {
        return false;
    }
}

} // namespace database_adapter
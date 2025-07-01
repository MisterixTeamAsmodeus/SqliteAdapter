#include "SqliteAdapter/sqlitetransaction.h"

#include "DatabaseAdapter/exception/sqlexception.h"
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

models::query_result sqlite_transaction::exec(const std::string& query)
{
    try {
        return ITransaction::exec(query);
    } catch(const sql_exception&) {
        _has_error = true;
        throw;
    }
}

bool sqlite_transaction::commit()
{
    if(_has_error) {
        rollback();
        return false;
    }
    _connection->exec("COMMIT;");
    return true;
}

void sqlite_transaction::rollback_to_save_point(const std::string& save_point)
{
    const auto sql = save_point.empty() ? "ROLLBACK;" : "ROLLBACK TO " + save_point;
    _connection->exec(sql);
}

void sqlite_transaction::add_save_point(const std::string& save_point)
{
    if(save_point.empty())
        return;

    const auto sql = "SAVEPOINT " + save_point;
    _connection->exec(sql);
}

} // namespace database_adapter
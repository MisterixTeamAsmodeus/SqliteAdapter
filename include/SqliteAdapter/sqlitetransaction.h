#pragma once

#include <DatabaseAdapter/itransaction.h>

namespace database_adapter {
class sqlite_connection;
} // namespace database_adapter

namespace database_adapter {

class sqlite_transaction final : public ITransaction
{
public:
    explicit sqlite_transaction(std::shared_ptr<sqlite_connection> connection);
    explicit sqlite_transaction(std::shared_ptr<IConnection> connection);

    bool commit() override;
    bool add_save_point(const std::string& save_point) override;
    bool rollback_to_save_point(const std::string& save_point) override;
};

} // namespace database_adapter
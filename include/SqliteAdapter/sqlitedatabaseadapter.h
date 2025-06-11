#pragma once

#include <DatabaseAdapter/databaseadapter.h>
#include <external/sqlite-amalgamation/sqlite3.h>

namespace database_adapter {

/**
 * Define для настроек подключения к Sqlite
 * @param url Путь до файла базы данных
 */
using sqlite_settings = models::database_settings;

class sqlite_database_adapter final : public IDataBaseDriver
{
public:
    explicit sqlite_database_adapter(const models::database_settings& settings);

    ~sqlite_database_adapter() override;

    std::shared_ptr<IDataBaseDriver> inject() override;

    bool connect() override;
    bool is_open() override;

    bool disconnect() const override;

    models::query_result exec(const std::string& query) override;
    std::shared_ptr<ITransaction> open_transaction(int type) const override;

private:
    sqlite3* _database = nullptr;
};

}; // namespace database_adapter

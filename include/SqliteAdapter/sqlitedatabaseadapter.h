#pragma once

#include <DatabaseAdapter/databaseadapter.h>
#include <external/sqlite-amalgamation/sqlite3.h>

namespace DatabaseAdapter {

/**
 * Define для настроек подключения к Sqlite
 * @param url Путь до файла базы данных
 */
using SqliteSettings = Models::DatabaseSettings;

class SqliteDatabaseAdapter final : public IDataBaseDriver
{
public:
    explicit SqliteDatabaseAdapter(const Models::DatabaseSettings& settings);

    ~SqliteDatabaseAdapter() override;

    bool connect() override;
    bool is_open() override;

    bool disconnect() const override;

    Models::QueryResult exec(const std::string& query) override;
    std::shared_ptr<ITransaction> open_transaction(int type) const override;

private:
    sqlite3* _database = nullptr;
};

}; // namespace DatabaseAdapter

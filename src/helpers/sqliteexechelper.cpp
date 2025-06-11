#include "SqliteAdapter/helpers/sqliteexechelper.h"

#include <DatabaseAdapter/exception/sqlexception.h>
#include <DatabaseAdapter/idatabasedriver.h>

#include <iostream>

namespace database_adapter {
namespace helpers {

models::query_result exec_sqlite_script(sqlite3* database, const std::string& query)
{
    using namespace database_adapter;
    sqlite3_stmt* stmt;

    std::cout << query << "\n"
              << "\n";

    if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::string _last_error = "Failed to prepare statement: ";
        _last_error.append(sqlite3_errmsg(database));
        throw sql_exception(std::move(_last_error), query);
    }

    int rc = sqlite3_step(stmt);

    models::query_result result;
    while(rc == SQLITE_ROW) {
        models::query_result::result_row row;
        for(int i = 0; i < sqlite3_column_count(stmt); i++) {
            const auto column_name = sqlite3_column_name(stmt, i);
            const auto column_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));

            row.emplace(column_name, column_value == nullptr ? IDataBaseDriver::null_value() : column_value);
        }
        result.add_row(row);

        rc = sqlite3_step(stmt);
    }

    if(rc != SQLITE_DONE) {
        std::string _last_error = "Failed to execute statement: ";
        _last_error.append(sqlite3_errmsg(database));
        throw sql_exception(std::move(_last_error), query);
    }

    sqlite3_finalize(stmt);

    return result;
}
} // namespace helpers
} // namespace database_adapter
#include <DatabaseAdapter/idatabasedriver.h>
#include <DatabaseAdapter/sqlexception.h>
#include <SqliteAdapter/helpers/sqliteexechelper.h>

namespace DatabaseAdapter {
namespace Helpers {

Models::QueryResult exec_sqlite_script(sqlite3* database, const std::string& query)
{
    using namespace DatabaseAdapter;
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::string _last_error = "Failed to prepare statement: ";
        _last_error.append(sqlite3_errmsg(database));
        throw sql_exception(_last_error.c_str(), query.c_str());
    }

    int rc = sqlite3_step(stmt);

    Models::QueryResult result;
    while(rc == SQLITE_ROW) {
        Models::QueryResult::ResultRow row;
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
        throw sql_exception(_last_error.c_str(), query.c_str());
    }

    sqlite3_finalize(stmt);

    return result;
}
} // namespace Helpers
} // namespace DatabaseAdapter
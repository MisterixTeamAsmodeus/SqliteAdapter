#pragma once

#include <DatabaseAdapter/model/queryresult.h>
#include <sqlite-amalgamation/sqlite3.h>

namespace database_adapter {
namespace helpers {
/**
 * Обёртка для выполнения запроса в sqlite3
 * @param database база где нужно выполнить запрос
 * @param query Запрос который необходимо выполнить
 * @return Результат выполнения запроса
 * @throws Выбрасывает sql_exception, если произошла ошибка выполнения запроса
 */
models::query_result exec_sqlite_script(sqlite3* database, const std::string& query);
} // namespace helpers
} // namespace database_adapter
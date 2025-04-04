#pragma once

#include <DatabaseAdapter/model/queryresult.h>
#include <sqlite-amalgamation/sqlite3.h>

namespace DatabaseAdapter {
namespace Helpers {
/**
 * Обёртка для выполнения запроса в sqlite3
 * @param database база где нужно выполнить запрос
 * @param query Запрос который необходимо выполнить
 * @return Результат выполнения запроса
 * @throws Выбрасывает sql_exception, если произошла ошибка выполнения запроса
 */
Models::QueryResult exec_sqlite_script(sqlite3* database, const std::string& query);
} // namespace Helpers
} // namespace DatabaseAdapter
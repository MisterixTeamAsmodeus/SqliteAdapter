#include "SqliteAdapter/sqlitedatabaseadapter.h"

#include <iostream>

int main()
{
    DatabaseAdapter::SqliteSettings settings;
    settings.url = R"(C:\Project\src\test.db)";

    DatabaseAdapter::SqliteDatabaseAdapter adapter(settings);
    if(adapter.connect()) {
        std::cout << "connect";
    }

    adapter.exec("CREATE TABLE IF NOT exists Persons ( PersonID int, LastName varchar(255), FirstName varchar(255), Address varchar(255), City varchar(255) );");

    auto transactional = adapter.open_base_transaction();

    transactional->exec("INSERT INTO Persons (PersonID, LastName, FirstName, Address, City) VALUES (1, 'Иванов', 'Иван', 'ул. Ленина, 10', 'Москва');");
}

#pragma once

#include <cstdint>

namespace DatabaseAdapter {
/// Типы транзакции поддерживаемые в sqlite3
enum class TransactionType : uint8_t
{
    DEFERRED = 0,
    IMMEDIATE,
    EXCLUSIVE
};
} // namespace DatabaseAdapter
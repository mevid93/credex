#ifndef DB_WRITER_H
#define DB_WRITER_H

#include "database.h"

/// @brief Write database into encrypted json file.
/// @param Database to be written.
void write_database(const std::unique_ptr<Database>& database);

#endif // DB_WRITER_H

#ifndef DB_READER_H
#define DB_READER_H

#include "database.h"

/// @brief Read database from encrypted json file.
std::unique_ptr<Database> read_database(std::string filePath, std::string& password);

#endif // DB_READER_H

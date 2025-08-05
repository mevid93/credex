#ifndef DATABASE_H
#define DATABASE_H

#include "record.h"
#include <memory>
#include <string>
#include <vector>

class Database
{
public:
    /// @brief Database constructor.
    /// @param filePath Database file path.
    explicit Database(std::string& filePath);

    /// @brief Database constructor.
    /// @param filePath Database file path.
    /// @param records Database records.
    Database(std::string& filePath, std::vector<Record>& records);

    /// @brief Database destructor.
    ~Database();

    const std::string& getFilePath() const;
private:
    std::string filePath;
    std::vector<std::shared_ptr<Record>> records;
};

#endif // DATABASE_H

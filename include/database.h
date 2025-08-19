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
    Database();

    /// @brief Database constructor.
    /// @param filePath Database file path.
    Database(std::string& filePath);

    /// @brief Database constructor.
    /// @param filePath Database file path.
    /// @param records Database records.
    Database(std::string& filePath, std::vector<Record>& records);

    /// @brief Database destructor.
    ~Database();

    /// @brief Insert new record to database.
    /// @param record New record to be inserted.
    void addNewRecord(std::shared_ptr<Record> record);

    /// @brief Deleted record by id.
    /// @param id Record id.
    void deleteRecord(uint32_t id);

    /// @brief Update record in database.
    /// @param record Updated record.
    void updateRecord(Record& record);

    /// @brief Set database password.
    /// @param password Database password.
    void setPassword(std::string& password);

    /// @brief Get Records.
    /// @returns Vector of shared ptr records.
    const std::vector<std::shared_ptr<Record>>& getRecords() const;

    /// @brief Set file path.
    /// @param newFilePath New file path.
    void setFilePath(const std::string &newFilePath);

    /// @brief Get database password.
    /// @returns Database password.
    std::string getPassword() const;

    /// @brief Get database file path.
    /// @returns File path.
    std::string getFilePath() const;

    /// @brief Get single record by id.
    /// @returns Record with given id.
    const std::shared_ptr<Record> getRecord(uint32_t id) const;

private:
    std::string m_password;
    std::string m_filePath;
    std::vector<std::shared_ptr<Record>> m_records;
};

#endif // DATABASE_H

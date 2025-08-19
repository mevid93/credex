#include "database.h"
#include <algorithm>

Database::Database() {}

Database::Database(std::string& filePath): m_filePath(filePath) {}

Database::~Database() {}

void Database::setPassword(std::string& password) {
    this->m_password = password;
}

void Database::addNewRecord(std::shared_ptr<Record> record) {
    this->m_records.push_back(record);
}

const std::vector<std::shared_ptr<Record>>& Database::getRecords() const  {
    return this->m_records;
}

void Database::setFilePath(const std::string &newFilePath)
{
    this->m_filePath = newFilePath;
}

std::string Database::getPassword() const
{
    return m_password;
}

std::string Database::getFilePath() const
{
    return m_filePath;
}

const std::shared_ptr<Record> Database::getRecord(uint32_t id) const {
    auto it = std::find_if(this->m_records.begin(), this->m_records.end(), [id](const std::shared_ptr<Record>& record) {
        return record->getId() == id;
    });

    if (it == m_records.end())
        return nullptr;

    return *it;
}

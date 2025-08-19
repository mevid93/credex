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

const std::shared_ptr<Record> Database::getRecord(const uint32_t id) const {
    auto it = std::find_if(this->m_records.begin(), this->m_records.end(), [id](const std::shared_ptr<Record>& record) {
        return record->getId() == id;
    });

    if (it == m_records.end())
        return nullptr;

    return *it;
}

void Database::updateRecord(std::shared_ptr<Record> record) {
    uint32_t id = record->getId();
    auto it = std::find_if(this->m_records.begin(), this->m_records.end(), [id](const std::shared_ptr<Record>& record) {
        return record->getId() == id;
    });

    if (it == m_records.end())
        return;

    *it = record;
}

void Database::deleteRecord(const uint32_t id) {
    for (auto it = this->m_records.begin(); it != this->m_records.end(); ) {
        if ((*it)->getId() == id) {
            it = this->m_records.erase(it); // erase returns the next valid iterator
        } else {
            ++it;
        }
    }
}

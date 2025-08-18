#include "database.h"

Database::Database() {};

Database::Database(std::string& filePath): m_filePath(filePath) {}

Database::~Database() {}

void Database::setPassword(std::string& password) {
    this->m_password = password;
}

void Database::addNewRecord(std::shared_ptr<Record> record) {
    this->m_records.push_back(record);
}

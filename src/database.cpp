#include "database.h"

Database::Database() {};

Database::Database(std::string& filePath): m_filePath(filePath) {}

Database::~Database() {}

void Database::setPassword(std::string& password) {
    this->m_password = password;
}

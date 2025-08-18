#include "record.h"

int Record::s_nextFreeId = 0;

Record::Record(uint32_t id): m_id(id) {}

Record::~Record() {}

uint32_t Record::getId() const {
    return this->m_id;
}

const std::string& Record::getDescription() const {
    return this->m_description;
}

const std::string& Record::getEmail() const {
    return this->m_email;
}

const std::string& Record::getTitle() const {
    return this->m_title;
}

const std::string& Record::getPassword() const {
    return this->m_password;
}

const std::string& Record::getUrl() const {
    return this->m_url;
}

const std::string& Record::getUsername() const {
    return this->m_username;
}

void Record::setDescription(std::string description) {
    this->m_description = description;
}

void Record::setEmail(std::string email) {
    this->m_email = email;
}

void Record::setTitle(std::string title) {
    this->m_title = title;
}

void Record::setPassword(std::string password) {
    this->m_password = password;
}

void Record::setUrl(std::string url) {
    this->m_url = url;
}

void Record::setUsername(std::string username) {
    this->m_username = username;
}

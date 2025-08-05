#include "record.h"

Record::Record(uint32_t id): id(id) {}

Record::~Record() {}

uint32_t Record::getId() const {
    return this->id;
}

const std::string& Record::getDescription() const {
    return this->description;
}

const std::string& Record::getEmail() const {
    return this->email;
}

const std::string& Record::getName() const {
    return this->name;
}

const std::string& Record::getPassword() const {
    return this->password;
}

const std::string& Record::getUrl() const {
    return this->url;
}

const std::string& Record::getUsername() const {
    return this->username;
}

void Record::setDescription(std::string& description) {
    this->description = description;
}

void Record::setEmail(std::string& email) {
    this->email = email;
}

void Record::setName(std::string& name) {
    this->name = name;
}

void Record::setPassword(std::string& password) {
    this->password = password;
}

void Record::setUrl(std::string& url) {
    this->url = url;
}

void Record::setUsername(std::string& username) {
    this->username = username;
}

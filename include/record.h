#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record
{
public:
    /// @brief Record constructor.
    /// @param id Record id.
    explicit Record(uint32_t id);

    /// @brief Record destructor.
    ~Record();

    /// @brief Get Record id.
    /// @return Record id.
    uint32_t getId() const;

    /// @brief Get Record name.
    /// @return Record name.
    const std::string& getName() const;

    /// @brief Get Record username.
    /// @return Record username.
    const std::string& getUsername() const;

    /// @brief Get Record password.
    /// @return Record password.
    const std::string& getPassword() const;

    /// @brief Get Record email.
    /// @return Record email.
    const std::string& getEmail() const;

    /// @brief Get Record url.
    /// @return Record url.
    const std::string& getUrl() const;

    /// @brief Get Record description.
    /// @return Record description.
    const std::string& getDescription() const;

    /// @brief Set name.
    /// @param name Record name.
    void setName(std::string& name);

    /// @brief Set username.
    /// @param username Record username.
    void setUsername(std::string& username);

    /// @brief Set password.
    /// @param password Record password.
    void setPassword(std::string& password);

    /// @brief Set email.
    /// @param email Record email.
    void setEmail(std::string& email);

    /// @brief Set url.
    /// @param url Record url.
    void setUrl(std::string& url);

    /// @brief Set description.
    /// @param description Record description.
    void setDescription(std::string& description);

private:
    uint32_t id;
    std::string name;
    std::string username;
    std::string password;
    std::string email;
    std::string url;
    std::string description;
};

#endif // RECORD_H

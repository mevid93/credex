#include "db_reader.h"
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>

std::string hashPasswordTo256(const std::string& password);
std::string decryptAES256CBC(const std::vector<unsigned char>& ciphertext,
                             const std::vector<unsigned char>& key,
                             const std::vector<unsigned char>& iv);

std::unique_ptr<Database> read_database(std::string filePath, std::string& password) {
    // 1. Read binary data from file.
    std::ifstream file(filePath, std::ios::binary);

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate buffer and read
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    if (buffer.size() < 17)
        return nullptr;

    std::vector<unsigned char> iv(buffer.begin(), buffer.begin() + 16);  // first 16 bytes
    std::vector<unsigned char> data(buffer.begin() + 16, buffer.end());  // rest of the data

    // 2. Decrypt the data.
    std::string decryptionPassword = "0123456789abcdef0123456789abcdef";

    if (password.length() != 0) {
        decryptionPassword = hashPasswordTo256(password);
    }

    std::vector<unsigned char> key(decryptionPassword.begin(), decryptionPassword.end());
    std::string plainText = decryptAES256CBC(data, key, iv);

    // Check that decryption was successful.
    if (plainText.find("records") == std::string::npos) {
        return nullptr;
    }

    // 3. Deserialize the decrypted data.
    QByteArray byteArray = QByteArray::fromStdString(plainText);
    QJsonDocument doc = QJsonDocument::fromJson(byteArray);

    // 4. Parse data into Database object.
    QJsonObject obj = doc.object();
    QJsonArray jsonRecords = obj["records"].toArray();

    std::unique_ptr<Database> db = std::make_unique<Database>();
    db->setFilePath(filePath);
    db->setPassword(password);
    Record::s_nextFreeId = 0;

    for (int i = 0; i < jsonRecords.size(); ++i) {
        const QJsonObject &rObj = jsonRecords[i].toObject();

        std::string rTitle = rObj["title"].toString().toStdString();
        std::string rUsername = rObj["username"].toString().toStdString();
        std::string rPassword = rObj["password"].toString().toStdString();
        std::string rEmail = rObj["email"].toString().toStdString();
        std::string rUrl = rObj["url"].toString().toStdString();
        std::string rDescription = rObj["description"].toString().toStdString();

        std::shared_ptr<Record> record = std::make_shared<Record>(++Record::s_nextFreeId);
        record->setTitle(rTitle);
        record->setUsername(rUsername);
        record->setPassword(rPassword);
        record->setEmail(rEmail);
        record->setUrl(rUrl);
        record->setDescription(rDescription);

        db->addNewRecord(record);
    }

    return db;
}

// AES decryption function.
std::string decryptAES256CBC(const std::vector<unsigned char>& ciphertext,
                             const std::vector<unsigned char>& key,
                             const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return "";

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len = 0;
    int plaintext_len = 0;

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.begin() + plaintext_len);
}

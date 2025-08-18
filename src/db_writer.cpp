#include "db_writer.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>

std::string encryptAES(const std::string& plaintext, const std::string& key, std::string& iv_out);
std::string hashPasswordTo256(const std::string& password);

void write_database(const std::unique_ptr<Database>& database) {
    // 1. Create json.
    QJsonObject dataObject;
    QJsonArray records;

    for (auto& record : database->getRecords()) {
        QJsonObject recordObject;
        recordObject["title"] = QString::fromStdString(record->getTitle());
        recordObject["username"] = QString::fromStdString(record->getUsername());
        recordObject["password"] = QString::fromStdString(record->getPassword());
        recordObject["email"] = QString::fromStdString(record->getEmail());
        recordObject["url"] = QString::fromStdString(record->getUrl());
        recordObject["description"] = QString::fromStdString(record->getDescription());

        records.append(recordObject);
    }

    dataObject["records"] = records;

    QJsonDocument doc(dataObject);

    // 2. Serialize to string.
    std::string jsonString = doc.toJson(QJsonDocument::Compact).toStdString();

    // 3. Encrypt (user password or default password).
    // Default password provides no security!
    std::string password = "0123456789abcdef0123456789abcdef";
    std::string userPassword = database->getPassword();
    std::string iv;

    if (userPassword.length() != 0) {
        password = hashPasswordTo256(userPassword);
    }

    std::string encrypted = encryptAES(jsonString, password, iv);

    // 4. Write encrypted data into file (*.crdx).
    std::ofstream file(database->getFilePath(), std::ios::binary);
    file.write(iv.data(), iv.size());
    file.write(encrypted.data(), encrypted.size());
    file.close();
}

// AES encryption function
std::string encryptAES(const std::string& plaintext, const std::string& key, std::string& iv_out) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string ciphertext;
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));
    iv_out = std::string(reinterpret_cast<char*>(iv), sizeof(iv));

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       iv);

    std::vector<unsigned char> out(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0;
    EVP_EncryptUpdate(ctx, out.data(), &out_len1,
                      reinterpret_cast<const unsigned char*>(plaintext.data()),
                      plaintext.size());

    int out_len2 = 0;
    EVP_EncryptFinal_ex(ctx, out.data() + out_len1, &out_len2);
    EVP_CIPHER_CTX_free(ctx);

    ciphertext.assign(reinterpret_cast<char*>(out.data()), out_len1 + out_len2);
    return ciphertext;
}

// Hash password to make it make it 32 bytes long.
std::string hashPasswordTo256(const std::string& password) {
    QString p = QString::fromStdString(password);
    QByteArray hash = QCryptographicHash::hash(p.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex()).toStdString(); // 64-character hex string = 32 bytes
}

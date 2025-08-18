#include "password_dialog.h"
#include "ui_password_dialog.h"
#include <QMessageBox>

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &PasswordDialog::okButtonPressed);
    connect(this->ui->showPushButton, &QPushButton::pressed, this, &PasswordDialog::showButton1Pressed);
    connect(this->ui->showPushButton, &QPushButton::released, this, &PasswordDialog::showButton1Released);
    connect(this->ui->showPushButton_2, &QPushButton::pressed, this, &PasswordDialog::showButton2Pressed);
    connect(this->ui->showPushButton_2, &QPushButton::released, this, &PasswordDialog::showButton2Released);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::okButtonPressed() {
    // Check that both user inputs are same (user did not make typo).
    auto password1 = this->ui->passwordLineEdit->text();
    auto password2 = this->ui->passwordConfirmLineEdit->text();

    if (password1 != password2) {
        QMessageBox::warning(this, "Validation Failed", "Please make sure that you typed the same password twice.");
        return;
    }

    this->m_password = password1.toStdString();

    accept();
}

const std::string& PasswordDialog::getPassword() const {
    return this->m_password;
}

void PasswordDialog::showButton1Pressed() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void PasswordDialog::showButton1Released() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void PasswordDialog::showButton2Pressed() {
    this->ui->passwordConfirmLineEdit->setEchoMode(QLineEdit::Normal);
}

void PasswordDialog::showButton2Released() {
    this->ui->passwordConfirmLineEdit->setEchoMode(QLineEdit::Password);
}

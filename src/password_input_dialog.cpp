#include "password_input_dialog.h"
#include "ui_password_input_dialog.h"
#include <QMessageBox>

PasswordInputDialog::PasswordInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordInputDialog)
{
    ui->setupUi(this);

    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &PasswordInputDialog::okButtonPressed);
    connect(this->ui->showPushButton, &QPushButton::pressed, this, &PasswordInputDialog::showButton1Pressed);
    connect(this->ui->showPushButton, &QPushButton::released, this, &PasswordInputDialog::showButton1Released);
}

PasswordInputDialog::~PasswordInputDialog()
{
    delete ui;
}

void PasswordInputDialog::okButtonPressed() {
    auto password1 = this->ui->passwordLineEdit->text();

    this->m_password = password1.toStdString();

    accept();
}

const std::string& PasswordInputDialog::getPassword() const {
    return this->m_password;
}

void PasswordInputDialog::showButton1Pressed() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void PasswordInputDialog::showButton1Released() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

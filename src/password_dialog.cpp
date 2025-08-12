#include "password_dialog.h"
#include "ui_password_dialog.h"
#include <QMessageBox>

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &PasswordDialog::okButtonPressed);
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

#include "record_dialog.h"
#include "ui_record_dialog.h"

RecordDialog::RecordDialog(QWidget *parent, std::string title)
    : QDialog(parent)
    , ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(title));

    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &RecordDialog::okButtonPressed);
    connect(this->ui->showPushButton, &QPushButton::pressed, this, &RecordDialog::showButtonPressed);
    connect(this->ui->showPushButton, &QPushButton::released, this, &RecordDialog::showButtonReleased);

    this->m_record = std::make_shared<Record>(++Record::s_nextFreeId);
}

RecordDialog::~RecordDialog()
{
    delete ui;
}

std::shared_ptr<Record> RecordDialog::getRecord() const {
    return this->m_record;
}

void RecordDialog::showButtonPressed() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void RecordDialog::showButtonReleased() {
    this->ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void RecordDialog::okButtonPressed() {
    this->m_record->setTitle(this->ui->titleLineEdit->text().toStdString());
    this->m_record->setUsername(this->ui->usernameLineEdit->text().toStdString());
    this->m_record->setPassword(this->ui->passwordLineEdit->text().toStdString());
    this->m_record->setEmail(this->ui->emailLineEdit->text().toStdString());
    this->m_record->setUrl(this->ui->urlLineEdit->text().toStdString());
    this->m_record->setDescription(this->ui->descriptionLineEdit->text().toStdString());

    accept();
}

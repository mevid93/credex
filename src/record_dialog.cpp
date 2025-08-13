#include "record_dialog.h"
#include "ui_record_dialog.h"

RecordDialog::RecordDialog(QWidget *parent, std::string title)
    : QDialog(parent)
    , ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(title));
}

RecordDialog::~RecordDialog()
{
    delete ui;
}

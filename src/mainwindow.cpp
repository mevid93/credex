#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "password_dialog.h"
#include "password_input_dialog.h"
#include "record_dialog.h"
#include "db_writer.h"
#include "db_reader.h"
#include "QFileDialog"
#include <QTime>
#include <QMessageBox>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->frame->setStyleSheet("QFrame#frame { border: 1px solid white; }");

    // Set table columns to fill whole table.
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::onRightClick);

    // Set flag values.
    this->unsavedChanges = false;
    this->newDatabaseFile = false;
    this->databaseFileOpen = false;

    // Set some custom signals and slots.
    connect(this->ui->action_Exit, &QAction::triggered, this, &MainWindow::closeApplication);
    connect(this->ui->action_New, &QAction::triggered, this, &MainWindow::newDatabase);
    connect(this->ui->action_Open, &QAction::triggered, this, &MainWindow::openExistingDatabase);
    connect(this->ui->action_Save, &QAction::triggered, this, &MainWindow::saveDatabaseChanges);
    connect(this->ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveNewDatabaseFile);
    connect(this->ui->action_Close, &QAction::triggered, this, &MainWindow::closeDatabase);

    connect(this->ui->actionCreate_New, &QAction::triggered, this, &MainWindow::createNewRecord);
    connect(this->ui->actionSet_DB_Password, &QAction::triggered, this, &MainWindow::setDatabasePassword);

    this->updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeApplication()
{
    if (!handleUnsavedChanges()) {
        return;
    }

    QCoreApplication* app = QApplication::instance();
    app->quit();
}

void MainWindow::newDatabase()
{
    if (!handleUnsavedChanges()){
        return;
    }

    this->unsavedChanges = true;
    this->newDatabaseFile = true;
    this->databaseFileOpen = true;
    Record::s_nextFreeId = 0;

    this->m_database = std::make_unique<Database>();

    this->populateTableWithRecords();

    this->writeLog("Opened a new database file.");

    this->ui->pathLineEdit->setText("*new database file*");

    this->updateUI();
}

void MainWindow::openExistingDatabase()
{
    if (!handleUnsavedChanges()) {
        return;
    }

    QFileDialog dialog;
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setNameFilter("CRDX Files (*.crdx)");
    dialog.setFileMode(QFileDialog::ExistingFile);

    QString fileName = "";
    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            fileName = files.at(0);
        }
    } else {
        return;
    }

    // Keep asking password until no error or user cancels the operation.
    while(true) {
        PasswordInputDialog pwInputDialog(this);

        if (pwInputDialog.exec() != QDialog::Accepted) {
            this->writeLog("Read operation failed!");
            return;
        }

        std::string password = pwInputDialog.getPassword();

        try {
            std::unique_ptr<Database> db = read_database(fileName.toStdString(), password);

            if (db != nullptr) {
                this->m_database = std::move(db);
                break;
            }
        } catch (...) {
            this->writeLog("Read operation failed!");
        }

        QMessageBox::information(nullptr, "Warning", "Could not decrypt the file! Password was most likely wrong.");
    }

    this->unsavedChanges = false;
    this->newDatabaseFile = false;
    this->databaseFileOpen = true;

    this->populateTableWithRecords();

    this->ui->pathLineEdit->setText(fileName);

    this->writeLog("Opened an existing database file: " + fileName.toStdString() + ".");

    this->updateUI();
}

void MainWindow::saveDatabaseChanges()
{
    if (this->newDatabaseFile) {
        saveNewDatabaseFile();
        return;
    }

    try {
        write_database(this->m_database);
    } catch (...) {
        this->writeLog("Write operation failed!");
        return;
    }

    this->writeLog("All changes were saved to file: " + this->ui->pathLineEdit->text().toStdString());

    this->unsavedChanges = false;

    this->updateUI();
}

void MainWindow::saveNewDatabaseFile()
{
    QFileDialog dialog(this, "Save As");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("CRDX Files (*.crdx)");
    dialog.setDefaultSuffix("crdx");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    QString fileName = "";
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            fileName = files.at(0);
            this->m_database->setFilePath(fileName.toStdString());
        }
    } else {
        return;
    }

    try {
        write_database(this->m_database);
    } catch (...) {
        this->writeLog("Write operation failed!");
        return;
    }

    this->writeLog("All changes were saved to file: " + fileName.toStdString());

    this->ui->pathLineEdit->setText(fileName);
    this->unsavedChanges = false;
    this->newDatabaseFile = false;

    this->updateUI();
}

void MainWindow::closeDatabase()
{
    if (!handleUnsavedChanges())
        return;

    this->unsavedChanges = false;
    this->newDatabaseFile = false;
    this->databaseFileOpen = false;

    this->m_database.reset();

    Record::s_nextFreeId = 0;

    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setRowCount(0);

    this->ui->pathLineEdit->setText("");

    this->writeLog("Closed a database file.");

    this->updateUI();
}

void MainWindow::writeLog(const std::string& logMsg)
{
    if (logMsg == "") return;

    auto msg = QString::fromStdString(logMsg);

    QTime time = QTime::currentTime();

    this->ui->logTextEdit->append(time.toString("hh:mm:ss") + ": " + msg);
}

void MainWindow::updateUI()
{
    this->ui->action_Close->setDisabled(!this->databaseFileOpen);
    this->ui->action_Save->setDisabled(!this->databaseFileOpen);
    this->ui->actionSave_As->setDisabled(!this->databaseFileOpen);
    this->ui->tableWidget->setDisabled(!this->databaseFileOpen);

    this->ui->menuRecord->setDisabled(!this->databaseFileOpen);
    this->ui->actionCreate_New->setDisabled(!this->databaseFileOpen);
}

bool MainWindow::handleUnsavedChanges() {
    if (this->unsavedChanges) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm", "There are unsaved changes. Close without saving?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return false;
        }
        else {
            this->unsavedChanges = false;
            this->newDatabaseFile = false;
            this->databaseFileOpen = false;
            Record::s_nextFreeId = 0;
            return true;
        }
    }

    return true;
}

void MainWindow::setDatabasePassword() {
    if (!this->databaseFileOpen) {
        return;
    }

    PasswordDialog pwDialog(this);

    if (pwDialog.exec() == QDialog::Accepted) {
        std::string password = pwDialog.getPassword();
        this->m_database->setPassword(password);
        this->writeLog("Database password changed!");
    }
}

void MainWindow::createNewRecord() {
    if (!this->databaseFileOpen) {
        return;
    }

    RecordDialog recordDialog(this, "Create Record");

    if (recordDialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Record> record = recordDialog.getRecord();
        this->m_database->addNewRecord(record);
        this->populateTableWithRecords();
        this->writeLog("New record was created!");
    }
}

void MainWindow::populateTableWithRecords() {
    if (!this->databaseFileOpen)
        return;

    std::vector<std::shared_ptr<Record>> records = m_database->getRecords();

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    for (uint row = 0; row < records.size(); ++row) {
        auto record = records[row];

        int rowCount = ui->tableWidget->rowCount(); // get current row count
        ui->tableWidget->insertRow(rowCount);       // add a new row at the end

        QTableWidgetItem* titleItem = new QTableWidgetItem(QString::fromStdString(record->getTitle()));
        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);  // Remove editable flag
        titleItem->setData(Qt::UserRole, record->getId()); // store record id
        this->ui->tableWidget->setItem(row, 0, titleItem);

        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(record->getUsername()));
        usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);
        usernameItem->setData(Qt::UserRole, record->getId());
        this->ui->tableWidget->setItem(row, 1, usernameItem);

        QTableWidgetItem* passwordItem = new QTableWidgetItem("*****");  // Masked display
        passwordItem->setData(Qt::UserRole, record->getId());
        passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 2, passwordItem);

        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(record->getEmail()));
        emailItem->setData(Qt::UserRole, record->getId());
        emailItem->setFlags(emailItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 3, emailItem);

        QTableWidgetItem* urlItem = new QTableWidgetItem(QString::fromStdString(record->getUrl()));
        urlItem->setData(Qt::UserRole, record->getId());
        urlItem->setFlags(urlItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 4, urlItem);

        QTableWidgetItem* descriptionItem = new QTableWidgetItem(QString::fromStdString(record->getDescription()));
        descriptionItem->setData(Qt::UserRole, record->getId());
        descriptionItem->setFlags(descriptionItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 5, descriptionItem);
    }
}

void MainWindow::onRightClick(const QPoint &pos) {
    QTableWidgetItem *item = this->ui->tableWidget->itemAt(pos);
    if (!item) return;

    // Get record id.
    uint32_t id = item->data(Qt::UserRole).toInt();

    QMenu contextMenu;
    QAction* copyAction = contextMenu.addAction("Copy password");
    connect(copyAction, &QAction::triggered, this, [this, id]() {copyPassword(id);});

    QAction* editAction = contextMenu.addAction("Edit");
    connect(editAction, &QAction::triggered, this, [this, id]() {editRecord(id);});

    QAction* deleteAction = contextMenu.addAction("Delete");
    connect(deleteAction, &QAction::triggered, this, [this, id]() {deleteRecord(id);});

    contextMenu.exec(this->ui->tableWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::copyPassword(uint32_t recordId) {
    const std::shared_ptr<Record> record = this->m_database->getRecord(recordId);

    if (record == nullptr) {
        this->writeLog("Unexpected error when copying the password.");
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    QString password = QString::fromStdString(record->getPassword());
    clipboard->setText(password);

    this->writeLog("Password was copied to clipboard.");
}

void MainWindow::editRecord(const uint32_t recordId) {
    const std::shared_ptr<Record> record = this->m_database->getRecord(recordId);

    if (record == nullptr) {
        this->writeLog("Unexpected error when editing the record.");
        return;
    }

    RecordDialog recordDialog(this, "Edit Record");
    recordDialog.setRecord(record);

    if (recordDialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Record> record = recordDialog.getRecord();
        this->m_database->updateRecord(record);
        this->populateTableWithRecords();
        this->writeLog("Record was updated!");
    }
}

void MainWindow::deleteRecord(const uint32_t recordId) {
    const std::shared_ptr<Record> record = this->m_database->getRecord(recordId);

    if (record == nullptr) {
        this->writeLog("Unexpected error when deleting the record.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Are you sure you want to delete the record?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    this->m_database->deleteRecord(recordId);

    this->populateTableWithRecords();

    this->writeLog("Record was deleted!");
}


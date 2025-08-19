#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "password_dialog.h"
#include "record_dialog.h"
#include "db_writer.h"
#include "db_reader.h"
#include "QFileDialog"
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->frame->setStyleSheet("QFrame#frame { border: 1px solid white; }");

    // Set table columns to fill whole table.
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
        PasswordDialog pwDialog(this);

        if (pwDialog.exec() != QDialog::Accepted) {
            this->writeLog("Read operation failed!");
            return;
        }

        std::string password = pwDialog.getPassword();

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
        this->ui->tableWidget->setItem(row, 0, titleItem);

        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(record->getUsername()));
        usernameItem->setFlags(usernameItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 1, usernameItem);

        QTableWidgetItem* passwordItem = new QTableWidgetItem("*****");  // Masked display
        passwordItem->setData(Qt::UserRole, QString::fromStdString(record->getPassword()));  // Store actual password
        passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 2, passwordItem);

        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(record->getEmail()));
        emailItem->setFlags(emailItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 3, emailItem);

        QTableWidgetItem* urlItem = new QTableWidgetItem(QString::fromStdString(record->getUrl()));
        urlItem->setFlags(urlItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 4, urlItem);

        QTableWidgetItem* descriptionItem = new QTableWidgetItem(QString::fromStdString(record->getDescription()));
        descriptionItem->setFlags(descriptionItem->flags() & ~Qt::ItemIsEditable);
        this->ui->tableWidget->setItem(row, 5, descriptionItem);
    }
}

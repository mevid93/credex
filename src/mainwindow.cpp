#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "password_dialog.h"
#include "QFileDialog"
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set flag values.
    this->unsavedChanges = false;
    this->newDatabaseFile = false;
    this->databaseFileOpen = false;

    // Set some custom signals and slots
    connect(this->ui->action_Exit, &QAction::triggered, this, &MainWindow::closeApplication);
    connect(this->ui->action_New, &QAction::triggered, this, &MainWindow::newDatabase);
    connect(this->ui->action_Open, &QAction::triggered, this, &MainWindow::openExistingDatabase);
    connect(this->ui->action_Save, &QAction::triggered, this, &MainWindow::saveDatabaseChanges);
    connect(this->ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveNewDatabaseFile);
    connect(this->ui->action_Close, &QAction::triggered, this, &MainWindow::closeDatabase);

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

    this->database = std::make_unique<Database>();

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

    this->unsavedChanges = false;
    this->newDatabaseFile = false;
    this->databaseFileOpen = true;

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
        }
    } else {
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

    this->database.reset();

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
        this->database->setPassword(password);
        this->writeLog("Database password changed!");
    } else {
        return;
    }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include <memory>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// @brief Constructor.
    MainWindow(QWidget *parent = nullptr);

    /// @brief Destructor.
    ~MainWindow();

    /// @brief Writes a log message to UI textbox.
    /// @param logMsg Log message.
    void writeLog(const std::string& logMsg);

    /// @brief Update UI components based on internal state changes.
    void updateUI();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Database> database;

    // State flags.
    bool newDatabaseFile;
    bool unsavedChanges;
    bool databaseFileOpen;

    bool handleUnsavedChanges();

    void populateTableWithRecords();

private slots:
    /// @brief Close the application.
    void closeApplication();

    /// @brief New database file.
    void newDatabase();

    /// @brief Open an existing database file.
    void openExistingDatabase();

    /// @brief Save changes made for database file.
    void saveDatabaseChanges();

    /// @brief Save a new database file.
    void saveNewDatabaseFile();

    /// @brief Close database file.
    void closeDatabase();

    /// @brief Set database password.
    void setDatabasePassword();

    /// @brief Create new Record.
    void createNewRecord();
};
#endif // MAINWINDOW_H

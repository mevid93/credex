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

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Database> database;

    // State flags.
    bool newDatabaseFile;
    bool unsavedChanges;
    bool databaseFileOpen;

private slots:
    /// @brief Close the application.
    void closeApplication();

    /// @brief New database file.
    void newDatabase();

};
#endif // MAINWINDOW_H

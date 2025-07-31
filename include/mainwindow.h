#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;

private slots:
    /// @brief Close the application.
    void closeApplication();
};
#endif // MAINWINDOW_H

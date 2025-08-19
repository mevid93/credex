#ifndef PASSWORD_INPUT_DIALOG_H
#define PASSWORD_INPUT_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class PasswordInputDialog; }
QT_END_NAMESPACE

class PasswordInputDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Constructor.
    explicit PasswordInputDialog(QWidget *parent = nullptr);

    /// @brief Destructor.
    ~PasswordInputDialog();

    /// Get password.
    const std::string& getPassword() const;

private:
    Ui::PasswordInputDialog *ui;

    std::string m_password;

private slots:
    void okButtonPressed();
    void showButton1Pressed();
    void showButton1Released();
};

#endif // PASSWORD_INPUT_DIALOG_H

#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class PasswordDialog; }
QT_END_NAMESPACE

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Constructor.
    explicit PasswordDialog(QWidget *parent = nullptr);

    /// @brief Destructor.
    ~PasswordDialog();

    /// Get password.
    const std::string& getPassword() const;

private:
    Ui::PasswordDialog *ui;

    std::string m_password;

private slots:
    void okButtonPressed();
};

#endif // PASSWORD_DIALOG_H

#ifndef RECORD_DIALOG_H
#define RECORD_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class RecordDialog; }
QT_END_NAMESPACE

class RecordDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Constructor.
    explicit RecordDialog(QWidget *parent = nullptr, std::string title = "Record");

    /// @brief Destructor.
    ~RecordDialog();
private:
    Ui::RecordDialog *ui;

private slots:

};

#endif // RECORD_DIALOG_H

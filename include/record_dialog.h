#ifndef RECORD_DIALOG_H
#define RECORD_DIALOG_H

#include "record.h"
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

    /// @brief Get record.
    std::shared_ptr<Record> getRecord() const;

private:
    Ui::RecordDialog *ui;
    std::shared_ptr<Record> m_record;

private slots:
    void showButtonPressed();
    void showButtonReleased();
    void okButtonPressed();
};

#endif // RECORD_DIALOG_H

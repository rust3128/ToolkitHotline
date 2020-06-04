#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>


namespace Ui {
class ContactDialog;
}

class ContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContactDialog(int contID, uint clnID, QString clnName, QWidget *parent = nullptr);
    ~ContactDialog();

private slots:
    void on_lineEditFIO_textChanged(const QString &fio);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void createUI();
private:
    Ui::ContactDialog *ui;
    int contactID;
    uint clientID;

};

#endif // CONTACTDIALOG_H

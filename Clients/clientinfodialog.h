#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>

namespace Ui {
class ClientInfoDialog;
}

class ClientInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientInfoDialog(uint ID, QWidget *parent = nullptr);
    ~ClientInfoDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ClientInfoDialog *ui;
    uint clientID;
};

#endif // CLIENTINFODIALOG_H

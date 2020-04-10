#ifndef SERVERSDIALOG_H
#define SERVERSDIALOG_H

#include <QDialog>


namespace Ui {
class ServersDialog;
}

class ServersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServersDialog(int sID, uint clID, QString clName, QWidget *parent = nullptr);
    ~ServersDialog();

private slots:
    void changeInfo();
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void createUI();
private:
    Ui::ServersDialog *ui;
    int serverID;
    uint clientID;
};

#endif // SERVERSDIALOG_H

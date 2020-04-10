#ifndef CLIENTINFODIALOG_H
#define CLIENTINFODIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QItemSelection>

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

private slots:
    void slotSelectionContact(const QItemSelection &, const QItemSelection &);
    void on_toolButton_clicked();
    void on_tableViewServers_doubleClicked(const QModelIndex &idx);
    void on_groupBoxFBServer_clicked(bool checked);
    void on_toolButtonAddContact_clicked();

    void on_listViewContact_doubleClicked(const QModelIndex &index);

private:
    void createUI();
    void createConnections();
    void createTitle();
    void createServerBox();
    void createFBServerBox();
    void createContactsBox();
    void createPaytypesBox();
private:
    Ui::ClientInfoDialog *ui;
    uint clientID;
    uint fbServerID;
    int contactID;
    QSqlQueryModel *modelServers;
    QSqlQueryModel *modelContacts;
    QSqlQueryModel *modelPaytypes;
};

#endif // CLIENTINFODIALOG_H

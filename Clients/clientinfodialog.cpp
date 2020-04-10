#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "Clients/serversdialog.h"
#include "Clients/contactdialog.h"

#include <QSqlQuery>
#include <QSqlError>



ClientInfoDialog::ClientInfoDialog(uint ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog),
    clientID(ID)
{
    ui->setupUi(this);

    createUI();
    createConnections();

}

ClientInfoDialog::~ClientInfoDialog()
{
    delete ui;
}

void ClientInfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void ClientInfoDialog::createUI()
{
    createTitle();
    createServerBox();
    createFBServerBox();
    createContactsBox();
    createPaytypesBox();
}

void ClientInfoDialog::createConnections()
{
    connect(ui->listViewContact->selectionModel(),&QItemSelectionModel::selectionChanged,this,&ClientInfoDialog::slotSelectionContact);
}


void ClientInfoDialog::createTitle()
{
    QSqlQuery q;
    q.prepare("SELECT name, logo, comments FROM clients WHERE client_id = :clientID");
    q.bindValue(":clientID", clientID);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось получить данные о клиенте."  << q.lastError().text();
        this->reject();
    }
    q.next();
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(q.value(1).toByteArray());
    if(!outPixmap.isNull()){
        ui->labelLogo->setPixmap(  (outPixmap.size().width() > 90) ? outPixmap.scaledToWidth(90) : outPixmap  );
    }
    else
        ui->labelLogo->clear();
    ui->labelName->setText(q.value(0).toString());
    ui->labelComments->setText(q.value(2).toString());

}

void ClientInfoDialog::createServerBox()
{
    modelServers = new QSqlQueryModel(this);
    QString strSQL = QString("select s.server_id, s.address, s.comments from tservers s where s.client_id = %1").arg(clientID);
    modelServers->setQuery(strSQL);
    modelServers->setHeaderData(1,Qt::Horizontal,"Сервер");
    modelServers->setHeaderData(2,Qt::Horizontal,"Описание");


    ui->tableViewServers->setModel(modelServers);
    ui->tableViewServers->verticalHeader()->hide();
    ui->tableViewServers->hideColumn(0);
    ui->tableViewServers->hideColumn(3);
    ui->tableViewServers->resizeColumnsToContents();
    ui->tableViewServers->resizeRowsToContents();
    ui->tableViewServers->horizontalHeader()->setStyleSheet("color: blue;");
    ui->tableViewServers->horizontalHeader()->setStretchLastSection(true);
}

void ClientInfoDialog::createFBServerBox()
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("SELECT COUNT(*) FROM fbservers where client_id = :clientID");
    q->bindValue(":clientID", clientID);
    q->exec();
    q->next();
    if(q->value(0) == 0){
        q->prepare("INSERT INTO FBSERVERS (CLIENT_ID, SERVERNAME, DATABASENAME, USERNAME, PASS) VALUES (:clientID, '', '', '', '');");
        q->bindValue(":clientID", clientID);
        q->exec();
    }

    q->prepare("select s.server_id, s.servername, s.databasename, s.username, s.pass, s.comment from fbservers s where s.client_id = :clientID");
    q->bindValue(":clientID",clientID);
    q->exec();
    q->next();
    fbServerID = q->value(0).toUInt();
    ui->lineEditFBServer->setText(q->value(1).toString());
    ui->lineEditFBDatabase->setText(q->value(2).toString());
    ui->lineEditFBUser->setText(q->value(3).toString());
    ui->lineEditFBPassword->setText(q->value(4).toString());
    ui->plainTextEditFBComments->setPlainText(q->value(5).toString());
}

void ClientInfoDialog::createContactsBox()
{
    modelContacts = new QSqlQueryModel();
    QString strSQL = QString("select c.contact_id, c.name, c.phone, c.email,c.comment,c.isactive from contacts c where c.client_id = %1").arg(clientID);
    modelContacts->setQuery(strSQL);

    ui->listViewContact->setModel(modelContacts);
    ui->listViewContact->setModelColumn(1);
    ui->frameContactInfo->hide();

}

void ClientInfoDialog::createPaytypesBox()
{
    modelPaytypes = new QSqlQueryModel(this);
    QString strSQL = QString("select p.paytype_id, p.clnpaytype_id, p.name, p.dllname from paytypes p "
                             "where p.client_id = %1 and p.isactive = 'true' "
                             "order by p.clnpaytype_id").arg(clientID);
    modelPaytypes->setQuery(strSQL);
    modelPaytypes->setHeaderData(1,Qt::Horizontal,"ID");
    modelPaytypes->setHeaderData(2,Qt::Horizontal,"Наименование");
    modelPaytypes->setHeaderData(3,Qt::Horizontal,"Библиотека");

    ui->tableViewPaytypes->setModel(modelPaytypes);
    ui->tableViewPaytypes->verticalHeader()->hide();
    ui->tableViewPaytypes->hideColumn(0);
    ui->tableViewPaytypes->resizeColumnsToContents();
    ui->tableViewPaytypes->resizeRowsToContents();
}

void ClientInfoDialog::slotSelectionContact(const QItemSelection &, const QItemSelection &)
{
    ui->frameContactInfo->show();
    QModelIndexList selection = ui->listViewContact->selectionModel()->selectedIndexes();

    ui->labelContactActive->setText( (modelContacts->data(modelContacts->index(selection.at(0).row(),4)).toBool()) ? "Работает" : "Не работает" );
    ui->lineEditContactPhone->setText(modelContacts->data(modelContacts->index(selection.at(0).row(),2)).toString());
    ui->lineEditContactEmail->setText(modelContacts->data(modelContacts->index(selection.at(0).row(),3)).toString());
    ui->plainTextEditContactComment->setPlainText(modelContacts->data(modelContacts->index(selection.at(0).row(),4)).toString());

}

void ClientInfoDialog::on_toolButton_clicked()
{
    ServersDialog *srvDlg = new ServersDialog(-1,clientID,ui->labelName->text(),this);
    srvDlg->exec();
    modelServers->setQuery(modelServers->query().lastQuery());
}

void ClientInfoDialog::on_tableViewServers_doubleClicked(const QModelIndex &idx)
{
    int serverID = modelServers->data(modelServers->index(idx.row(),0)).toInt();
    ServersDialog *srvDlg = new ServersDialog(serverID,clientID,ui->labelName->text(),this);
    srvDlg->exec();
    modelServers->setQuery(modelServers->query().lastQuery());
}


void ClientInfoDialog::on_groupBoxFBServer_clicked(bool checked)
{
    if(!checked){
        QSqlQuery *q = new QSqlQuery();
        q->prepare("UPDATE FBSERVERS SET SERVERNAME = :serverName, DATABASENAME = :databaseName, USERNAME = :userName, "
                   "PASS = :password, COMMENT = :comment WHERE (SERVER_ID = :serverID)");
        q->bindValue(":serverName", ui->lineEditFBServer->text().trimmed());
        q->bindValue(":databaseName", ui->lineEditFBDatabase->text().trimmed());
        q->bindValue(":userName", ui->lineEditFBUser->text().trimmed());
        q->bindValue(":password", ui->lineEditFBPassword->text().trimmed());
        q->bindValue(":comment", ui->plainTextEditFBComments->toPlainText().trimmed());
        q->bindValue(":serverID", fbServerID);
        q->exec();
    }
}

void ClientInfoDialog::on_toolButtonAddContact_clicked()
{
    ContactDialog *contDlg = new ContactDialog(-1, clientID, ui->labelName->text(), this);
    contDlg->exec();
}

void ClientInfoDialog::on_listViewContact_doubleClicked(const QModelIndex &idx)
{
    ContactDialog *contDlg = new ContactDialog(modelContacts->data(modelContacts->index(idx.row(),0)).toInt(),clientID,ui->labelName->text(),this);
    contDlg->exec();
}

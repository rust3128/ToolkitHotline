#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"
#include "LoggingCategories/loggingcategories.h"

#include <QSqlQuery>
#include <QSqlError>

ClientInfoDialog::ClientInfoDialog(uint ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog),
    clientID(ID)
{
    ui->setupUi(this);

    createUI();

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

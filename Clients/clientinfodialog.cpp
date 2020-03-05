#include "clientinfodialog.h"
#include "ui_clientinfodialog.h"
#include "LoggingCategories/loggingcategories.h"

ClientInfoDialog::ClientInfoDialog(uint ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDialog),
    clientID(ID)
{
    ui->setupUi(this);
    qInfo(logInfo()) << "Client ID " << clientID;
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

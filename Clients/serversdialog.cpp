#include "serversdialog.h"
#include "ui_serversdialog.h"
#include "LoggingCategories/loggingcategories.h"

#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>


ServersDialog::ServersDialog(int sID, uint clID, QString clName,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServersDialog),
    serverID(sID),
    clientID(clID)
{
    ui->setupUi(this);
    ui->labelClients->setText(clName);
    createUI();
    connect(ui->lineEditServer, &QLineEdit::textChanged, this, &ServersDialog::changeInfo);
    connect(ui->plainTextEditComments, &QPlainTextEdit::textChanged, this, &ServersDialog::changeInfo);
}

ServersDialog::~ServersDialog()
{
    delete ui;
}

void ServersDialog::changeInfo()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled((ui->plainTextEditComments->toPlainText().length()> 5) && (ui->lineEditServer->text().length()>5));
}

void ServersDialog::createUI()
{
    ui->lineEditServer->setFocus();
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    if(serverID >= 0){
        this->setWindowTitle("Редактирование сервера");
        QSqlQuery *q = new QSqlQuery();
        q->prepare("select s.address, s.comments from tservers s where s.server_id = :serverID");
        q->bindValue(":serverID", serverID);
        q->exec();
        q->next();
        ui->lineEditServer->setText(q->value(0).toString());
        ui->plainTextEditComments->setPlainText(q->value(1).toString());
    } else {
        this->setWindowTitle("Новый сервер");

    }

}

void ServersDialog::on_buttonBox_accepted()
{
    QSqlQuery *q = new QSqlQuery();

    if(serverID == -1){
        q->prepare("INSERT INTO TSERVERS (CLIENT_ID, ADDRESS, COMMENTS) VALUES (:clientID, :sAddress, :sComments)");
    } else {
        q->prepare("UPDATE TSERVERS SET ADDRESS = :sAddress, COMMENTS = :sComments WHERE SERVER_ID = :serverID");
    }
    q->bindValue(":clientID", clientID);
    q->bindValue(":sAddress", ui->lineEditServer->text().trimmed());
    q->bindValue(":sComments", ui->plainTextEditComments->toPlainText().trimmed());
    q->bindValue(":serverID", serverID);

    if(!q->exec()){
        qCritical(logCritical()) << "Не удалось обновить информацию о сервере." << q->lastError().text();
        QMessageBox::critical(this,"Ошибка", "Не удалось обновить информацию о сервере!\n"+q->lastError().text().trimmed());
        this->reject();
    }
    qInfo(logInfo()) << "Данные о сервере "+ui->lineEditServer->text().trimmed()+" успешно обновлены.";
    this->accept();
}

void ServersDialog::on_buttonBox_rejected()
{
    this->reject();
}

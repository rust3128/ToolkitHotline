#include "contactdialog.h"
#include "ui_contactdialog.h"
#include "LoggingCategories/loggingcategories.h"

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

ContactDialog::ContactDialog(int contID, uint clnID, QString clnName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactDialog),
    contactID(contID),
    clientID(clnID)
{
    ui->setupUi(this);
    ui->labelClientName->setText(clnName);
    createUI();
}

ContactDialog::~ContactDialog()
{
    delete ui;
}

void ContactDialog::createUI()
{
    if(contactID == -1){
        this->setWindowTitle("Новый контакт");
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    } else {
        this->setWindowTitle("Редактировать контакт");
        QSqlQuery *q = new QSqlQuery();
        q->prepare("select c.name, c.phone, c.email,c.comment,c.isactive from contacts c where c.contact_id = :contactID");
        q->bindValue(":contactID", contactID);
        q->exec();
        q->next();
        ui->lineEditFIO->setText(q->value(0).toString());
        ui->lineEditContactPhone->setText(q->value(1).toString());
        ui->lineEditContactEmail->setText(q->value(2).toString());
        ui->plainTextEditContactComment->setPlainText(q->value(3).toString());
        ui->checkBoxIsActive->setChecked( q->value(4).toBool() );
    }
}

void ContactDialog::on_lineEditFIO_textChanged(const QString &fio)
{
    if(fio.length() >= 3){
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

void ContactDialog::on_buttonBox_accepted()
{
    QSqlQuery *q = new QSqlQuery();
    if(contactID == -1){
        q->prepare("INSERT INTO CONTACTS (CLIENT_ID, NAME, PHONE, EMAIL, COMMENT) VALUES (:clientID, :name, :phone, :email, :comment)");
    } else {
        q->prepare("UPDATE CONTACTS SET NAME = :name , PHONE = :phone, EMAIL = :email, COMMENT = :comment, ISACTIVE = :isactive WHERE CONTACT_ID = :contactID");
    }
    q->bindValue(":clientID", clientID);
    q->bindValue(":name", ui->lineEditFIO->text().trimmed());
    q->bindValue(":phone", ui->lineEditContactPhone->text().trimmed());
    q->bindValue(":email",ui->lineEditContactEmail->text().trimmed());
    q->bindValue(":comment", ui->plainTextEditContactComment->toPlainText().trimmed());
    q->bindValue(":isactive", QVariant(ui->checkBoxIsActive->isChecked()).toString());
    q->bindValue(":contactID",contactID);
    if(!q->exec()){
        qCritical(logCritical()) << "Не удалось обновить информацию о контакте" << q->lastError().text();
        this->reject();
    }
    qInfo(logInfo()) << "Информация о контакте "+ui->lineEditFIO->text().trimmed()+" успешно обновлена.";
    this->accept();
}

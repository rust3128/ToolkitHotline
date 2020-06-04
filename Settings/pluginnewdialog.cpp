#include "pluginnewdialog.h"
#include "ui_pluginnewdialog.h"
#include <QPushButton>

PluginNewDialog::PluginNewDialog(int ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginNewDialog),
    pluginID(ID)
{
    ui->setupUi(this);
    createUI();
}

PluginNewDialog::~PluginNewDialog()
{
    delete ui;
}

void PluginNewDialog::createUI()
{
    modelType = new QSqlQueryModel(this);
    modelType->setQuery("select setuptype_id, typename from pluginssetuptype order by setuptype_id");
    ui->comboBox->setModel(modelType);
    ui->comboBox->setModelColumn(1);
    ui->comboBox->setCurrentIndex(-1);
    if(pluginID<0){
        this->setWindowTitle("Новый плагин");
    } else {
        this->setWindowTitle("Редактирование плагина");
    }
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

void PluginNewDialog::on_buttonBox_rejected()
{
    this->reject();
}

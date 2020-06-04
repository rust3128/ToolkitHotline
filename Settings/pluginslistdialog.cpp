#include "pluginslistdialog.h"
#include "ui_pluginslistdialog.h"
#include "Settings/pluginnewdialog.h"

PluginsListDialog::PluginsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginsListDialog)
{
    ui->setupUi(this);
}

PluginsListDialog::~PluginsListDialog()
{
    delete ui;
}

void PluginsListDialog::on_pushButtonAdd_clicked()
{
    PluginNewDialog *addPlgDlg = new PluginNewDialog(-1,this);
    addPlgDlg->exec();
}

#include "pluginslistdialog.h"
#include "ui_pluginslistdialog.h"

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

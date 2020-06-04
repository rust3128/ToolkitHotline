#ifndef PLUGINSLISTDIALOG_H
#define PLUGINSLISTDIALOG_H

#include <QDialog>

namespace Ui {
class PluginsListDialog;
}

class PluginsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginsListDialog(QWidget *parent = nullptr);
    ~PluginsListDialog();

private slots:
    void on_pushButtonAdd_clicked();

private:
    Ui::PluginsListDialog *ui;
};

#endif // PLUGINSLISTDIALOG_H

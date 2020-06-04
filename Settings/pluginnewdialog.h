#ifndef PLUGINNEWDIALOG_H
#define PLUGINNEWDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class PluginNewDialog;
}

class PluginNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginNewDialog(int ID, QWidget *parent = nullptr);
    ~PluginNewDialog();
private slots:
    void on_buttonBox_rejected();

private:
    void createUI();
private:
    Ui::PluginNewDialog *ui;
    int pluginID;
    QSqlQueryModel *modelType;

};

#endif // PLUGINNEWDIALOG_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionSettingDialog/connectionsettingdialog.h"
#include "GlobalSettings/globalsettings.h"
#include "OptionsDialog/optionsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "Clients/clientslistdialog.h"
#include "DynamicButton/dynamiicbutton.h"
#include "Clients/clientinfodialog.h"
#include "Clients/addclientdialog.h"
#include "Settings/pluginslistdialog.h"
#include "Settings/migrateoptionsdialog.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCloseEvent>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
    setToolBarClients();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    /* Инициализируем иконку трея, устанавливаем иконку из набора системных иконок,
     * а также задаем всплывающую подсказку
     * */
    trayIcon = new QSystemTrayIcon(QIcon(":/Images/supporticon.png"), this);
    trayIcon->setToolTip("Workplace Hotline" "\n"
                         "Рабочее место сотрудника горячей линии");
    /* После чего создаем контекстное меню из двух пунктов*/
    QMenu * trayMenu = new QMenu(this);
    QAction * viewWindow = new QAction(tr("Развернуть окно"), this);
    QAction * quitAction = new QAction(tr("Выход"), this);

    /* подключаем сигналы нажатий на пункты меню к соответсвующим слотам.
     * Первый пункт меню разворачивает приложение из трея,
     * а второй пункт меню завершает приложение
     * */
    connect(viewWindow, &QAction::triggered, this, &QWidget::show);
    connect(quitAction, &QAction::triggered, this, &MainWindow::slotExit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayMenu->addAction(viewWindow);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    /* Устанавливаем контекстное меню на иконку
     * и показываем иконку приложения в трее
     * */
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    /* Также подключаем сигнал нажатия на иконку к обработчику
     * данного нажатия
     * */
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

}

void MainWindow::setToolBarClients()
{


    QSqlQuery q;

    q.prepare("SELECT client_id, name FROM clients WHERE isactive = 'true' ORDER BY cnt, client_id");
    if(!q.exec()){
        qCritical(logCritical()) << "Не возможно получить список клиентов" << q.lastError().text();
        return;
    }
    while(q.next()){
         DynamiicButton *button = new DynamiicButton(q.value(0).toUInt(),this);
         button->setText(q.value(1).toString());
//         button->setStyleSheet("color: white;"
//                               "background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #88d, stop: 0.1 #444450, stop: 0.49 #444450, stop: 0.5 #3c3c46, stop: 1 #1f1f24);"
//                               "border-width: 1px;"
//                               "border-color: #141529;"
//                               "border-style: solid;"
//                               "border-radius:4;"
//                               "padding: 3px;"
//                               "font-size: 16px;"
//                               "font-weight: bold;"
//                               "font-variant: small-caps;"
//                               "font-family: serif;"
//                               "padding-left:5px;"
//                               "padding-right: 5px;"
//                               "min-width: 100px;"
//                               "max-width: 100px;"
//                               "min-height: 15px;"
//                               "max-height: 15px;");
         button->setStyleSheet("font-size: 16px;"
                               "font-weight: bold;"
                               "font-variant: small-caps;"
                               "font-family: serif;"
                               "padding-left:10px;"
                               "padding-right:10px;"
                               "min-width: 100px;"
                               "max-width: 100px;"
                               "min-height: 20px;"
                               "max-height: 20px;");
        button->setFlat(true);
        ui->toolBarClients->addSeparator();
        ui->toolBarClients->addWidget(button);

         connect(button,&QAbstractButton::clicked,this,&MainWindow::slotGetNumberButton);
    }
    q.finish();
    ui->toolBarClients->addSeparator();


    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacerWidget->setVisible(true);
    QPushButton *pbNewClient = new QPushButton(this);
    pbNewClient->setStyleSheet("font-size: 16px;"
                             "font-weight: bold;"
                             "font-variant: small-caps;"
                             "font-family: serif;"
                             "padding-left:10px;"
                             "padding-right:10px;"
                             "min-width: 100px;"
                             "max-width: 100px;"
                             "min-height: 20px;"
                             "max-height: 20px;");
    pbNewClient->setText("Новый клиент");
    pbNewClient->setFlat(true);

    ui->toolBarClients->addWidget(spacerWidget);
    ui->toolBarClients->addSeparator();
    ui->toolBarClients->addWidget(pbNewClient);
    connect(pbNewClient,&QAbstractButton::clicked,[]() {
        QSqlRecord r;
        r.clear();
        AddClientDialog *addClnDlg = new AddClientDialog(&r);
        addClnDlg->exec();
    });

}


void MainWindow::on_actionDatabase_triggered()
{
    ConnectionSettingDialog *connDlg = new ConnectionSettingDialog(this);
    connDlg->setWindowTitle(this->windowTitle()+" - [Настройка базы данных]");
    connDlg->exec();
}

void MainWindow::on_actionParams_triggered()
{
    OptionsDialog *optDlg = new OptionsDialog(this);
    optDlg->setWindowTitle(this->windowTitle()+" - [Настройка]");
    optDlg->exec();
}

void MainWindow::slotExit()
{
    toClose=true;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        /* Событие игнорируется в том случае, если чекбокс не отмечен
         * */
            /* иначе, если окно видимо, то оно скрывается,
             * и наоборот, если скрыто, то разворачивается на экран
             * */
            if(!this->isVisible()){
                this->show();
            } else {
                this->hide();
        }
        break;
    default:
        break;
    }
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    QSqlQuery q;
        bool toTray;
        q.prepare("select o.optionvalue from options o where o.option_id=:optionID");
        q.bindValue(":optionID",GlobalSettings::OPT_MIMIMAZE_TO_TRAY);
        if(!q.exec()){
            qCritical(logCritical()) << "Не возможно получть значение параметра " << GlobalSettings::OPT_MIMIMAZE_TO_TRAY
                                     << q.lastError().text();
        }
        q.next();
        toTray = q.value(0).toBool();
        /* Если окно видимо и чекбокс отмечен, то завершение приложения
         * игнорируется, а окно просто скрывается, что сопровождается
         * соответствующим всплывающим сообщением
         */
        if(this->isVisible() && !q.isValid()){
            // Отображаем MessageBox c вопросом о закрытии
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Закрыть программу"));
            msgBox.setText(tr("<b>Свернуть программу в трей вместо закрытия?</b><p>(Изменить параметр можно в меню <b>Настройка</b>)."));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.addButton(QMessageBox::Yes);
            msgBox.addButton(QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            QCheckBox dontShowCheckBox(tr("Запомнить мой выбор."));
            dontShowCheckBox.blockSignals(true);
            msgBox.addButton(&dontShowCheckBox, QMessageBox::ResetRole);
            int32_t userReply = msgBox.exec();
            toTray = (userReply == QMessageBox::Yes) ? true : false;
            if(dontShowCheckBox.isChecked()){
                q.prepare("INSERT INTO OPTIONS (OPTION_ID, OPTIONVALUE, COMMENT) "
                          "VALUES (:optionID, :optionValue, :optComment)");
                q.bindValue(":optionID", GlobalSettings::OPT_MIMIMAZE_TO_TRAY);
                q.bindValue(":optionValue",toTray);
                q.bindValue(":optComment", "Сворачивать программу в трей");
                q.exec();
            }
        }
        if(this->isVisible() && toTray && !toClose){
            event->ignore();
            this->hide();
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

            trayIcon->showMessage("Toolkit Hotline",
                                  tr("Приложение свернуто в трей. Для того чтобы, "
                                      "развернуть окно приложения, щелкните по иконке приложения в трее"),
                                  icon,
                                  2000);
        }
}

void MainWindow::on_actionClientList_triggered()
{
    ClientsListDialog *clnLstDlg = new ClientsListDialog(this);
    clnLstDlg->setWindowTitle(this->windowTitle()+" - [Список клиентов]");
    clnLstDlg->exec();
}

void MainWindow::slotGetNumberButton()
{
    DynamiicButton *button = static_cast<DynamiicButton*>(sender());
    ClientInfoDialog *clnInfDlg = new ClientInfoDialog(button->getButtonID());
    this->setCentralWidget(clnInfDlg);
    clnInfDlg->exec();
}


void MainWindow::on_actionPlugins_triggered()
{
    PluginsListDialog *plgLstDlg = new PluginsListDialog(this);
    plgLstDlg->exec();
}

void MainWindow::on_actionMigrateOptions_triggered()
{
    MigrateOptionsDialog *mgOptDlg = new MigrateOptionsDialog();
    this->setCentralWidget(mgOptDlg);
    mgOptDlg->exec();
}

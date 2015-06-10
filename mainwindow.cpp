#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем все используемые окна
    ChooseAdress = new Dialog;
    Menu = new MainMenu;
    SettingsDialog = new Settings;

    // Получает сообщение, шифрует и отправляет
    Encryptor = new MyClass;
    // Получает шифрованное сообщение, расшифровывает и отправляет в приложение для вывода
    Decryptor = new MyClass;
    // Записывает в log файл историю сообщений
    Log = new MyClass;

    // Создаем новый клиент
    Client = new MyClient(this);

    // Создаем соответствующие потоки
    _Encryptor = new QThread(this);
    _Decryptor = new QThread(this);
    _Log = new QThread(this);

    // Породним объекты с потоками
    Encryptor->moveToThread(_Encryptor);
    Decryptor->moveToThread(_Decryptor);
    Log->moveToThread(_Log);

    // соединяем сигнал на старт потока со слотом MessageCreated
    connect(_Encryptor, SIGNAL(started()), this, SLOT(MessageCreated()));

    // Соединяем сигнал на окончание операции с слотом закрытия потока
    connect(Encryptor, SIGNAL(EncryptionFinished()), _Encryptor, SLOT(quit()));

    // Соединяем сигнал на окончание операции с слотом закрытия потока
    connect(Decryptor, SIGNAL(DecryptionFinished()), _Decryptor, SLOT(quit()));

    // Закрываем поток _Log по сигналу EndLog
    connect(Log, SIGNAL(EndLog()), _Log, SLOT(quit()));

    // По нажатию enter (или по нажатию на кнопку Send) вызывается слот on_pbSend_clicked
    connect(ui->lineEdit, SIGNAL(returnPressed()), this,
                                        SLOT(on_pbSend_clicked()));

    // Передаем строчку для записи в файл в соответсвующий слот объекта Log
    connect(this, SIGNAL(SendToLog(const QString*)), Log,
                                        SLOT(WriteToLog(const QString*)));

    // Передаем строчку, полученную из текстового поля для её шировки и отправления
    connect(this, SIGNAL(EncryptMessage(const QString*)), Encryptor ,
                                        SLOT(ReceiveMessage(const QString*)));

    // Отправляем зашифрованное сообщение собеседнику
    connect(Encryptor, SIGNAL(SendMessageToClient(const QString*)), Client,
                                        SLOT(SendMessageOut(const QString*)));

    // Decryptor получает сообщение от клиента
    connect(Client , SIGNAL(NewMessage(const QString*)), this ,
                                        SLOT(MessageReceived(const QString*)));

    // Расшифрованную строчку передаем сразу на вывод в textBrowser
    connect(Decryptor, SIGNAL(ShowMessage(const QString)), ui->textBrowser ,
                                        SLOT(append(QString)));

    // Расшифрованную строчку передаем на запись в Log
    connect(Decryptor, SIGNAL(LogMessage(const QString*)), Log ,
                                        SLOT(WriteToLog(const QString*)));

    // Отправляем IP и порт для соединения
    connect(ChooseAdress, SIGNAL(Connect(QHostAddress, quint16)), Client,
                                    SLOT(Connect(const QHostAddress, quint16)));

    // Сигнал о закрытии окна при установки соединения
    connect(Client, SIGNAL(ConnectionValid()), ChooseAdress,
                                    SLOT(hide()));
    connect(Client, SIGNAL(ConnectionValid()), Menu,
                                    SLOT(hide()));
    connect(Client, SIGNAL(ConnectionValid()), this, SLOT(show()));

    // При нажатии на выход возвращаемся в главное меню
    connect(ChooseAdress, SIGNAL(BackToMenu()), Menu, SLOT(show()));
    connect(ChooseAdress, SIGNAL(BackToMenu()), ChooseAdress, SLOT(hide()));

    // По нажатию на "соединение" в меню закрываем меню и открываем диалог
    connect(Menu, SIGNAL(Connect()), ChooseAdress, SLOT(show()));
    connect(Menu, SIGNAL(Connect()), Menu, SLOT(hide()));

    // Задание пользователем параметров
    connect(Menu, SIGNAL(ChangeSettings()), this, SLOT(ShowSettings()));
    connect(this, SIGNAL(SendSettings(const QString, uint)),
                    SettingsDialog, SLOT(ShowSettings(const QString, uint)));

    // Открываем окно настроек и скрываем меню
    connect(Menu, SIGNAL(ChangeSettings()), SettingsDialog, SLOT(show()));
    connect(Menu, SIGNAL(ChangeSettings()), Menu, SLOT(hide()));

    // Сохраним настройки
    connect(SettingsDialog, SIGNAL(NewSettings(const QString, uint)),
                        this, SLOT(NewSettings(const QString, uint)));

    // Или не сохраним, оставив прошлые (по нажатию на "Отмена")
    connect(SettingsDialog, SIGNAL(Cancel()), Menu, SLOT(show()));
    connect(SettingsDialog, SIGNAL(Cancel()), SettingsDialog, SLOT(hide()));

    // Выведем IP на экран настроек
    connect(Menu, SIGNAL(ChangeSettings()), Client, SLOT(RequestServerIP()));
    connect(Client, SIGNAL(ShowIP(QHostAddress)), SettingsDialog, SLOT(ShowIP(QHostAddress)));

    Menu->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Encryptor;
    delete Decryptor;
    delete Log;
    delete Client;
    delete Menu;
    delete SettingsDialog;
}

void MainWindow::MessageReceived(const QString *str)
{
    if(!_Decryptor->isRunning())
        _Decryptor->start();
    if(!_Log->isRunning())
        _Log->start();

    emit Decryptor->ReceiveEncryptedMessage(str);
}

void MainWindow::MessageCreated()
{
    QString *str;
    str = new QString;

    // Считываем сообщение из текстовой строки (lineEdit)
    *str = ui->lineEdit->text();

    // Если строка не пустая, то отправляем её на зашифровку и на запись в лог
    if(!str->isEmpty())
    {
        *str = QString("(%2:%3:%4) %1: %5").arg(Encryptor->GetUserName())
                             .arg(QString::number(QTime::currentTime().hour()))
                             .arg(QString::number(QTime::currentTime().minute()))
                             .arg(QString::number(QTime::currentTime().second()))
                             .arg(*str);

        QString *Str;
        Str = new QString;
        *Str = QString("<font color=green>%1</font>").arg(*str);

        emit EncryptMessage(str);
        ui->textBrowser->append(*Str);
        emit SendToLog(str);

        // Очищаем строку ввода
        ui->lineEdit->setText("");
    }
    else _Encryptor->exit();
}

void MainWindow::on_pbSend_clicked()
{
    // Если поток обработки сообщения еще не запущен, то запускаем
    if(!_Encryptor->isRunning())
        _Encryptor->start();
    if(!_Log->isRunning())
        _Log->start();
}

void MainWindow::ShowSettings()
{
    emit SendSettings(Encryptor->GetUserNameW(), Client->GetPort());
}

void MainWindow::NewSettings(const QString Name, uint Port)
{
    Encryptor->SetUserName(&Name);
    Client->SetPortW(Port);
    SettingsDialog->setVisible(FALSE);
    Menu->setVisible(TRUE);
}

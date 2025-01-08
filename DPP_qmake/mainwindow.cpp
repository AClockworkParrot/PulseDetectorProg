//файл описания функций обработки виджетов
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    // Устанавливаем сцену в QGraphicsView
    ui->graphicsView->setScene(scene);

    // Инициализируем столбцы
    bar1 = scene->addRect(0, 0, 50, 0, QPen(Qt::blue), QBrush(Qt::blue));
    bar2 = scene->addRect(60, 0, 50, 0, QPen(Qt::red), QBrush(Qt::red));
    bar3 = scene->addRect(120, 0, 50, 0, QPen(Qt::green), QBrush(Qt::green));
    bar4 = scene->addRect(180, 0, 50, 0, QPen(Qt::yellow), QBrush(Qt::yellow));

    // Перемещаем сцену, чтобы ось Y начиналась снизу
    scene->setSceneRect(0, -200, 250, 200);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    // Нажатие на кнопку connectButton вызывает слот onConnect.
    connect(this->ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnect);
    //Нажатие на кнопку setTimeButton вызывает слот onSetButtonClick.
    //connect(this->ui->setKoeffButton, &QPushButton::clicked, this, &MainWindow::onSetKoeffClick);

    //Сигнал timeChanged объекта arduinoClock вызывает слот updateTime.
    connect(&this->arduinoNano, &ArduinoNano::dataChanged, this, &MainWindow::updateData);
    this->updateData(0,0,0,0);

    //Сигнал timeChanged объекта arduinoClock вызывает слот updateTime.
    connect(&this->arduinoNano, &ArduinoNano::dataChanged, this, &MainWindow::updateHistogram);
    this->updateHistogram(0,0,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}



// Слот onConnect
void MainWindow::onConnect()
{
    // arduinoClock.stop(): Завершает текущее соединение с портом.
    this->arduinoNano.stop();
    // ui->portNameEdit->text() считывает имя порта, введённое в поле ввода.
    QString portName = this->ui->portNameEdit->text();
    //arduinoClock.start(portName): Инициирует соединение с указанным портом.
    this->arduinoNano.start(portName);
    // qDebug(): Логирует нажатие кнопки и имя порта.
    // ui->lcdNumber->display("3"): Отображает в виджете lcdNumber значение «3» для теста.
    qDebug() << "connect button pressed" << portName << "...";
    ui->lcdNumber->display("error");
}


// Слот onSetButtonClick
void MainWindow::onSetKoeffClick()
{
    /*
    // ui->hhEdit, ui->mmEdit, ui->ssEdit — поля ввода для часов, минут и секунд.
    // text().toUInt() конвертирует введённый текст в число.
    uint hh = ui->hhEdit->text().toUInt();
    uint mm = ui->mmEdit->text().toUInt();
    uint ss = ui->ssEdit->text().toUInt();

    // arduinoClock.setTime(hh, mm, ss): Отправляет команду для установки времени на Arduino.
    this->arduinoClock.setTime(hh, mm, ss);
    */
}


// Слот updateTime
void MainWindow::updateData(int a, int b, int c, int d)
{
    // QString().asprintf(): Форматирует часы, минуты и секунды в строку вида HH:MM:SS (с ведущими нулями).
    //QString data = QString().number(hh) + ":" + QString().number(mm) + ":" + QString().number(ss);
    //ui->lcdNumber->display(data);
    QString data = QString().asprintf("%2.2d", a) + ":" + QString().asprintf("%2.2d", b) + ":" + QString().asprintf("%2.2d", c)
                   + ":" + QString().asprintf("%2.2d", d);
    //ui->lcdNumber->display(data): Отображает время на виджете lcdNumber.
    ui->lcdNumber->display(data);
}


void MainWindow::updateHistogram(int counter1, int counter2, int counter3, int counter4)
{
    // Обновляем высоту столбцов
    bar1->setRect(0, -counter1, 50, counter1);
    bar2->setRect(60, -counter2, 50, counter2);
    bar3->setRect(120, -counter3, 50, counter3);
    bar4->setRect(180, -counter4, 50, counter4);
}

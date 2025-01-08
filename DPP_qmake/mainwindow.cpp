//файл описания функций обработки виджетов
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QString>
#include <QLineF>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , isBarChart(false) // Изначально отображается гистограмма
{
    ui->setupUi(this);

    // Добавляем линию
    lineSeries << QPointF(25, 0) << QPointF(85, 0) << QPointF(145, 0) << QPointF(205, 0);

    // Перемещаем сцену, чтобы ось Y начиналась снизу
    scene->setSceneRect(0, -200, 250, 200);
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    // Нажатие на кнопку connectButton вызывает слот onConnect
    connect(this->ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnect);

    connect(ui->toggleChartButton, &QPushButton::clicked, this, &MainWindow::toggleChartType);


    // Сигнал dataChanged объекта arduinoNano вызывает слоты updateData и updateChart
    connect(&this->arduinoNano, &ArduinoNano::dataChanged, this, &MainWindow::updateData);
    connect(&this->arduinoNano, &ArduinoNano::dataChanged, this, &MainWindow::updateChart);

    this->updateData(0, 0, 0, 0);
    this->updateChart(0, 0, 0, 0);
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


void MainWindow::toggleChartType()
{
    // Переключаем тип графика
    isBarChart = !isBarChart;

    // Обновляем график
    updateChart(10, 20, 30, 40); // Пример данных для обновления
}


// Слот updateData
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




void MainWindow::updateChart(int counter1, int counter2, int counter3, int counter4)
{
    // Очищаем старые элементы (если они существуют)
    scene->clear();

    if (isBarChart) {
        // Добавляем гистограмму
        bar1 = scene->addRect(0, -counter1, 50, counter1, QPen(Qt::blue), QBrush(Qt::blue));
        bar2 = scene->addRect(60, -counter2, 50, counter2, QPen(Qt::red), QBrush(Qt::red));
        bar3 = scene->addRect(120, -counter3, 50, counter3, QPen(Qt::green), QBrush(Qt::green));
        bar4 = scene->addRect(180, -counter4, 50, counter4, QPen(Qt::yellow), QBrush(Qt::yellow));
    } else {
        // Добавляем линии
        lineSeries.clear();
        lineSeries.append(QPointF(0, counter1));
        lineSeries.append(QPointF(60, counter2));
        lineSeries.append(QPointF(120, counter3));
        lineSeries.append(QPointF(180, counter4));

        // Добавляем линии на сцену
        for (int i = 1; i < lineSeries.size(); ++i) {
            scene->addLine(lineSeries[i-1].x(), lineSeries[i-1].y(),
                           lineSeries[i].x(), lineSeries[i].y(),
                           QPen(Qt::black));
        }
    }
}

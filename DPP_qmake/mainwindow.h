#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//Подключает класс ArduinoClock, который используется для связи с Arduino через последовательный порт.
#include "arduinonano.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //Слот — это метод, который можно связать с сигналом (например, при нажатии кнопки).
public slots:
    // Вызывается при подключении к Arduino. Вероятно, связана с нажатием кнопки «Подключить».
    void onConnect();
    // Вызывается при нажатии кнопки "Установить время".
    void onSetKoeffClick();
    // Обновляет время в интерфейсе, используя данные, полученные от Arduino.
    //Этот слот, вероятно, соединён с сигналом timeChanged из класса ArduinoClock.
    void updateData(int a, int b, int c, int d);

    // Обновление гистограммы
    void updateHistogram(int counter1, int counter2, int counter3, int counter4);

private:
    Ui::MainWindow *ui;
    //Экземпляр класса ArduinoClock, отвечающий за связь с Arduino.
    ArduinoNano arduinoNano;

    QGraphicsScene *scene; // Сцена для графики
    QGraphicsRectItem *bar1; // Первый столбец
    QGraphicsRectItem *bar2; // Второй столбец
    QGraphicsRectItem *bar3; // Третий столбец
    QGraphicsRectItem *bar4; // Четвертый столбец


};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QSpinBox>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , scene2(new QGraphicsScene(this))  // Инициализация сцены
{
    ui->setupUi(this);

    QMenu *fileMenu = menuBar()->addMenu("Файл");
    QAction *openAction = new QAction("Открыть", this);
    QAction *saveAction = new QAction("Сохранить", this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    // Привязка сцены к QGraphicsView
    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene2);

    // Связываем нажатие кнопки с открытием файла
    connect(ui->openFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    //connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawExample(){
    scene->clear();
    scene2 ->clear();
    std::vector<Dot> data;
    std::vector<Point> points;
    data = compare.getData();
    for (auto i : data) points.push_back(polarToDec(i));
    for(int i = 0 ; i < points.size(); i++)
        scene->addEllipse(points[i].x * 30, points[i].y * 30, 5, 5, QPen(Qt::blue), QBrush(Qt::blue));
}


void MainWindow::openFile()
{
    // Открываем диалог выбора файла
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty())
        return;  // Если файл не выбран, выходим

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
        return;
    }

    // Очищаем сцену перед загрузкой новых данных
    scene->clear();
    scene2->clear();

    QTextStream in(&file);
    std::vector<Dot> data;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList coordinates = line.split(' ');
        Point point;
        Dot dot;
        if (coordinates.size() == 2) {
            bool ok1, ok2;

            point.x = coordinates[0].toDouble(&ok1);
            point.y = coordinates[1].toDouble(&ok2);
            dot = decToPolar(point);
            // dot.R = coordinates[0].toDouble(&ok1);
            // dot.ang = coordinates[1].toDouble(&ok2);

            if (ok1 && ok2) {
                data.push_back(dot);
                // Добавляем точку на сцену
                //scene->addEllipse(point.x * 30, point.y * 30, 5, 5, QPen(Qt::blue), QBrush(Qt::blue));
            }
        }
    }
    compare.setData(data);
    drawExample();
}

void MainWindow::on_pushButton_clicked()
{
    // QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text Files (*.txt);;All Files (*)");

    // if (fileName.isEmpty())
    //     return;  // Если файл не выбран, выходим

    // QFile file(fileName);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
    //     return;
    // }
    // QTextStream in(&file);
    // std::vector<Dot> data;
    // while (!in.atEnd()) {
    //     QString line = in.readLine();
    //     QStringList coordinates = line.split(' ');
    //     Dot dot;
    //     Point point;
    //     if (coordinates.size() == 2) {
    //         bool ok1, ok2;
    //         // dot.R = coordinates[0].toDouble(&ok1);
    //         // dot.ang = coordinates[1].toDouble(&ok2);
    //         point.x = coordinates[0].toDouble(&ok1);
    //         point.y = coordinates[1].toDouble(&ok2);
    //         dot = decToPolar(point);
    //         //point = polarToDec(dot);

    //         if (ok1 && ok2) {
    //             data.push_back(dot);
    //             // Добавляем точку на сцену
    //             //if()
    //             // scene->addEllipse(point.x * 30, point.y * 30, 5, 5, QPen(Qt::blue), QBrush(Qt::blue));
    //         }
    //     }
    // }

    scene2->clear();
    int medianStep = ui->spinBox->value();
    int minimizeStep = ui->spinBox_2->value();
    double expVal = ui->doubleSpinBox->value();
    DotMesh res = compare;

    std::vector<Dot> data = res.getData();

    if (minimizeStep > 0) minimizeData(data, minimizeStep);
    applyMedianFilter(data, medianStep);

    if (expVal > 0) applyExponentialSmoothing(data, expVal);

    // for (int i = 0; i < res.size(); i++) data[i].R = res[i];
    example.setData(data);
    std::vector<Dot> compData = example.getData(); // compare
    res.calcDiff(example);
    std::vector<double> diff = res.get_delt(); // compare

    restore.clear();

    for (int i = 0; i < example.getDataSize(); i++){
        Point point = polarToDec(compData[i]);
        if (fabs(diff[i]) < 0.15) {
            scene2->addEllipse(point.x * 30, point.y * 30, 5, 5, QPen(Qt::green), QBrush(Qt::green));
            restore.push_back(compData[i]);
        }
        else {
            scene2->addEllipse(point.x * 30, point.y * 30, 5, 5, QPen(Qt::red), QBrush(Qt::red));
        }
    }
}

void MainWindow::on_saveCurButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (int i = 0 ; i < restore.size(); i++){
        Point a;
        a = polarToDec(restore[i]);
        out << a.x << " " << a.y << "\n";
    }
    file.close();
    qDebug() << "Запись завершена.";
}

void MainWindow::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    std::vector<Dot> dataD;
    dataD = compare.getData();
    if(ui->checkBox->isChecked()){


        for (int i = 0; i < dataD.size(); i++){
            Point a;
            a.x = dataD[i].R; a.y = dataD[i].ang;
            dataD[i] = decToPolar(a);
        }

    }
    else {

        for (int i = 0; i < dataD.size(); i++){
            Point a;
            a = polarToDec(dataD[i]);
            dataD[i].R = a.x; dataD[i].ang = a.y;
        }

    }
    compare.setData(dataD);
    drawExample();
}


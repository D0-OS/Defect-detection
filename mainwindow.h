#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "dotmesh.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class graphicsView;
class openFileButton;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void drawExample();

    void openFile();  // Слот для выбора файла и отображения данных

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_saveCurButton_clicked();

    void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;  // Сцена для отрисовки точек
    QGraphicsScene *scene2;
    DotMesh example;
    std::vector<Dot> restore;
    DotMesh compare;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "smart_calc.h"

#ifdef __cplusplus
}
#endif

#include <QMainWindow>
#include <QVector>
//#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_EqButton_clicked();

    void on_ClearButton_clicked();

    void on_PlotButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

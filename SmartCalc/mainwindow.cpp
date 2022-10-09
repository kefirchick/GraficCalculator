#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_EqButton_clicked()
{
    setlocale(LC_NUMERIC, "C");
    ui->labelResult->setEnabled(true);
    QString q_equation = ui->textEdit->toPlainText();
    std::string str = q_equation.toStdString();
    const char *c_equation = str.c_str();

    char new_equation[256] = "";
    int ret = eq_button_handle(c_equation, new_equation);
    if (ret) {
        q_equation = QString::fromUtf8(new_equation);
        ui->labelResult->setText(q_equation);
    } else {
        ui->labelResult->setEnabled(true);
        ui->labelResult->setText("CALC ERROR");
    }
}

void MainWindow::on_ClearButton_clicked()
{
    ui->textEdit->clear();
    ui->labelResult->setText("");
    ui->widget->clearGraphs();
    ui->widget->replot();


}

void MainWindow::on_PlotButton_clicked()
{
    setlocale(LC_NUMERIC, "C");
    QVector<double> x(256), y(256);
    double yArr[256];
    ui->widget->clearGraphs();

    QString q_equation = ui->textEdit->toPlainText();
    std::string str = q_equation.toStdString();
    const char *c_equation = str.c_str();
    double step = (ui->xEnd->value() - ui->xBegin->value())/256.;
    int ret = plot_button_handle(c_equation, yArr, ui->xBegin->value(), ui->xEnd->value());

    if (ret) {
        for (int i = 0; i < 256; i++) {
            x[i] = ui->xBegin->value() + step * i;
            y[i] = yArr[i];
        }

        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x,y);
        ui->widget->graph(0)->rescaleAxes();
        ui->widget->replot();
    } else {
        ui->labelResult->setEnabled(true);
        ui->labelResult->setText("PLOT ERROR");
    }
}

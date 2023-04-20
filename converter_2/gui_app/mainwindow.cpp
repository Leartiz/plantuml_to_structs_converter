#include <sstream>

#include <QString>

#include "usecasegraph.h"
#include "robustnessgraph.h"
#include "sequencegraph.h"
#include "classgraph.h"
#include "grapherror.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushBtnConvert_clicked()
{
    ui->textEditJson->clear();
    ui->statusbar->clearMessage();

    std::istringstream sin{ ui->textEditWsd->toPlainText().toStdString() };
    std::ostringstream sout;

    try {
        if (ui->radioButtonUc->isChecked()) {
            UseCaseGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (ui->radioButtonRob->isChecked()) {
            RobustnessGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (ui->radioButtonSeq->isChecked()) {
            SequenceGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (ui->radioButtonClass->isChecked()) {
            ClassGraph g; g.read_puml(sin);
            g.write_json(sout);
        }

        ui->textEditJson->setText(QString::fromStdString(sout.str()));
    }
    catch (const GraphError& err) {
        ui->statusbar->showMessage(QString::fromStdString(err.complete_message()));
        return;
    }
    catch (const std::runtime_error& err) {
        ui->statusbar->showMessage(err.what());
        return;
    }

    ui->statusbar->showMessage("[OK]");
}


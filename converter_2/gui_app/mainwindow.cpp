#include <sstream>

#include <QString>
#include <QByteArray>

#include <QJsonDocument>
#include <QJsonObject>

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
    , m_model(new JsonModel(this))
{
    ui->setupUi(this);
    ui->treeViewJson->setModel(m_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -----------------------------------------------------------------------

void MainWindow::on_pushBtnConvert_clicked()
{
    ui->textEditJson->clear();
    m_model->setJson(QJsonObject{});

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
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(sout.str()).toUtf8());
        m_model->setJson(jsonDoc.object());
    }
    catch (const GraphError& err) {
        ui->textEditJson->setText(QString::fromStdString(err.complete_message()));
        return;
    }
    catch (const std::runtime_error& err) {
        ui->textEditJson->setText(QString::fromStdString(err.what()));
        return;
    }
}




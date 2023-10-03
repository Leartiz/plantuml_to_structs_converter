#include <sstream>

#include <QString>
#include <QByteArray>

#include <QJsonDocument>
#include <QJsonObject>

#include "usecasegraph.h"
#include "layoutflowgraph.h"
#include "robustnessgraph.h"
#include "sequencegraph.h"
#include "classgraph.h"
#include "grapherror.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_model(new JsonModel(this))
{
    m_ui->setupUi(this);
    m_ui->treeViewJson->setModel(m_model);

    // ui
    {
        connect(m_ui->pushBtnConvert, &QPushButton::clicked,
                this, &MainWindow::onClicked_pushBtnConvert);
    }

    // geometry
    {

    }
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_model;
}

// -----------------------------------------------------------------------

void MainWindow::onClicked_pushBtnConvert()
{
    m_ui->plainTextEditLog->clear();

    m_ui->textEditJson->clear();
    m_model->setJson(QJsonObject{});

    // ***

    std::istringstream sin{
        m_ui->codeEditor->toPlainText().toStdString()
    };
    std::ostringstream sout;

    try {
        if (m_ui->radioButtonUc->isChecked()) {
            UseCaseGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (m_ui->radioButtonRob->isChecked()) {
            RobustnessGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (m_ui->radioButtonSeq->isChecked()) {
            SequenceGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (m_ui->radioButtonClass->isChecked()) {
            ClassGraph g; g.read_puml(sin);
            g.write_json(sout);
        }
        else if (m_ui->radioButtonLw->isChecked()) {
            LayoutFlowGraph g; g.read_puml(sin);
            g.write_json(sout);
        }

        // ***

        m_ui->textEditJson->setText(
            QString::fromStdString(
                sout.str()));

        m_model->setJson(
            QJsonDocument::fromJson(
                QString::fromStdString(
                    sout.str()).toUtf8()).object());
        m_model->dumpObjectTree();
    }
    catch (const GraphError& err) {
        m_ui->plainTextEditLog->setPlainText("graph err: " + QString::fromStdString(err.what()));
        return;
    }
    catch (const std::runtime_error& err) {
        m_ui->plainTextEditLog->setPlainText("runtime err: " + QString::fromStdString(err.what()));
        return;
    }

    m_ui->plainTextEditLog->setPlainText(
        "puml2stts - [OK]");
}




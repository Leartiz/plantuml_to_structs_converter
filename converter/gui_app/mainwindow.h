#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QCodeEditor>

#include "jsonmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClicked_pushBtnConvert();

private:


private:
    Ui::MainWindow *m_ui;
    JsonModel *m_model{ nullptr };
};
#endif // MAINWINDOW_H

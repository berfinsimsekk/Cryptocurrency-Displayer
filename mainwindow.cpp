#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TableWidgetDisplay();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TableWidgetDisplay(){
    QTableWidget *table = new QTableWidget(this);

    table->setColumnCount(4);
    QStringList first;
    first << " "<<"USD"<<"EUR"<<"GBP";
    table->setHorizontalHeaderLabels(first);

    for(int column=0; column<4;column++){
        QTableWidgetItem *item;
        table->insertRow(column);

        for(int row=0;row<5;row++){
            item =new QTableWidgetItem;
            item->setText("Bitcoin");

            table->setItem(row,0,item);
        }

    }

    this->setCentralWidget(table);
}





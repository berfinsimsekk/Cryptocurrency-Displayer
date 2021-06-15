#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QRegExp>
#include <QFile>

int numberOfLines=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this) ;
    QString path=qgetenv("MYCRYPTOCONVERT");
    QFile inputFile(path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          numberOfLines++;
       }
       inputFile.close();
    }


    connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(TableWidgetDisplay(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd,eur,gbp")));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TableWidgetDisplay(QNetworkReply *reply){
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(4);
    QStringList first;
    first << " "<<"USD"<<"EUR"<<"GBP";
    table->setHorizontalHeaderLabels(first);

    // read the data fetched from the web site
    QString data = (QString) reply->readAll();

    // use pattern matching to extract the rate
    QRegExp rx("\"usd\":(\\d+),\"eur\":(\\d+),\"gbp\":(\\d+)");


    for(int row=0; row<numberOfLines;row++){
        QTableWidgetItem *item;
        QTableWidgetItem *coin;
        coin = new QTableWidgetItem;
        table->insertRow(row);
        coin->setText("Bitcoin");
        table->setItem(row,0,coin);

        for(int column=1; column<4;column++){
            item =new QTableWidgetItem;
            int pos=0;
            if ( rx.indexIn(data, pos) != -1 ) {
               item->setText(rx.cap(column));
            }else{
                item->setText("Error");
            }

            table->setItem(row,column,item);

        }

    }
    table->verticalHeader()->hide();
    this->setCentralWidget(table);

}
